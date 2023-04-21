/* Crea una zona de memoria común, escribe en ella y la borra */
#include <stdio.h>
#include <stdlib.h>
#include "memComp.h"
#include <pthread.h>

#define MAX 100

typedef struct
{
    int id;
    pthread_mutex_t mutex;
    void* dato;
    sem_t *semaforo;

}SHM_Slot;

SHM_Slot *SlotsArray;

int shm_fd;

pthread_mutex_t mutex_general;


int SHM_InitSlot(unsigned int slot_id, unsigned int data_size)  ///??????????????????????????????????????????????
{
    SHM_Slot *slot;

    int existe_InitSlot = -1;

    for(int i=0;i<sizeof(SlotsArray) - 1;i++)
    {
        if(SlotsArray[i]->id == slot_id)
        {
            //Existes
            existe_InitSlot = i;
        } 
    }

    if(existe_InitSlot != -1) //cuando existe
    {
        slot = SlotsArray[existe_InitSlot];
    }
    else //cuando no existe
    {
        pthread_mutex_lock(&mutex_general);

        SlotsArray[sizeof(SlotsArray)] = malloc(sizeof(SHM_Slot)+data_size);
        if(slot == NULL){
            printf("No hay memoria disponible");
            exit(1);
        }

        slot = SlotsArray[sizeof(SlotsArray)];
        slot->id = slot_id;

        slot->semaforo = sem_open(strcat("sem_slot",slot_id), O_CREAT , S_IRWXU, 0);

        pthread_mutex_init(&(slot->mutex),NULL); //danger!

        pthread_mutex_unlock(&mutex_general);
    }
    
    shm_fd = shm_open(strcat("/dev/shm/fd_slot",slot_id), O_CREAT | O_RDWR, 0666 ); 

    if (shm_fd == -1){
        printf("Error al crear memoria compartida\n");
    }

    slot = mmap(0,sizeof(SHM_Slot) + data_size, PROT_READ | PROT_WRITE, shm_fd,0);

}


int SHM_Init(void)
{

    pthread_mutex_init(&mutex_general,NULL);

    SHM_Slot *slot;

    for(int i=0;i<MAX - 1;i++)
    {
        dato->SlotsArray[i] = 0;
        
        slot = SlotsArray[i];

        slot->id = i;

        slot->semaforo = sem_open(strcat("sem_slot",slot_id), O_CREAT , S_IRWXU, 0);

        pthread_mutex_init(&(slot->mutex),NULL); //danger!
    }
    
    
    pthread_mutex_lock(&mutex_general);

    SlotsArray[sizeof(SlotsArray)] = malloc(sizeof(SHM_Slot)+data_size);
    if(slot == NULL){
        printf("No hay memoria disponible");
        exit(1);
    }

    slot = SlotsArray[sizeof(SlotsArray)];
    slot->id = slot_id;

    slot->semaforo = sem_open(strcat("sem_slot",slot_id), O_CREAT , S_IRWXU, 0);

    pthread_mutex_init(&(slot->mutex),NULL); //danger!

    pthread_mutex_unlock(&mutex_general);
    
    
    shm_fd = shm_open(strcat("/dev/shm/fd_slot",slot_id), O_CREAT | O_RDWR, 0666 ); 

    if (shm_fd == -1){
        printf("Error al crear memoria compartida\n");
    }

    slot = mmap(0,sizeof(SHM_Slot) + data_size, PROT_READ | PROT_WRITE, shm_fd,0);

}


int  SHM_ReadSlot(unsigned int slot_id, void* data, unsigned int data_size) //data_size??????????????????
{

    if(slot_id==1){

        sem_wait(semid1);
        
        if(fstat(fd1, &stat1) == -1){
            printf("Error file descriptor %s\n", strerror(errno));
            exit(1);
        }

        ptr1 = mmap(0, stat1.st_size, PROT_READ, MAP_SHARED, fd1, 0);
        if(ptr1 == MAP_FAILED){
            printf("Error en mapeado de memoria\n");
        } else {
            ((TAttitude *) data)->pitch = ptr1->pitch;  ///CUIDAOOOOOO!!!!!!!!!
            ((TAttitude *) data)->roll = ptr1->roll;
            ((TAttitude *) data)->yaw = ptr1->yaw;
        }

        sem_close(semid1);
    }

    if(slot_id==2){

        sem_wait(semid2);

        if(fstat(fd2, &stat1) == -1){
            printf("Error file descriptor %s\n", strerror(errno));
            exit(1);
        }

        ptr2 = mmap(0, stat1.st_size, PROT_READ, MAP_SHARED, fd2, 0);
        if(ptr2 == MAP_FAILED){
            printf("Error en mapeado de memoria\n");
        } else {
            ((TPos *) data)->x = ptr2->x;  ///CUIDAOOOOOO!!!!!!!!!
            ((TPos *) data)->y = ptr2->y;
            ((TPos *) data)->z = ptr2->z;
        }

        sem_close(semid2);
    }

    return 0;   //DEVUELVE?????????????????????????????????
}




int  SHM_WriteSlot(unsigned int slot_id, void* data, unsigned int data_size)
{

    if(slot_id==1){

        sem_wait(semid1);

        if(fstat(fd1, &stat1) == -1){
            printf("Error file descriptor %s\n", strerror(errno));
            exit(1);
        }

        ftruncate(fd1, data_size);
        ptr1 = mmap(0, data_size, PROT_WRITE, MAP_SHARED, fd1, 0);
        if(ptr1 == MAP_FAILED){
            printf("Error en mapeado de memoria\n");
        } else {
           
            ptr1->pitch = (float)((TAttitude *) data)->pitch;
            ptr1->roll = (float)((TAttitude *) data)->roll;
            ptr1->yaw = (float)((TAttitude *) data)->yaw;
        }

        sem_close(semid1);
    }

    if(slot_id==2){

        sem_wait(semid2);

        if(fstat(fd2, &stat2) == -1){
            printf("Error file descriptor %s\n", strerror(errno));
            exit(1);
        }

        ftruncate(fd2, data_size);
        ptr2 = mmap(0, data_size, PROT_WRITE, MAP_SHARED, fd2, 0);
        if(ptr2 == MAP_FAILED){
            printf("Error en mapeado de memoria\n");
        } else {
           
            ptr2->x = (float)((TPos *) data)->x;
            ptr2->y = (float)((TPos *) data)->y;
            ptr2->z = (float)((TPos *) data)->z;
        }

        sem_close(semid2);
    }

    return 0;   //DEVUELVE?????????????????????????????????
}
