#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <die.hpp>

void oci::Ocillator::player(){
    std::string cmd = "play -t raw -b 16 -c 1 -e s -r 44100 -";
    const char *cmd_c = cmd.c_str();//string -> char

    if (NULL == (fp = popen( cmd_C, "w"))) die("popen");
    int fd = fileno(fp);

    while(finFlag){}

    pclose(fp);
}

void ocillator(){
    short data;

    



}
