/* Crea una zona de memoria común, escribe en ella y la borra */
#include "memComp.h"

SHM_Slot *SlotsArray[MAX_SLOTS];

int shm_fd;
pthread_mutex_t mutex_general;


int SHM_InitSlot(unsigned int slot_id, unsigned int data_size) 
{
    SHM_Slot *slot;
    SHM_Slot a;

    if(slot_id >= 0 && slot_id < MAX_SLOTS){
        if(SlotsArray[slot_id]!=NULL){
            slot = SlotsArray[slot_id];
        }else{
            slot=&a;
            SlotsArray[slot_id]=slot;
        }
    }else{
        printf("No existe slot seleccionado\n");
        return -1;
    }


    /*Inicializo semaforo y mutex del slot*/
    char id[3];
    sprintf(id,"%d",slot_id);
    slot->semaforo = sem_open(strcat("/dev/shm/sem_slot",id), O_CREAT , S_IRWXU, 0);
    pthread_mutex_init(&(slot->mutex),NULL); //danger!----



    shm_fd = shm_open(strcat("fd_slot",id), O_CREAT | O_RDWR, 0666 ); 

    if (shm_fd == -1){
        printf("Error al crear memoria compartida\n");
    }

    ftruncate(shm_fd, (sizeof(SHM_Slot) + data_size));

    Lock(slot);

    slot = mmap(0,sizeof(SHM_Slot) + data_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0);
    slot->payloadSize = data_size;
    SlotsArray[slot_id] = slot;

    Unlock(slot);

}


/* Inicializo a NULL todos mis Slots*/
int SHM_Init(void)
{
    pthread_mutex_init(&mutex_general,NULL);
    
    pthread_mutex_lock(&mutex_general);

    for(int i = 0;i < MAX_SLOTS - 1;i++)
    {
        SlotsArray[i] = NULL;
    }

    pthread_mutex_unlock(&mutex_general);
}

/*

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
*/

// Lock mutex y semaforo del Slot *//*
void Lock(SHM_Slot *slot){
    pthread_mutex_lock(&(slot->mutex));
    sem_wait(slot->semaforo);
}

 //Unlock mutex y semaforo del Slot
void Unlock(SHM_Slot *slot){
    pthread_mutex_unlock(&(slot->mutex));
    sem_post(slot->semaforo);
}
