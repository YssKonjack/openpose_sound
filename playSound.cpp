#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "die.hpp"
#include "threadGuard.hpp"
#include "playSound.hpp"

namespace ps{
    void playSound::player(){
        std::string f = std::to_string(sampleFreq);
        std::string cmd = "play -t raw -b 16 -c 1 -e s -r " +  f + " -";
        const char *c_cmd = cmd.c_str();//string -> char

        FILE *fp;
        if (NULL == (fp = popen( c_cmd, "w"))) die("popen");
        fd = fileno(fp);
        boolSwitchON = true;

        while(boolSwitchON){}

        pclose(fp);
    }

    void playSound::playSineTest(){
        short soundData;

        while(!boolSwitchON){}

        for(int t = 0; t < 50000; t++){
            soundData = 10000*sin(2*M_PI*442.0*(double)t/sampleFreq);
            int check = write(fd, &soundData, sizeof(soundData)); if(-1==check) die("write");
        }

        stop();
    }

    void playSound::master(){
        std::thread t1(&playSound::player,this);
        threadGuard tg1(t1,"player");

        std::thread t2(&playSound::playSineTest,this);
        threadGuard tg2(t2,"sine");
    }


    ///////////////////////////////////////////////


    // void Sine::controller(){
    //     //座標値の最大値を受け取りたい
    //     float val;
    //
    //     //openposeからのデータが来るまで待つ
    //     while(!poseArray.stFlg()){}
    //     boolSound = true;
    //
    //     while(boolSwitchON){
    //         if(!poseArray.stFlg()) stop();
    //
    //         //openposeの入力を受け取る
    //         //getKpVal(関節,変数(x,y,score))
    //         // val = (poseArray.getKpVal(0,0) / 1268.0 - 0.5) * 100;
    //         val = poseArray.getKpVal(0,0);
    //         Freq = val;
    //         // F = F * pow(pow(2,1/12),val);
    //     }
    // }
    //
    // void Sine::synthesizer(){
    //     while(!boolSwitchON){}
    //
    //     int t = 0;
    //
    //     while(boolSwitchON){
    //         if(boolSound){
    //             soundData = Vol*Amp*sin(2*M_PI*Freq*(double)t/sampleFreq);
    //             int check = write(fd, &soundData, sizeof(soundData)); if(-1==check) die("write");
    //             t++;
    //         }
    //     }
    // }
    //
    //
    // void Sine::master(){
    //     std::thread t1(&ps::Sine::player,this);
    //     threadGuard tg1(t1,"player");
    //
    //     std::thread t2(&ps::Sine::controller,this);
    //     threadGuard tg2(t2,"controller");
    //
    //     std::thread t3(&ps::Sine::synthesizer,this);
    //     threadGuard tg3(t3,"synthesizer");
    //
    //     poseArray.readData(0);
    //     boolSwitchON = false;
    // }


    /////////////////////////////////////////

    void Chord::transChordX(bool positive){
        std::cout << "trans X" << std::endl;



        if(true == positive){
            if(fifthFreq < 1600){
                rootFreq = thirdFreq;
                thirdFreq = fifthFreq;
                fifthFreq = rootFreq * pow(2.0,7.0/12.0);//根音から完全5度上
            }
        }else{
            if(rootFreq > 200){
                fifthFreq = thirdFreq;
                thirdFreq = rootFreq;
                rootFreq = fifthFreq / pow(2.0,7.0/12.0);
            }
        }
    }

    //和音の転回
    void Chord::transChordY(bool positive){
        std::cout << "trans Y" << std::endl;

        float buf = 0.0;
        if(true == positive){
            if(fifthFreq < 1600){
                buf = fifthFreq;
                fifthFreq = rootFreq * 2.0;
                rootFreq = thirdFreq;
                thirdFreq = buf;

                // if(fifthFreq > 1600) fifthFreq /= pow(2.0,3);
            }
        }else{
            if(rootFreq > 200){
                buf = rootFreq;
                rootFreq = fifthFreq / 2.0;
                fifthFreq = thirdFreq;
                thirdFreq = buf;

                // if(rootFreq < 200) rootFreq *= pow(2.0,3);
            }
        }

    }

    void Chord::controller(){
        std::cout<< "controller start"<<std::endl;

        int x1;//直前のx
        int x2;//次のx
        int y1;//直前のy
        int y2;//次のy

        int diff = 5;//直前のjrtといくつ値が異なっていた時に和音を切り替えるか

        thirdFreq = rootFreq * pow(2.0,4.0/12.0);//長3度
        fifthFreq = rootFreq * pow(2.0,7.0/12.0);//完全5度

        //openposeからのデータが来るまで待つ
        while(!poseArray.stFlg()){}
        boolSound = true;

        x1 = poseArray.jrtPositiveX();
        y1 = poseArray.jrtPositiveY();


        while(boolSwitchON){
            if(!poseArray.stFlg()) stop();

            x2 = poseArray.jrtPositiveX();
            y2 = poseArray.jrtPositiveY();


            if(x2 > x1 + diff){
                transChordX(true);
            }else if(x2 < x1 - diff){
                transChordX(false);
            }else if(y2 > y1 + diff){
                transChordY(true);
            }else if(y2 < y1 - diff){
                transChordY(false);
            }

            x1 = x2;
            y1 = y2;

            // sleep(2);
            // transChordX(true);

        }
    }

    void Chord::synthesizer(){

        std::cout<< "sysnthesizer start"<<std::endl;

        while(!boolSwitchON){}

        int t = 0;

        while(boolSwitchON){
            if(boolSound){
                soundData = Vol*Amp*sin(2*M_PI*rootFreq*(double)t/sampleFreq)
                            +Vol*Amp*sin(2*M_PI*thirdFreq*(double)t/sampleFreq)
                            +Vol*Amp*sin(2*M_PI*fifthFreq*(double)t/sampleFreq);

                int check = write(fd, &soundData, sizeof(soundData)); if(-1==check) die("write");
                t++;
            }
        }
    }

    void Chord::master(){
        std::thread t1(&ps::Chord::player,this);
        threadGuard tg1(t1,"player");

        std::thread t2(&ps::Chord::controller,this);
        threadGuard tg2(t2,"controller");

        std::thread t3(&ps::Chord::synthesizer,this);
        threadGuard tg3(t3,"synthesizer");

        poseArray.readData(1);
        boolSwitchON = false;
    }
}
