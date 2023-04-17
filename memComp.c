/* Crea una zona de memoria común, escribe en ella y la borra */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "memComp.h"
#include "memComp2.h" /* Fichero que contiene la información de la estructura de datos a


compartir */
/* Función principal */



#include <semaphore.h>



int shmid1;
int shmid2;

int currentShmid;

TAttitude *Attitude;
TPos *Pos;

TAttitude Attitude1;
TPos Pos1;





int SHM_Init(void){
    if((shmid1 = shmget(SHM_ID_1, sizeof(TAttitude), IPC_CREAT|0666)) == -1)
{
perror("shmget1");
exit(EXIT_FAILURE);
}
    Attitude = (TAttitude *)shmat(shmid1,0,0);
    if((shmid2 = shmget(SHM_ID_2, sizeof(TPos), IPC_CREAT|0666)) == -1)
{
perror("shmget2");
exit(EXIT_FAILURE);
}
    Pos = (TPos *)shmat(shmid2,0,0);
}

int SHM_InitSlot(unsigned int slot_id){
    if(slot_id==&shmid1){
        if((shmid1 = shmget(SHM_ID_1, sizeof(TAttitude), IPC_CREAT|0666)) == -1)
{
perror("shmget1");
exit(EXIT_FAILURE);
}
    Attitude = (TAttitude *)shmat(shmid1,0,0);
    }
    if(slot_id==&shmid2){
            if((shmid2 = shmget(SHM_ID_2, sizeof(TPos), IPC_CREAT|0666)) == -1)
{
perror("shmget2");
exit(EXIT_FAILURE);
}
    Pos = (TPos *)shmat(shmid2,0,0);
    }
    
    
 
}

int  SHM_ReadSlot(unsigned int slot_id, void* data, unsigned int data_size){
    

    if(slot_id==shmid1){
        Attitude1.pitch;=Attitude->pitch;
        Attitude1.roll=Attitude->roll;
        Attitude1.yaw=Attitude->yaw;
    }
    if(slot_id==shmid2){
    }

    data = shmat(slot_id,0,0);

}
int  SHM_WriteSlot(unsigned int slot_id, void* data, unsigned int data_size){

}

int main(void)
{

TAttitude *zona;
/* Creación de la zona de memoria compartida */

/* Obtención del puntero a la estructura de datos compartida */
zona = (TAttitude *)shmat(shmid1,0,0);
/* Escribimos en la zona de memoria común */
printf("TAREA es: %f \n", zona->pitch);
printf("VALOR es: %f \n", zona->roll);
zona->pitch = 5;
zona->roll = 16;
/* Leemos los valores de las variables de la zona de memoria común */
printf("TAREA es: %d \n", zona->pitch);
printf("VALOR es: %f \n", zona->roll);
/* Borrado de la zona de memoria compartida */
//shmctl(shmid, IPC_RMID,0);
}
/* Fin función principal */