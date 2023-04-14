//Example: Usage of the library where the following functions exist:

int LNX_SHM_EXPORT SHM_Init(void);
int LNX_SHM_EXPORT SHM_InitSlot(unsigned int slot_id, unsigned int data_size);
int LNX_SHM_EXPORT SHM_ReadSlot(unsigned int slot_id, void* data, unsigned int data_size);
int LNX_SHM_EXPORT SHM_WriteSlot(unsigned int slot_id, void* data, unsigned int data_size);
int LNX_SHM_EXPORT SHM_InitSlot(unsigned int slot_id, unsigned int data_size);

///proceso A, productor

struct
{
float pitch;
float roll;
float yaw;
} TAttitude;

void main()
{
TAttitude attitude;
float angle = -pi;

SHM_Init(void);
SHM_InitSlot(SHM_ID_1,sizeof(count));
	while(1)
	{
		delay(10ms);
		angle+=(pi/10);
		attitude.pitch=angle;
		if(angle >=pi)
		{
			angle = -pi;
		}
		SHM_WriteSlot(SHM_ID_1,&attitude,sizeof(TAttitude));	
	}

}

///proceso B, consumidor
void main()
{
float pitch;

SHM_Init(void);
SHM_InitSlot(SHM_ID_1,sizeof(count));
	while(1)
	{
		delay(10ms);
		SHM_ReadSlot(SHM_ID_1,&pitch,sizeof(pitch));
		msg= crearMSGMavlink(pitch);
		send(UDP,	msg);
	}

}	
