#include "broomstick.hpp"

Broomstick::Broomstick()
{
}
Broomstick::~Broomstick()
{
}

int Broomstick::begin()
{   
    pinMode(GPIO_MAIN_JOY_X, INPUT);
    pinMode(GPIO_MAIN_JOY_Y, INPUT);
    pinMode(GPIO_FIRE, INPUT_PULLUP);
    pinMode(GPIO_THROTTLE, INPUT);
    pinMode(GPIO_BREAK, INPUT_PULLUP);
    analogReadResolution(ANALOG_READ_RESOLUTION);

    main_joy_x_filter.compute_alpha(FILTER_CUTOFF_FREQUENCY, COMPUTING_RATE);
    main_joy_y_filter.compute_alpha(FILTER_CUTOFF_FREQUENCY, COMPUTING_RATE);
    throttle_filter.compute_alpha(FILTER_CUTOFF_FREQUENCY, COMPUTING_RATE);

    demu.set_analog(30);
    demu.set_analog(24);
    demu.set_analog(26);

    if (!EEPROM.begin(CALIBRATION_EEPROM_SIZE))
        return -1; // EEPROM error

    if (loadCalibration() < 0)
        return 0; // no calibration found
    
    #ifdef USE_BLE
        if (beginBluetooth() < 0)
            return -2; // bluetooth error
    #endif
    return 1;     // calibration found
}

int Broomstick::readSensors()
{   
    data.main_joy_x = analogRead(GPIO_MAIN_JOY_X);
    data.main_joy_y = analogRead(GPIO_MAIN_JOY_Y);
    data.throttle = analogRead(GPIO_THROTTLE);
    data.brake_ = !digitalRead(GPIO_BREAK);   
    correction_done = false;
    return 0;
}

int Broomstick::readPannel()
{
    demu.update();
    data.enable_pilot_auto = !demu.get_value(10);
    data.enable_battery = !demu.get_value(15);
    data.enable_engine = !demu.get_value(21);
    data.eject = !demu.get_value(9);
    data.fumigene = !demu.get_value(8);
    data.feux_de_position = !demu.get_value(17);
    data.bp1 = !demu.get_value(8);
    data.bp2 = !demu.get_value(9);
    data.bp3 = !demu.get_value(19);
    data.enable_trains_datterrissage = !demu.get_value(12);
    data.aerofrein_up = !demu.get_value(13);
    data.aerofrein_middle = !demu.get_value(7);
    data.aerofrein_down = !demu.get_value(5);
    data.volet_up = !demu.get_value(23);
    data.volet_middle = !demu.get_value(3);
    data.volet_down = !demu.get_value(1);
    data.parachute = !demu.get_value(0);
    data.eject = !demu.get_value(14);
    pad.update(demu.get_value(PAD_CHANNEL));
    data.fire = !demu.get_value(25);
    data.rotation_x = demu.get_value(30);
    data.rotation_y = demu.get_value(24);
    data.cycle_arm = !demu.get_value(29);
    data.cycle_mode = !demu.get_value(31);
    data.contre_mesure = pad.getButton(1);
    data.bp4 = pad.getButton(2);
    data.bp5 = pad.getButton(0);
    return 0;
}

int Broomstick::applyCorrection()
{
    if (correction_done)
        return 0; // correction already done

    // map filter output to 0-2^ANALOG_READ_RESOLUTION
    // correct offsets and gains
    if (data.main_joy_x > calib.main_joy_x_offset)
        data.main_joy_x = pow(2, ANALOG_READ_RESOLUTION - 1) * (1.0f + (double)(data.main_joy_x - calib.main_joy_x_offset) / (calib.main_joy_x_max - calib.main_joy_x_offset));
    else
        data.main_joy_x = pow(2, ANALOG_READ_RESOLUTION - 1) * (1.0f + (double)(data.main_joy_x - calib.main_joy_x_offset) / (calib.main_joy_x_offset - calib.main_joy_x_min));

    if (data.main_joy_y > calib.main_joy_y_offset)
        data.main_joy_y = pow(2, ANALOG_READ_RESOLUTION - 1) * (1.0f + (double)(data.main_joy_y - calib.main_joy_y_offset) / (calib.main_joy_y_max - calib.main_joy_y_offset));
    else
        data.main_joy_y = pow(2, ANALOG_READ_RESOLUTION - 1) * (1.0f + (double)(data.main_joy_y - calib.main_joy_y_offset) / (calib.main_joy_y_offset - calib.main_joy_y_min));
#ifdef APPLY_FILTER
    data.main_joy_x = round(main_joy_x_filter.compute_output(data.main_joy_x));
    data.main_joy_y = round(main_joy_y_filter.compute_output(data.main_joy_y));
    data.throttle = round(throttle_filter.compute_output(data.throttle));
#endif
    correction_done = true;
    return 0;
}

