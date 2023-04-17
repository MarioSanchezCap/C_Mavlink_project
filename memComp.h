/* Fichero shmz.h, contiene información sobre la zona de memoria que se pretende
compartir entre diversos procesos, será preciso incluirlo en la cabecera del fichero de
aquellos procesos que se conecten a la zona de memoria común */
/* Definición de la clave de acceso a la zona de memoria común */
#define SHM_ID_1 ((key_t) 1001)
/* Estructura de datos que se pretende compartir en la zona de memoria común */



typedef struct
{
float pitch;
float roll;
float yaw;
} TAttitude;

int id_sem1=0xA;

