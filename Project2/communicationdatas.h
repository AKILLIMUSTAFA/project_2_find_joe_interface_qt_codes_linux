#ifndef COMMUNICATIONDATAS_H
#define COMMUNICATIONDATAS_H

#include <stdint.h>

#define EMPTY 0
#define PLAY 1
#define PAUSE 2
#define STOP 3
#define PATH 4

struct HWtoUI{
    float x;
    float y;
    bool found;
    int32_t rotation;
};

struct UItoHW{
    int32_t mode;
};

#endif // COMMUNICATIONDATAS_H
