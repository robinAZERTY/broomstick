#include "low_pass_filter.hpp"

void low_pass_filter::compute_alpha(int cut_off_freq,int sample_rate)
{
    alpha = (double)1.0 - exp(-2*PI * (double)cut_off_freq / (double)sample_rate);
}

float low_pass_filter::compute_output(int &input)
{   
    if(!inited)
        output = (double)input;
    else
        output = alpha*(double)input + (1-alpha)*output;
    inited = true;
    return output;
}