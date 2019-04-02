#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define communication_port     9877
#define max_file_name          512
#define max_buffer_size        512

int main(int argc,char *argv[])
{	
	int file_block_length,offset = 0;
	char command[10];
	char message[max_buffer_size],file_name[max_file_name];
	FILE *fp;
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(communication_port);
	
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0)
	{
		printf(" Create server_socket Failed \n");
		exit(1);
	}
	if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(sockaddr_in)) < 0)
	{
		printf("server bind port Failed \n");
		exit(1);
	}
	if(listen(server_socket,10))
	{
		printf("server listen Failed \n");
		exit(1);	
	}

	struct sockaddr_in client_addr;
	socklen_t          length = sizeof(client_addr);


	int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);	
	
	while(1)
	{
		if(recv(new_server_socket,command,10,0) < 0)
		{
			printf("Receive command failed\n");
		}else{
			printf("Receive command:%s \n",command);
		}
		
		if(strcmp(command,"get") == 0)
		{
			printf("get data from server\n");
			recv(new_server_socket,file_name,sizeof(file_name),0);
			printf("接收到的文件名 :  %s \n",file_name);
			fp = fopen(file_name, "r");  
			if (fp == NULL)  
			{  
				printf("File:\t%s Can Not Open To Write!\n", file_name);  
				break;  
			}else{
				while((file_block_length = fread(message,sizeof(char),max_buffer_size,fp)) > 0)
					{
						if(offset == 0)
						{
							offset = file_block_length;
						}else{
							offset += file_block_length;
						}
						fseek(fp,(offset) * 1L,0);
						send(new_server_socket,message,sizeof(message),0);
						bzero(message, max_buffer_size);
					}
					printf("1111111\n");
					offset = 0;
				}
		}else if(strcmp(command,"input") == 0)
		{
			printf("input data from client\n");
			recv(new_server_socket,file_name,sizeof(file_name),0);
			printf("接收到的文件名 :  %s \n",file_name);
			recv(new_server_socket,message,sizeof(message),0);
				fp = fopen(file_name, "w");
				fseek(fp,0L,2);
				fwrite(message,sizeof(char),strlen(message),fp);		
		}else if(strcmp(command,"close") == 0){
			printf("close socket connection \n");
			break;
		}
	}
	close(new_server_socket);
	exit(0);
}



