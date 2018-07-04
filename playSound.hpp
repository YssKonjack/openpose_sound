#ifndef PS_PLAYSOUND_H
#define PS_PLAYSOUND_H

#include "poseKeyPoints.hpp"

//g++ -pthread -std=c++11 playSound.cpp -o playSoundTest.bin

namespace ps{
    class playSound{
    protected:
        int fd;
        const float sampleFreq;
        float Vol;
        bool boolSwitchON;

    public:
        playSound() :
            fd(),
            sampleFreq(44100),
            Vol(1.0),
            boolSwitchON(false)
        {}

        virtual void master();

    protected:
        void player();
        void stop(){boolSwitchON = false;}

    private:
        void playSineTest();
    };

///////////////////////////////////////////

    class Sine : public playSound{
    private:
        float Amp;
        float F;
        short data;
        bool boolSound;
        pkp::PoseKeyPoints poseArray;

    public:
        Sine() :
            Amp(10000.0),
            F(0.0),
            data(0),
            boolSound(false),
            poseArray()
        {}

        void master() override;

    private:
        void synthesizer();
        void controller();


    };
}

#endif
