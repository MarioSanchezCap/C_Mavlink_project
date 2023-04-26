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
TAttitude at1;

/*
TPos posicion;
TPos pos;
*/
int main()
{

SHM_Init();

SHM_InitSlot(1, sizeof(TAttitude));

for(;;)
{

SHM_ReadSlot(1, &attitude, sizeof(attitude));

printf("angulos: %f %f %f\n", attitude.pitch, attitude.roll, attitude.yaw);

sleep(0.01);
}

/*SHM_WriteSlot(2, &posicion, sizeof(posicion));
SHM_ReadSlot(2, &pos, sizeof(pos));

printf("posiciones: %f %f %f %s\n", pos.x, pos.y, pos.z,pos.nombre);*/

return 0;

}