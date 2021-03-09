#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#define PORT 8000

int main(int argc, char *argv[])
{
	struct sockaddr_in ser_addr;
	int ser_socket;
	int ser_addr_len = sizeof(ser_addr);
    int PORT;
    char ser_ip[20];
    
    if(argc == 3){
        strcpy(ser_iP, argv[1]);
        PORT = atoi(argv[2]);
    }
    
	bzero(&ser_addr, ser_addr_len);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	ser_addr.sin_addr.s_addr = inet_addr(ser_ip);

	ser_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(ser_socket < 0){
		printf("Error creating socket\n");
		exit(0);
	}	

	if(connect(ser_socket, (struct sockaddr *)&ser_addr, ser_addr_len) == -1){
		printf("connect failed\n");
		close(ser_socket);
		exit(0);
	}

	int bytesSend, bytesRecv;
	char send_buf[500], recv_buf[500];	

	while(1){
		bytesRecv = recv(ser_socket, recv_buf, sizeof(recv_buf), 0);
		if(bytesRecv < 0) printf("Error recving packet\n");
		printf("%s\n", recv_buf);

		fflush(stdin);
		scanf(" %[^\n]", send_buf);
		
		bytesSend = send(ser_socket, send_buf, sizeof(send_buf), 0);
		if(bytesSend < 0) printf("Error sending packet\n");
		
	}

	return 0;
}
