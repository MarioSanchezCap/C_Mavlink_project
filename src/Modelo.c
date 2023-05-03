#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "memComp.h"

typedef struct
{
    float pitch;
    float roll;
    float yaw;
} TAttitude;

TAttitude attitude;

float alpha = 0;
float delta = (2*3.14*0.1)/10;


int main()
{

SHM_Init();
SHM_InitSlot(1, sizeof(TAttitude));

for(;;)
{

    usleep(1000*100);
    alpha = alpha + delta;

    attitude.pitch = (3.14/4) * sin(alpha);
    attitude.roll = 0;
    attitude.yaw = 0;

    SHM_WriteSlot(1, &attitude, sizeof(attitude));

}

return 0;

}