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
            sampleFreq(44100.0),
            Vol(1.0),
            boolSwitchON(false)
        {}

        virtual void master();
        void player();

        protected:
        void stop(){boolSwitchON = false;}

    private:
        void playSineTest();
    };

    ///////////////////////////////////////////

    // class Sine : public playSound{
    // private:
    //     float Amp;
    //     float Freq;
    //     short soundData;
    //     bool boolSound;
    //     pkp::PoseKeyPoints poseArray;
    //
    // public:
    //     Sine() :
    //         Amp(10000.0),
    //         Freq(0.0),
    //         soundData(0),
    //         boolSound(false),
    //         poseArray()
    //     {}
    //
    //     void master() override;
    //
    // // private:
    //     void controller();
    //     void synthesizer();
    //
    // };

    ////////////////////////////////////////////

    class Chord : public playSound{
    private:
        float Amp;
        float rootFreq;
        float thirdFreq;
        float fifthFreq;
        short soundData;
        bool  boolSound;
        pkp::PoseKeyPoints poseArray;

    public:
        Chord() :
            Amp(10000.0),
            rootFreq(442.0),
            thirdFreq(0.0),
            fifthFreq(0.0),
            soundData(0),
            boolSound(false),
            poseArray()
        {}

        void master() override;
        void controller();
        void synthesizer();

    private:
        void transChordX(bool positive);//和音のx方向の遷移
        void transChordY(bool positive);//y方向の遷移



    };

}



#endif
