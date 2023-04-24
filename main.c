#include <stdio.h>

#include "memComp.h"





typedef struct

{

float pitch;

float roll;

float yaw;

} TAttitude;




TAttitude dato;





int main(){




SHM_Init();




SHM_InitSlot(1, sizeof(TAttitude));




/*SHM_WriteSlot(1, &attitude, sizeof(attitude));

SHM_WriteSlot(2, &posicion, sizeof(posicion));

SHM_ReadSlot(1, &at1, sizeof(at1));

SHM_ReadSlot(2, &pos1, sizeof(pos1));

*/

//printf("angulos: %f %f %f\n", at1.pitch, at1.roll, at1.yaw);

//printf("posicion: %f %f %f\n", pos1.x, pos1.y, pos1.z);

return 0;




}