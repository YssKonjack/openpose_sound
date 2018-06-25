#ifndef PS_PLAYSOUND_H
#define PS_PLAYSOUND_H

namespace ps{
    class playSound{
    private:
        const double sampleFreq;
        double Vol;
        bool boolSound;
        bool boolSwitchON;
        int fd;

    public:
        playSound() :
            sampleFreq(44100),
            Vol(10000),
            boolSound(false),
            boolSwitchON(false),
            // fp(),
            fd()
        {}

        void master();
        // void controller();

    private:
        void player();
        void stop(){boolSwitchON = false;}

        void playSine();



    // private:
    //     class Sine {
    //     private:
    //         double Amp
    //         double Freq;
    //         short data;
    //
    //     };

    };
}


#endif
