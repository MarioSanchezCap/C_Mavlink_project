/* Crea una zona de memoria común, escribe en ella y la borra */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "memComp.h"
#include "memComp2.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
 /* Fichero que contiene la información de la estructura de datos a


compartir */
/* Función principal */


int shmid1;
int shmid2;

int currentShmid;

TAttitude *Attitude;
TPos *Pos;

int SHM_Init(void){

    int semid = semget(0xa,0,0);

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

int SHM_InitSlot(unsigned int slot_id)
{
    if(slot_id==&shmid1){
        sem_init(&sem1,1,1);
        if((shmid1 = shmget(SHM_ID_1, sizeof(TAttitude), IPC_CREAT|0666)) == -1)
{
perror("shmget1");
exit(EXIT_FAILURE);
}
    Attitude = (TAttitude *)shmat(shmid1,0,0);
    }
    if(slot_id==&shmid2){
            sem_init(&sem2,1,1);
            if((shmid2 = shmget(SHM_ID_2, sizeof(TPos), IPC_CREAT|0666)) == -1)
{
perror("shmget2");
exit(EXIT_FAILURE);
}
    Pos = (TPos *)shmat(shmid2,0,0);
    }
    
    
}

int  SHM_ReadSlot(unsigned int slot_id, void* data, unsigned int data_size)
{

    if(slot_id==shmid1){
        sem_wait(&sem1);
        data->pitch=Attitude->pitch;
        data->roll=Attitude->roll;
        data->yaw=Attitude->yaw;
        sem_post(&sem1);
    }
    if(slot_id==shmid2){
        sem_wait(&sem2);
        data->x=Pos->pitch;
        data->y=Pos->roll;
        data->z=Pos->yaw;
        sem_post(&sem2);
    }


}
int  SHM_WriteSlot(unsigned int slot_id, void* data, unsigned int data_size)
{

    if(slot_id==shmid1){
        sem_wait(&sem1);
        Attitude->pitch=data->pitch;
        Attitude->roll=data->roll;
        Attitude->yaw=data->yaw;
        sem_post(&sem1);
    }
    if(slot_id==shmid2){
        sem_wait(&sem2);
        Pos->x=data->pitch;
        Pos->y=data->roll;
        Pos->z=data->yaw;
        sem_post(&sem2);
    }

}
