#ifndef DEMULTIPLEXER_HPP_
#define DEMULTIPLEXER_HPP_

class demultiplexer {
public:
    demultiplexer(int selector_pins[], int value_pin[], int complexity, int number_of_demu);
    ~demultiplexer();
    void update();
    int* value;
    int get_value(int index);
    bool get_change_indicator();
    bool get_change_index(int index);

private:
    int complexity;
    int* selector_pins;
    int* value_pin;
    int number_of_demu;

    int number_of_value;
    
    int* value_old;
    bool* change_indexes;
    bool change_indicator;
};

#endif /* DEMULTIPLEXER_HPP_ */