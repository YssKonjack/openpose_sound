#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <die.hpp>

void ps::playSound::player(){
    std::string cmd = "play -t raw -b 16 -c 1 -e s -r 44100 -";
    const char *c_cmd = cmd.c_str();//string -> char

    if (NULL == (fp = popen( c_cmd, "w"))) die("popen");
    int fd = fileno(fp);

    while(boolSwitch){}

    pclose(fp);
}

void::ps::playSound::master(){

}


void ocillator(){
    short data;





}
