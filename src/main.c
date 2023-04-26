#include <stdio.h>
#include <string.h>
#include "memComp.h"

typedef struct
{
float pitch;
float roll;
float yaw;
} TAttitude;

typedef struct
{
float x;
float y;
float z;
char nombre[6];
} TPos;

TAttitude attitude;
TAttitude at1;

TPos posicion;
TPos pos;

int main()
{

SHM_Init();

attitude.pitch = 1.0;
attitude.roll = 2.0;
attitude.yaw = 3.0;

posicion.x = 99999.0;
posicion.y = 48.0;
posicion.z = 316.84;
memcpy(posicion.nombre ,"Tomi",5);

SHM_InitSlot(1, sizeof(TAttitude));
SHM_InitSlot(2, sizeof(TPos));

SHM_WriteSlot(1, &attitude, sizeof(attitude));
SHM_ReadSlot(1, &at1, sizeof(at1));

printf("angulos: %f %f %f\n", at1.pitch, at1.roll, at1.yaw);

SHM_WriteSlot(2, &posicion, sizeof(posicion));
SHM_ReadSlot(2, &pos, sizeof(pos));

printf("posiciones: %f %f %f %s\n", pos.x, pos.y, pos.z,pos.nombre);

return 0;

}