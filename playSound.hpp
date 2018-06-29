#ifndef PS_PLAYSOUND_H
#define PS_PLAYSOUND_H

namespace ps{
    class playSound{
    private:
        const double sampleFreq;
        double Vol;
        bool boolSwitchON;
        int fd;

    public:
        playSound() :
            sampleFreq(44100),
            Vol(10000),
            boolSwitchON(false),
            fd()
        {}

        virtual void master();

    private:
        void player();
        void stop(){boolSwitchON = false;}
        void playSineTest();
    };



    class Sine : public playSound{
    private:
        double Amp;
        double F;
        short data;
        bool boolSound;

    public:
        playSoundSine() :
            Amp(10000),
            F(),
            data(),
            boolSound(false)
        {}

        void master() override {}
        void controller();

    private:
        playSine();

    };
}

#endif
