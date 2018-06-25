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
        // std::cout << "sox start" << std::endl;

        while(boolSwitchON){}

        pclose(fp);
    }

    void playSound::playSine(){
        short data;

        while(!boolSwitchON){}

        for(int t = 0; t < 50000; t++){
            data = Vol*sin(2*M_PI*442.0*(double)t/sampleFreq);
            int check = write(fd, &data, sizeof(data)); if(-1==check) die("write");
        }

        stop();
    }

    void playSound::master(){
        std::thread t1(&playSound::player,this);
        threadGuard tg1(t1,"player");

        std::thread t2(&playSound::playSine,this);
        threadGuard tg2(t2,"sine");

    }
}


int main(){
    ps::playSound player;
    player.master();
}
