/* Crea una zona de memoria común, escribe en ella y la borra */
#include <stdio.h>
#include <stdlib.h>
#include "memComp.h"




int SHM_Init(void){

    fd1 = shm_open("/memComp_1", O_CREAT | O_RDWR, 0666 );  //PERMISOS?????????

    if (fd1 == -1){
        printf("Error al crear memoria compartida\n");
    }

    semid1 = sem_open("sem1", O_CREAT, 0700, 0);    //PERMISOS?????????



    fd2 = shm_open("/memComp_2", O_CREAT | O_RDWR, 0666 );   //PERMISOS?????????

    if (fd2 == -1){
        printf("Error al crear memoria compartida\n");
    }

    semid2 = sem_open("sem2", O_CREAT, 0700, 0);    //PERMISOS?????????

    return 0;   //DEVUELVE?????????????????????????????????
}






/*
int SHM_InitSlot(unsigned int slot_id, unsigned int data_size)  ///??????????????????????????????????????????????
{
    
    
}

*/



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
