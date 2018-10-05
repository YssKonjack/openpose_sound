#ifndef PS_PLAYSOUND_H
#define PS_PLAYSOUND_H

#include "poseKeyPoints.hpp"

//g++ -pthread -std=c++11 playSound.cpp -o playSoundTest.bin

namespace ps{
    class playSound{
    protected:
        int fd;//ファイルディスクリプタ
        const float sampleFreq;//サンプリング周波数
        float Vol;//0...1.0
        bool boolSwitchON;//soxの準備が完了したらONに

    public:
        playSound() :
            fd(),
            sampleFreq(44100.0),
            Vol(1.0),
            boolSwitchON(false)
        {}

        virtual void master();
        void player();//soxを起動する

        protected:
        void stop(){boolSwitchON = false;}
    };


    ////////////////////////////////////////////

    class Chord : public playSound{
    private:
        float Amp;

        float root;//根音 最低音とは限らない
        int level; //スケール上の位置 0...1
        bool Major;//メジャーコード？マイナーコード？
        float firstFreq;//実際に鳴らす和音の構成音　rootとlevelから一意に決まる
        float thirdFreq;
        float fifthFreq;

        short soundData;//soxに渡すデータ
        bool  boolSound;//音のon/off
        pkp::PoseKeyPoints poseArray;

    public:
        Chord() :
            Amp(10000.0),
            root(523.25),//C Major
            level(1),
            Major(true),
            firstFreq(0.0),
            thirdFreq(0.0),
            fifthFreq(0.0),
            soundData(0),
            boolSound(false),
            poseArray()
        {}

        void master() override;
        void controller1();//規則にしたがって和音を遷移させる
        void controller2();
        void synthesizer();//実際にsoxに音を渡して再生する

    private:
        void transChordX(bool positive);//和音のx方向の遷移
        void transChordY(bool positive);//y方向の遷移　転回
        void printChord();

        void makeChord();//root(周波数)とスケール上の位置(level)から和音を一つに決める

    };

}



#endif
