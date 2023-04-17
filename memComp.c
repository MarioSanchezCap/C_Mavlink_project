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

int semid1,semid2;
int currentShmid;

TAttitude *Attitude;
TPos *Pos;

void P(int semid,int sem){
    struct sembuf buf;
    buf.sem_num=sem;
    buf.sem_op=-1;
    buf.sem_flg=0;
    semop(semid,&buf,1);
}

void V(int semid,int sem){
    struct sembuf buf;
    buf.sem_num=sem;
    buf.sem_op=1;
    buf.sem_flg=0;
    semop(semid,&buf,1);
}

int SHM_Init(void){

     semid1 = semget(id_sem1,0,0);

    if((shmid1 = shmget(SHM_ID_1, sizeof(TAttitude), IPC_CREAT|0666)) == -1)
{
perror("shmget1");
exit(EXIT_FAILURE);
}
    Attitude = (TAttitude *)shmat(shmid1,0,0);

    semid2 = semget(id_sem2,0,0);
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
        semid1 = semget(id_sem1,0,0);
        if((shmid1 = shmget(SHM_ID_1, sizeof(TAttitude), IPC_CREAT|0666)) == -1)
{
perror("shmget1");
exit(EXIT_FAILURE);
}
    Attitude = (TAttitude *)shmat(shmid1,0,0);
    }
    if(slot_id==&shmid2){
            semid2 = semget(id_sem2,0,0);
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
        P(semid1,1);
        data->pitch=Attitude->pitch;
        data->roll=Attitude->roll;
        data->yaw=Attitude->yaw;
        V(semid1,0);
    }
    if(slot_id==shmid2){
        P(semid2,1);
        data->x=Pos->x;
        data->y=Pos->y;
        data->z=Pos->z;
        sem_post(&sem2);
        V(semid2,0);
    }


}
int  SHM_WriteSlot(unsigned int slot_id, void* data, unsigned int data_size)
{

    if(slot_id==shmid1){
        P(semid1,1);
        Attitude->pitch=data->pitch;
        Attitude->roll=data->roll;
        Attitude->yaw=data->yaw;
        V(semid1,0);
    }
    if(slot_id==shmid2){
        P(semid2,1);
        Pos->x=data->x;
        Pos->y=data->y;
        Pos->z=data->z;
        V(semid2,0);
    }

}
