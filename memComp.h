#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <semaphore.h>


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
} TPos;

int fd1, fd2;

TAttitude *ptr1;
TPos *ptr2;
struct stat stat1, stat2;

/*Semaforos*/
sem_t *semid1, *semid2;


/*Funciones*/
int  SHM_Init(void);
//int  SHM_InitSlot(unsigned int slot_id, unsigned int data_size);
int  SHM_ReadSlot(unsigned int slot_id, void* data, unsigned int data_size);
int  SHM_WriteSlot(unsigned int slot_id, void* data, unsigned int data_size);


