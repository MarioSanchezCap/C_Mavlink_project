/*#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <semaphore.h>
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_SLOTS 300

typedef struct
{
    unsigned int payloadSize;
    pthread_mutex_t mutex;
    //void* dato;
    sem_t *semaforo;

}SHM_Slot;


/*Funciones*/
int  SHM_Init(void);
int  SHM_InitSlot(unsigned int slot_id, unsigned int data_size);
//int  SHM_ReadSlot(unsigned int slot_id, void* data, unsigned int data_size);
//int  SHM_WriteSlot(unsigned int slot_id, void* data, unsigned int data_size);

void Lock(SHM_Slot *slot);
void Unlock(SHM_Slot *slot);


