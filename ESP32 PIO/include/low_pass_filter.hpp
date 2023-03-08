#ifndef LOW_PASS_FILTER_HPP
#define LOW_PASS_FILTER_HPP

#include <Arduino.h>
struct low_pass_filter
{
    double alpha;
    void compute_alpha(int cut_off_freq, int sample_rate);
    float compute_output(int &input);
    float output;
    bool inited=false;
};
#endif