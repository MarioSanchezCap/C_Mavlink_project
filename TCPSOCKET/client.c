#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

short socketCreate(void){
	short hSocket;
	printf("Create the socket\n");
	hSocket=socket(AF_INET,SOCK_STREAM,0);
	return hSocket;
}
int socketConnect(int hSocket){
	int iRetval=-1;
	int serverPort=12345;
	struct sockaddr_in remote={0};
	
	remote.sin_family=AF_INET;
	remote.sin_addr.s_addr=inet_addr("127.0.0.1");
	remote.sin_port=htons(serverPort);
	
	iRetval=connect(hSocket,(struct sockaddr *) &remote,sizeof(remote));
	return iRetval;
}
int socketSend(int hSocket,char* Rqst,short lenRqst){
	int shortRetval=-1;
	struct timeval tv;
	tv.tv_sec=20;
	tv.tv_usec=0;
	
	if(setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char*) &tv,sizeof(tv))<0){
		printf("Time out\n");
		return -1;
	}
	shortRetval=send(hSocket,Rqst,lenRqst,0);
	return shortRetval;
}
int socketRecieve(int hSocket,char* Rsp,short RvcSize){
	int shortRetval=-1;
	struct timeval tv;
	tv.tv_sec=20;
	tv.tv_usec=0;

	if(setsockopt(hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*) &tv,sizeof(tv))<0){
		printf("Time out\n");
		return -1;
	}
	shortRetval=recv(hSocket,Rsp,RvcSize,0);
	printf("Response %s\n",Rsp);
	
	return shortRetval;
}

int main(int argc, char *argv[]){
	int hSocket=0,read_size=0;
	struct sockaddr_in server;
	char sendToServer[100]={0};
	char server_reply[200]={0};
	
	hSocket=socketCreate();
	
	if(hSocket==-1){
		printf("Could not create socket\n");
		return 1;
	}
	
	printf("Socket created");
	if(socketConnect(hSocket)<0){
		perror("Connect failed\n");
		return 1;
	}
	printf("Successfully connected to server");
	printf("Enter the message: ");
	fgets(sendToServer,100,stdin);
	
	socketSend(hSocket,sendToServer,strlen(sendToServer));
	
	read_size=socketRecieve(hSocket,server_reply,200);
	printf("Server response: %s\n\n",server_reply);
	
	close(hSocket);
	return 0;
}
	









