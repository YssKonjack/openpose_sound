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
        short data;

        while(!boolSwitchON){}

        for(int t = 0; t < 50000; t++){
            data = 10000*sin(2*M_PI*442.0*(double)t/sampleFreq);
            int check = write(fd, &data, sizeof(data)); if(-1==check) die("write");
        }

        stop();
    }

    void playSound::master(){
        std::thread t1(&playSound::player,this);
        threadGuard tg1(t1,"player");

        std::thread t2(&playSound::playSineTest,this);
        threadGuard tg2(t2,"sine");

    }






    void ps::Sine::controller(){
        //座標値の最大値を受け取りたい
        float val;

        //openposeからのデータが来るまで待つ
        while(!poseArray.stFlg()){}
        boolSound = true;

        while(boolSwitchON){
            if(!poseArray.stFlg()) stop();

            //openposeの入力を受け取る
            //getKpVal(関節,変数(x,y,score))
            // val = (poseArray.getKpVal(0,0) / 1268.0 - 0.5) * 100;
            val = poseArray.getKpVal(0,0);
            F = val;
            // F = F * pow(pow(2,1/12),val);
        }
    }

    void ps::Sine::synthesizer(){
        while(!boolSwitchON){}

        int t = 0;

        while(boolSwitchON){
            if(boolSound){
                data = Vol*Amp*sin(2*M_PI*F*(double)t/sampleFreq);
                int check = write(fd, &data, sizeof(data)); if(-1==check) die("write");
                t++;
            }
        }
    }


    void ps::Sine::master(){
        std::thread t1(&ps::Sine::player,this);
        threadGuard tg1(t1,"player");

        std::thread t2(&ps::Sine::controller,this);
        threadGuard tg2(t2,"controller");

        std::thread t3(&ps::Sine::synthesizer,this);
        threadGuard tg3(t3,"synthesizer");

        poseArray.readData(0);
        boolSwitchON = false;
    }

}


// int main(){
//     ps::playSound player;
//     player.master();
// }
