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

/*
TPos posicion;
TPos pos;
*/
int main()
{

SHM_Init();


/*
posicion.x = 99999.0;
posicion.y = 48.0;
posicion.z = 316.84;
memcpy(posicion.nombre ,"Tomi",5);
*/
SHM_InitSlot(1, sizeof(TAttitude));
//SHM_InitSlot(2, sizeof(TPos));

for(;;)
{

attitude.pitch = sin(rand()%6 - 3.14);
attitude.roll = sin(rand()%6 - 3.14);
attitude.yaw = sin(rand()%6 - 3.14);

SHM_WriteSlot(1, &attitude, sizeof(attitude));


//printf("angulos: %f %f %f\n", attitude.pitch, attitude.roll, attitude.yaw);

sleep(0.01);
}

/*SHM_WriteSlot(2, &posicion, sizeof(posicion));
SHM_ReadSlot(2, &pos, sizeof(pos));

printf("posiciones: %f %f %f %s\n", pos.x, pos.y, pos.z,pos.nombre);*/

return 0;

}