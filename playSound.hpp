#ifndef PLAYSOUND_H
#define PLAYSOUND_H


namespace ps{
    class playSound{
    private:
        const double sampleFreq;
        double Amp;
        double Freq;

        bool boolSound;
        bool boolSwitch;

        FILE *fp;
        int fd;

    public:
        playSound() :
            sampleFreq(44100),
            Amp(10000),
            Freq(0),
            boolSound(false),
            boolSwitch(true),
            fp(),
        {}

        void player();
        void stop(){boolSwitch = false;}

        void master();


    };
}


class sine : public playSound{

};

#endif
