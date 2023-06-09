/*******************************************************************************
 Copyright (C) 2010  Bryan Godbolt godbolt ( a t ) ualberta.ca
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ****************************************************************************/
/*
 This program sends some data to qgroundcontrol using the mavlink protocol.  The sent packets
 cause qgroundcontrol to respond with heartbeats.  Any settings or custom commands sent from
 qgroundcontrol are printed by this program along with the heartbeats.
 
 
 I compiled this program successfully on Ubuntu 10.04 with the following command
 
 gcc -I ../../pixhawk/mavlink/include -o udp-server udp-server-test.c
 
 the rt library is needed for the clock_gettime on linux
 */
/* These headers are for QNX, but should all be standard on unix/linux */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#if (defined __QNX__) | (defined __QNXNTO__)
/* QNX specific headers */
#include <unix.h>
#else
/* Linux / MacOS POSIX timer headers */
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdbool.h> /* required for the definition of bool in C99 */
#endif

/* This assumes you have the mavlink headers on your include path
 or in the same folder as this source file */
#include <mavlink.h>
#include <math.h>
#include "memComp.h"
#include "mavlink_msg_global_position_int.h"



#define BUFFER_LENGTH 2041 // minimum buffer size that can be used with qnx (I don't know why)

uint64_t microsSinceEpoch();

typedef struct
{
float pitch;
float roll;
float yaw;
} TAttitude;

typedef struct
{
uint32_t lat;
uint32_t  longi;
float alt;
} TPos;

TPos pos = {0,0,100000};
TAttitude attitude, preattitude = {0.0, 0.0, 0.0}, attitude_speed;

uint32_t lon = 0.0;