int Broomstick::centerCalibration()
{
    // parameters of the calibration
    const unsigned int nb_samples = 100;
    const unsigned int delay_between_samples = 10;

    Serial.println("Please put the broomstick in the center position and press enter");
    while (Serial.available() == 0)
        ;
    Serial.read();

    // estimate offset and std noise
    float E_x[2] = {0, 0};
    float E_x2[2] = {0, 0};
    int *x[2] = {&data.main_joy_x, &data.main_joy_y};

    for (unsigned int i = 0; i < nb_samples; i++)
    {
        readSensors();
        for (unsigned int j = 0; j < 2; j++)
        {
            E_x[j] += *x[j];
            E_x2[j] += (*x[j]) * (*x[j]);
        }
        delay(delay_between_samples);
    }
    E_x[0] /= nb_samples;
    E_x[1] /= nb_samples;
    E_x2[0] /= nb_samples;
    E_x2[1] /= nb_samples;
    calib.main_joy_x_offset = E_x[0];
    calib.main_joy_y_offset = E_x[1];
    calib.main_joy_x_std_var = E_x2[0] - E_x[0] * E_x[0];
    calib.main_joy_y_std_var = E_x2[1] - E_x[1] * E_x[1];

    return 0;
}

int Broomstick::extremumCalibration()
{
    // parameters of the calibration
    const unsigned int duration = 10000;
    const unsigned int delay_between_samples = 10;

    Serial.println("Please move slowly the broomstick in all extrem directions during " + String(duration / 1000) + "seconds");
    delay(1000);

    int extremum[2][2] = {{data.main_joy_x, data.main_joy_x}, {data.main_joy_y, data.main_joy_y}};
    int *x[2] = {&data.main_joy_x, &data.main_joy_y};
    const unsigned long start_time = millis();
    while (millis() - start_time < duration)
    {
        readSensors();
        for (unsigned int j = 0; j < 2; j++)
        {
            if (*x[j] > extremum[j][1])
            {
                extremum[j][1] = *x[j];
            }
            if (*x[j] < extremum[j][0])
            {
                extremum[j][0] = *x[j];
            }
        }
        delay(delay_between_samples);
    }
    calib.main_joy_x_min = extremum[0][0];
    calib.main_joy_x_max = extremum[0][1];
    calib.main_joy_y_min = extremum[1][0];
    calib.main_joy_y_max = extremum[1][1];
    return 0;
}

int Broomstick::compute()
{   

    if (time - last_update_time >= 1000000 / COMPUTING_RATE)
    {   
        dt = time - last_update_time;
        readSensors();
        applyCorrection();
        last_update_time = time;
        return 1; // data updated
    }
    return 0; // data not updated because not enough time elapsed
}
void Broomstick::printData()
{
    Serial.print("main_joy_x:" + String(data.main_joy_x));
    Serial.print(",main_joy_y:" + String(data.main_joy_y));
    Serial.print(",fire:" + String(data.fire));
    Serial.print(",throttle:" + String(data.throttle));
    Serial.print(",pilot_auto:" + String(data.enable_pilot_auto));
    Serial.print(",Battery:" + String(data.enable_battery));
    Serial.print(",Motor:" + String(data.enable_engine));
    Serial.print(",Eject:" + String(data.eject));
    Serial.print(",frein:" + String(data.brake_));
    Serial.print(",RX:" + String(data.rotation_x));
    Serial.print(",RY:" + String(data.rotation_y));
    //Serial.print(",aerofre")

    #ifdef PRINT_FREQ
        Serial.print(",sample_freq:" + String(1000000.0/dt,2));
    #endif
    Serial.println("");
}
int Broomstick::run()
{   
    time = micros();
    compute();
    communicate();
    return 0;
}

int Broomstick::saveCalibration()
{
    EEPROM.put(CALIBRATION_EEPROM_ADDRESS, CALIBRATION_EEPROM_FLAG);
    EEPROM.put(CALIBRATION_EEPROM_ADDRESS + sizeof(CALIBRATION_EEPROM_FLAG), calib);
    if (!EEPROM.commit())
        return -1; // EEPROM error
    return 0;
}
int Broomstick::loadCalibration()
{
    char maybe_flag[sizeof(CALIBRATION_EEPROM_FLAG) / sizeof(char)];
    EEPROM.get(CALIBRATION_EEPROM_ADDRESS, maybe_flag);
    if (strcmp(maybe_flag, CALIBRATION_EEPROM_FLAG) != 0)
        return -1; // No calibration found

    EEPROM.get(CALIBRATION_EEPROM_ADDRESS + sizeof(CALIBRATION_EEPROM_FLAG), calib);
    return 0;
}
void Broomstick::printCalibration()
{
    Serial.println("main_joy_x_offset: " + String(calib.main_joy_x_offset));
    Serial.println("main_joy_y_offset: " + String(calib.main_joy_y_offset));
    Serial.println("main_joy_x_std_var: " + String(calib.main_joy_x_std_var));
    Serial.println("main_joy_y_std_var: " + String(calib.main_joy_y_std_var));
    Serial.println("main_joy_x_min: " + String(calib.main_joy_x_min));
    Serial.println("main_joy_x_max: " + String(calib.main_joy_x_max));
    Serial.println("main_joy_y_min: " + String(calib.main_joy_y_min));
    Serial.println("main_joy_y_max: " + String(calib.main_joy_y_max));
}