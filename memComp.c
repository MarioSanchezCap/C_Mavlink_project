/* Crea una zona de memoria común, escribe en ella y la borra */

#include "memComp.h"

SHM_Slot *SlotsArray[MAX_SLOTS];
int shm_fd;
pthread_mutex_t mutex_general;
sem_t *semaforo = NULL;


int SHM_InitSlot(unsigned int slot_id, unsigned int data_size)

{

sem_t *sem;
SHM_Slot *slot;
SHM_Slot a;

if(slot_id >= 0 && slot_id <= MAX_SLOTS)
{
slot = &a;
SlotsArray[slot_id] = slot;
}
else
{
printf("No existe slot seleccionado\n");
return -1;
}

/*Inicializo semaforo y mutex del slot*/
char id[30];
sprintf(id,"/sem_slot%d",slot_id);

semaforo = sem_open(id, O_CREAT | O_EXCL, 0644, 1);

sem = sem_open(id,0);

pthread_mutex_init(&(slot->mutex),NULL); //danger!

char shm[10];
sprintf(shm,"/fd_slot%d",slot_id);

shm_fd = shm_open(shm, O_CREAT | O_RDWR, 0666 );

if (shm_fd == -1)
{
printf("Error al crear memoria compartida\n");
}

ftruncate(shm_fd, (sizeof(SHM_Slot) + data_size));

pthread_mutex_lock(&(slot->mutex));
sem_wait(sem);

slot = mmap(0,sizeof(SHM_Slot) + data_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0);
slot->payloadSize = data_size;
SlotsArray[slot_id] = slot;

pthread_mutex_unlock(&(slot->mutex));
sem_post(sem);

return 0;

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


int  SHM_ReadSlot(unsigned int slot_id, void* data, unsigned int data_size) //data_size??????????????????
{
    sem_t *sem;

    char id[30];
    sprintf(id,"/sem_slot%d",slot_id);

    sem = sem_open(id,0);

    sem_wait(sem);
    memcpy(data, SlotsArray[slot_id], data_size);
    sem_post(sem);
    
}
int  SHM_WriteSlot(unsigned int slot_id, void* data, unsigned int data_size)
{
    sem_t *sem;

    char id[30];
    sprintf(id,"/sem_slot%d",slot_id);

    sem = sem_open(id,0);

    sem_wait(sem);
    memcpy(SlotsArray[slot_id], data, data_size);
    sem_post(sem);

}
