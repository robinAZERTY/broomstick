//build a class for the analogic pad

class analPad
{
    public : 
        analPad(const int thres[], int nbButton);
        ~analPad();
        void update(int analogicValue);
        bool getButton(int index);
    private :
        int nbButton;
        int* thres;
        bool* button;
        int combinations;
};