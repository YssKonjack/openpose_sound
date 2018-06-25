#ifndef PS_DIE_H
#define PS_DIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

void die(std::string str){
    const char *s = str.c_str();
    perror(s);
    exit(1);
}

#endif
