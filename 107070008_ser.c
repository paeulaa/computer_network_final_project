#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <unistd.h>	
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

//#define PORT 8000

int main(int argc, char *argv[])
{
	struct sockaddr_in ser_addr, cli_addr;
	int ser_socket, cli_socket;
	int ser_addr_len = sizeof(ser_addr);
    int cli_addr_len = sizeof(cli_addr);
    int PORT;
    
    if(argc == 2){
        int PORT = atoi(argv[1]);
    }

 	bzero(&ser_addr, ser_addr_len);
	ser_addr.sin_family = AF_INET;
  	ser_addr.sin_port = htons(PORT);
	ser_addr.sin_addr.s_addr = INADDR_ANY;

	ser_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(ser_socket < 0){
		printf("Error creating socket\n");
		exit(0);
	}

	if(bind(ser_socket,(struct sockaddr *) &ser_addr, ser_addr_len) == -1){
		printf("Error binding\n");
		close(ser_socket);
		exit(0);
	}
	
	if(listen(ser_socket, 1) == -1){
		printf("Error listening\n");
		close(ser_socket);
		exit(0);
	}

	printf("Waiting to the client!\n");
	if((cli_socket = accept(ser_socket, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_addr_len)) == -1){
		printf("accept failed\n");
		close(ser_socket);
		exit(0);
	}
	printf("Client connect successfully\n");	

	int bytesRecv, bytesSend;
    char send_buf[500], recv_buf[500], messagebox[500];
    messagebox[0] = '\0';
    char *menu = "\
\n\n------------------Menu------------------\n\
1. Read all the messages.\n\
2. Write new messages.\n\
Please enter 1 or 2 : \0";

	char *messagelist = "\
\n\n----------------messages----------------\n\
All Messages:\n\
First line here.\0";

	char *newitem = "\
\n\n----------------------------------------\n\
Type anything you want:\0";

	char *errorlist = "\
\n\n--------------Warning!----------------\n\
you can only enter operation 1 or 2 !\0";

	char *changeline = "\n";

	send_buf[0] = '\0';
	strcat(send_buf, menu);
	bytesSend = send(cli_socket, send_buf, sizeof(send_buf), 0);
	if(bytesSend < 0) printf("Error sending packet\n");

  	while(1){		
		bytesRecv = recv(cli_socket, recv_buf, sizeof(recv_buf), 0);
		if(bytesRecv < 0) printf("Error receiving packet\n");
		
		printf("%s\n", recv_buf);

		if(!strncmp(recv_buf, "1", 1)){
			send_buf[0] = '\0';
			strcat(send_buf, messagelist);
			if(messagebox[0]!='\0') strcat(send_buf, messagebox);
			strcat(send_buf, menu);
			bytesSend = send(cli_socket, send_buf, sizeof(send_buf), 0);
			if(bytesSend < 0) printf("Error sending packet\n");
		}
		else if(!strncmp(recv_buf, "2", 1)){
			send_buf[0] = '\0';
			strcat(send_buf, newitem);
			bytesSend = send(cli_socket, send_buf, sizeof(send_buf), 0);
			if(bytesSend < 0) printf("Error sending packet\n");

			bytesRecv = recv(cli_socket, recv_buf, sizeof(recv_buf), 0);
			if(bytesRecv < 0) printf("Error receiving packet\n");
			printf("%s\n", recv_buf);
			strcat(messagebox, changeline);
			strcat(messagebox, recv_buf);
			
			send_buf[0] = '\0';
			strcat(send_buf, menu);
			bytesSend = send(cli_socket, send_buf, sizeof(send_buf), 0);
			if(bytesSend < 0) printf("Error sending packet\n");

		}
		else{
			
			bytesSend = send(cli_socket, menu, strlen(menu), 0);
			if(bytesSend < 0) printf("Error sending packet\n");	
		}
	}
        
	return 0;
}