int main(int argc, char* argv[])
{
	
	char help[] = "--help";
	
	
	char target_ip[100];
	
	float position[6] = {0, 0, 0, 0 ,0 ,0};
	mavlink_global_position_int_t packet;
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in gcAddr; 
	struct sockaddr_in locAddr;
	//struct sockaddr_in fromAddr;
	uint8_t buf[BUFFER_LENGTH];
	ssize_t recsize;
	socklen_t fromlen = sizeof(gcAddr);
	int bytes_sent;
	mavlink_message_t msg;
	uint16_t len;
	int i = 0;
	//int success = 0;
	unsigned int temp = 0;
	
    //Inicializacion mem compartida
    SHM_Init();
    SHM_InitSlot(1, sizeof(TAttitude));

	// Check if --help flag was used
	if ((argc == 2) && (strcmp(argv[1], help) == 0))
    {
		printf("\n");
		printf("\tUsage:\n\n");
		printf("\t");
		printf("%s", argv[0]);
		printf(" <ip address of QGroundControl>\n");
		printf("\tDefault for localhost: udp-server 127.0.0.1\n\n");
		exit(EXIT_FAILURE);
    }
	
	
	// Change the target ip if parameter was given
	strcpy(target_ip, "127.0.0.1");
	if (argc == 2)
    {
		strcpy(target_ip, argv[1]);
    }
	
		
	/* Attempt to make it non blocking */
#if (defined __QNX__) | (defined __QNXNTO__)
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
#else
	if (fcntl(sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0)
#endif

    {
		fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
		close(sock);
		exit(EXIT_FAILURE);
    }
	
	
	memset(&gcAddr, 0, sizeof(gcAddr));
	gcAddr.sin_family = AF_INET;
	gcAddr.sin_addr.s_addr = inet_addr(target_ip);
	gcAddr.sin_port = htons(14550);
	

	/* Global position origin */
	/*mavlink_msg_set_gps_global_origin_pack(1, 200, &msg, 1, 0, 0, 100000, microsSinceEpoch());
	len = mavlink_msg_to_send_buffer(buf, &msg);
	bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));*/

	
	for (;;) 
    {
		SHM_ReadSlot(1, &attitude, sizeof(attitude));
        attitude_speed.pitch = abs(attitude.pitch - preattitude.pitch);
        attitude_speed.roll = abs(attitude.roll - preattitude.roll);
        attitude_speed.yaw = abs(attitude.yaw - preattitude.yaw);
        printf("angulos: %f %f %f\n", attitude.pitch, attitude.roll, attitude.yaw);
        preattitude.pitch = attitude.pitch;
        preattitude.roll = attitude.roll;
        preattitude.yaw = attitude.yaw;

		/*Send Heartbeat */
		mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_FIXED_WING, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
		
		/* Send Status */
		/*mavlink_msg_sys_status_pack(1, 200, &msg, 0, 0, 0, 500, 11000, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof (struct sockaddr_in));*/
		
		/* Send Local Position */
		/*mavlink_msg_local_position_ned_pack(1, 200, &msg, microsSinceEpoch(), 
										position[0], position[1], position[2],
										position[3], position[4], position[5]);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));*/



/*

		mavlink_msg_global_position_int_pack(1, 200, &msg, microsSinceEpoch(), pos.lat, pos.longi++, pos.alt, pos.alt, 0, 0, 0, UINT16_MAX);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
*/


		packet.time_boot_ms = microsSinceEpoch();
		packet.lat = (int32_t) 0;
		packet.lon = (int32_t) ((lon++)* 1E07);
		packet.alt = (int32_t) 1000000;
		packet.relative_alt = (int32_t) (packet.lat + 10000);
		packet.vx = (int16_t) 0;
		packet.vy = (int16_t) 0;
		packet.vz = (int16_t) 0;
		packet.hdg = (uint16_t) UINT16_MAX;

		mavlink_msg_global_position_int_encode(1, 200, &msg, &packet);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));





		/* Send attitude */
		mavlink_msg_attitude_pack(1, 200, &msg, microsSinceEpoch(), attitude.roll, attitude.pitch, attitude.yaw, attitude_speed.roll, attitude_speed.pitch, attitude_speed.yaw);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
		


		memset(buf, 0, BUFFER_LENGTH);
		recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr *)&gcAddr, &fromlen);
		if (recsize > 0)
      	{
			// Something received - print out all bytes and parse packet
			mavlink_message_t msg;
			mavlink_status_t status;
			
			printf("Bytes Received: %d\nDatagram: ", (int)recsize);
			for (i = 0; i < recsize; ++i)
			{
				temp = buf[i];
				printf("%02x ", (unsigned char)temp);
				if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
				{
					// Packet received
					printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
				}
			}
			printf("\n");
		}
		memset(buf, 0, BUFFER_LENGTH);
		usleep(1000*100); // Sleep one second
    }
}


/* QNX timer version */
#if (defined __QNX__) | (defined __QNXNTO__)
uint64_t microsSinceEpoch()
{
	
	struct timespec time;
	
	uint64_t micros = 0;
	
	clock_gettime(CLOCK_REALTIME, &time);  
	micros = (uint64_t)time.tv_sec * 1000000 + time.tv_nsec/1000;
	
	return micros;
}
#else
uint64_t microsSinceEpoch()
{
	
	struct timeval tv;
	
	uint64_t micros = 0;
	
	gettimeofday(&tv, NULL);  
	micros =  ((uint64_t)tv.tv_sec) * 1000000 + tv.tv_usec;
	
	return micros;
}
#endif




/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h> 
#include "memComp.h"

typedef struct
{
float pitch;
float roll;
float yaw;
} TAttitude;


TAttitude attitude;
TAttitude at1;



int main()
{

SHM_Init();

SHM_InitSlot(1, sizeof(TAttitude));

for(;;)
{

SHM_ReadSlot(1, &attitude, sizeof(attitude));

printf("angulos: %f %f %f\n", attitude.pitch, attitude.roll, attitude.yaw);

sleep(0.01);
}

/*SHM_WriteSlot(2, &posicion, sizeof(posicion));
SHM_ReadSlot(2, &pos, sizeof(pos));

printf("posiciones: %f %f %f %s\n", pos.x, pos.y, pos.z,pos.nombre);*//*

return 0;

}*/