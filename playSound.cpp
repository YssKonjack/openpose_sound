#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <mutex>

#include "die.hpp"
#include "threadGuard.hpp"
#include "playSound.hpp"

bool debug = false;//debug mode

std::mutex mtx;

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

    void playSound::master(){
        std::thread t1(&playSound::player,this);
        threadGuard tg1(t1,"player");
    }

    ///////////////////////////////////////////////

    void Chord::printChord(){
        std::cout << std::endl;
        std::cout << firstFreq  <<  " " << thirdFreq << " " << fifthFreq << std::endl;
    }

    // void Chord::setChord(){
    //
    // }

    //現在のrootとlevelから和音を作る first,third,fifthFreqの周波数設定
    void Chord::makeChord(){
        if(level < 0 || level > 6) die("wrong level");
        switch (level) {
            //基本形
            case 1:
            case 2:
            case 3:
                firstFreq = root;
                if(Major){//メジャーコードの時
                    thirdFreq = root * pow(2.0,4.0/12.0);//長3度
                }else{
                    thirdFreq = root * pow(2.0,3.0/12.0);//短3度
                }
                fifthFreq = root * pow(2.0,7.0/12.0);//完全5度
                break;
            //第一転回形
            case 4:
            case 5:
                firstFreq = root / pow(2.0,5.0/12.0);//1オクターブ低い第5音
                thirdFreq = root;
                if(Major){
                    fifthFreq = root * pow(2.0,4.0/12.0);//長3度
                }else{
                    fifthFreq = root * pow(2.0,3.0/12.0);//短3度
                }
                break;
            //第二転回形
            case 6:
            case 0:
                if(Major){
                    firstFreq = root / pow(2.0,8.0/12.0);//短6度
                }else{
                    firstFreq = root / pow(2.0,9.0/12.0);//長6度
                }
                thirdFreq = root / pow(2.0,5.0/12.0);
                fifthFreq = root;
                break;
            default:
                break;
        }
        // printChord();

    }


    //x方向の遷移　positiveは正の方向
    void Chord::transChordX(bool positive){
        if(level < 0 || level > 6) die("wrong level");
        // std::cout << std::endl;
        // std::cout << "trans X";

        if(positive){
            switch (level) {
                case 1:
                case 2:
                case 3:
                    root = thirdFreq;
                    break;
                case 4:
                case 5:
                    root = fifthFreq;
                    break;
                case 6:
                case 0:
                    root = firstFreq;
                    break;
                default:
                    break;
            }
            level = (level + 2) % 7;
        }else{
            switch (level) {
                case 1:
                case 2:
                    if(Major){
                        root = root * pow(2.0,9.0/12.0);//長6度
                    }else{
                        root = root * pow(2.0,8.0/12.0);//短6度
                    }
                    break;
                case 3:
                case 4:
                case 5:
                case 6:
                case 0:
                    if(Major){
                        root = root / pow(2.0,3.0/12.0);//短3度低く
                    }else{
                        root = root / pow(2.0,4.0/12.0);//長3度低く
                    }
                    break;
                default :
                    break;
            }
            level = (level + 5) % 7;
        }
        Major = !Major;
        makeChord();
    }


    //和音の転回 positive=trueの時、音は高くなり,levelの値は小さくなる
    void Chord::transChordY(bool positive){
        if(level < 0 || level > 6) die("wrong level");
        // std::cout << std::endl;
        // std::cout << "trans Y";

        if(positive){//音が高くなるとlevelの値は小さくなる
            if(fifthFreq < 1600){
                if(1 == level){
                    root *= 2.0;
                }
                level = (level + 7 - 1) % 7;
            }
        }else{
            if(firstFreq > 200){
                if(0 == level){
                    root /= 2.0;
                }
                level = (level + 1) % 7;
            }
        }
        makeChord();
    }


    //姿勢の変化とコードの遷移の関係
    void Chord::controller1(){
        std::cout<< "controller start"<<std::endl;

        makeChord();//プログラム開始時

        if(!debug){
            //openposeからのデータが来るまで待つ
            while(!poseArray.stFlg()){}
            boolSound = true;


            int x1;//直前のx
            int x2;//次のx
            int xdiff;
            int y1;//直前のy
            int y2;//次のy
            int ydiff;

            int th = 20;//直前のjrtといくつ値が異なっていた時に和音を切り替えるか

            x1 = poseArray.jrtPositiveX();
            y1 = poseArray.jrtPositiveY();

            while(boolSwitchON){
                if(!poseArray.stFlg()) stop();

                x2 = poseArray.jrtPositiveX();
                y2 = poseArray.jrtPositiveY();

                // x1 = poseArray.jrtPositiveX();
                // y1 = poseArray.jrtPositiveY();

                // std::cout << x1 << " " << y1 << std::endl;

                xdiff = x2-x1;
                ydiff = y2-y1;

                if(xdiff > ydiff){
                    if(xdiff > th){
                        transChordX(true);
                    }else if(xdiff > -1*th){
                        transChordX(false);
                    }
                }else{
                    if(ydiff >  th){
                        transChordY(true);
                    }else if(ydiff < -1*th){
                        transChordY(false);
                    }
                }


                x1 = x2;
                y1 = y2;
            }



        }else{//debug
            boolSound = true;
            while(boolSwitchON){}
        }
    }


    //各関節毎に前のフレームとの差分を取って、x or y、正、負どの方向への動きが最も大きかったか多数決で決める
    void Chord::controller2(){
        std::cout<< "controller2 start"<<std::endl;

        makeChord();

        if(!debug){
            while(!poseArray.stFlg()){}
            boolSound = true;

            int frameCnt = 0;
            int frameBefore = frameCnt;
            int keyPointNum = poseArray.kpNum();
            int variableNum = poseArray.varNum();
            float kpArray[2][keyPointNum][variableNum];//新旧２つ分記録
            int Old = 0;
            int New = 1;

            int th = 50;//関節がこの値以上動いたら和音を遷移させる
            int xdiff = 0;
            int ydiff = 0;
            bool transFlg = false;

            /*
            各関節ごとにx or y、正、負どの方向への動きが最も大きかったか
            score[0:x, 1:y][0:positive, 1:negative]
            */
            int score[2][2] = {{0,0},{0,0}};

            //score計算用　
            int max = -1;
            int maxi = -1;
            int maxj = -1;


            while(boolSwitchON){
                if(!poseArray.stFlg()) stop();

                Old = (frameCnt+1)%2;
                New = frameCnt%2;
                frameCnt = poseArray.frmCnt();

                if(frameBefore != frameCnt){//openposeの次のフレームに切り替わった時

                    mtx.lock();
                    for(int i = 0; i < keyPointNum; i++){
                        for(int j = 0; j < variableNum; j++){
                            kpArray[New][i][j] = poseArray.getKpVal(i, j);
                        }
                    }
                    mtx.unlock();

                    if(frameCnt > 1){//1番最初は除く (oldが空っぽ)
                        //各関節ごとにx,y方向の差分を計算
                        //xは画面左側(体の右側)が原点
                        //yは画面上が原点
                        for(int i = 0; i < keyPointNum; i++){

                            xdiff = (int)kpArray[New][i][0] - (int)kpArray[Old][i][0];

                            ydiff = (int)kpArray[New][i][1] - (int)kpArray[Old][i][1];
                            //手を上に上げた時に負になる

                            if(abs(xdiff) > abs(ydiff)){
                                if(abs(xdiff) > th){
                                    if(xdiff > 0){//体の左方向へ動かした時
                                        score[0][0]++;
                                    }else{
                                        score[0][1]++;
                                    }
                                    // transFlg = true;
                                }
                            }else{
                                if(abs(ydiff) > th){
                                    if(ydiff < 0){//上方向へ動かした時
                                        score[1][0]++;
                                    }else{
                                        score[1][1]++;
                                    }
                                    // transFlg = true;
                                }
                            }
                        }

                        for(int i = 0; i < 2; i++){
                            for(int j = 0; j < 2; j++){
                                if(score[i][j] > max){
                                    max = score[i][j];
                                    maxi = i;
                                    maxj = j;
                                }
                            }
                        }
                        std::cout << maxi << " " << maxj << " " << max << std::endl;
                        // std::cout << "x positive" << std::endl;
                        // std::cout << "x positive" << std::endl;

                        if(max > 2) transFlg = true;

                        if(transFlg){
                            if(0 == maxi){
                                if(0 == maxj){
                                    // std::cout << "x positive" << std::endl;
                                    transChordX(true);
                                }else{
                                    // std::cout << "x negative" << std::endl;
                                    transChordX(false);
                                }
                            }else if(1 == maxi){
                                if(0 == maxj){
                                    // std::cout << "y positive" << std::endl;
                                    transChordY(true);
                                }else{
                                    // std::cout << "y negative" << std::endl;
                                    transChordY(false);
                                }
                            }
                        }


                        max = -1;
                        maxi = -1;
                        maxj = -1;
                        for(int i = 0; i < 2; i++){
                            for(int j = 0; j < 2; j++){
                                score[i][j] = 0;
                            }
                        }
                        transFlg = false;
                    }

                    frameBefore = frameCnt;
                }
            }
        }
    }


    void Chord::synthesizer(){
        std::cout<< "synthesizer start"<<std::endl;

        int t = 0;
        // float t_fm = 0.0;

        while(!boolSwitchON){}

        while(boolSwitchON){
            if(boolSound){


                soundData =  Vol*Amp*sin(2*M_PI*firstFreq*(double)t/sampleFreq)
                           + Vol*Amp*sin(2*M_PI*thirdFreq*(double)t/sampleFreq)
                           + Vol*Amp*sin(2*M_PI*fifthFreq*(double)t/sampleFreq);

                // t_fm = (float)t * (0.5*cos(2*M_PI*(float)t) + 0.5);
                // soundData = Vol*Amp*sin(2*M_PI*firstFreq*t_fm/sampleFreq)
                //             +Vol*Amp*sin(2*M_PI*thirdFreq*t_fm/sampleFreq)
                //             +Vol*Amp*sin(2*M_PI*fifthFreq*t_fm/sampleFreq);

                int check = write(fd, &soundData, sizeof(soundData));
                if(-1==check) die("synthesizer write");
                t++;

                //debug
                if(debug){
                    if(t%200000 == 0){
                        transChordX(true);
                    }else if(t%100000 == 0){
                        transChordY(true);
                    }
                }
            }
        }
    }

    void Chord::master(){
        if(level < 0 || level > 6) die("wrong level");

        std::thread t1(&ps::Chord::player,this);
        threadGuard tg1(t1,"player");

        std::thread t2(&ps::Chord::controller1,this);
        threadGuard tg2(t2,"controller");

        std::thread t3(&ps::Chord::synthesizer,this);
        threadGuard tg3(t3,"synthesizer");

        if(!debug){
            poseArray.readData(1);
        }
    }


}
