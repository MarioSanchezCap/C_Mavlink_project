/* Fichero shmz.h, contiene información sobre la zona de memoria que se pretende
compartir entre diversos procesos, será preciso incluirlo en la cabecera del fichero de
aquellos procesos que se conecten a la zona de memoria común */
/* Definición de la clave de acceso a la zona de memoria común */
#define SHM_ID_2 ((key_t) 1001)
/* Estructura de datos que se pretende compartir en la zona de memoria común */



typedef struct
{
float x;
float y;
float z;
} TPos;

