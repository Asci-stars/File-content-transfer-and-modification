#include <netinet/in.h>                         // for sockaddr_in  
#include <sys/types.h>                          // for socket  
#include <sys/socket.h>                         // for socket  
#include <stdio.h>                              // for printf  
#include <stdlib.h>                             // for exit  
#include <string.h>                             // for bzero 
#include <arpa/inet.h>
#include <unistd.h>

#define communication_port     9877
#define max_file_name          512
#define max_buffer_size        512
int main(int argc,char *argv[])
{
	char command[10],file_name[max_file_name];
	char Buffer[max_buffer_size];
	char * find;
	FILE *fp;
	if(argc != 2)
	{
		printf("Client:input parameters error,parameters must == 2 \n");
		exit(1);
	}
 
	struct sockaddr_in client_addr;
	bzero(&client_addr,sizeof(client_addr));
	client_addr.sin_family = AF_INET; 
	client_addr.sin_addr.s_addr = htons(INADDR_ANY); 
    client_addr.sin_port = htons(0); 
 
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);  
    if (client_socket < 0)  
    {  
        printf("Client: Create Socket Failed!\n");  
        exit(1);  
    } 
 
    struct sockaddr_in  server_addr;  
    bzero(&server_addr, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;   

	//inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    if (inet_pton(AF_INET,argv[1], &server_addr.sin_addr) == 0)  
    {  
        printf("Server IP Address Error!\n");  
        exit(1);  
    }
	server_addr.sin_port = htons(communication_port);
   if(connect(client_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
	{
		printf("Can not Connect to %s \n",argv[1]);
		exit(1);
	}	
	else
	{
		printf("socket 连接成功.\n");
	}
	while(1)
	{
		printf("请输入命名get、input、or close ：");
		scanf("%s",&command);
		if(strcmp(command,"close") == 0)
		{
			break;	//终止连接
		}
		if(send(client_socket,command,sizeof(command),0) < 0)
		{
			printf("Send Data to client %s Failed\n",command);
		}else{
			printf("发送的命名 :  %s \n",command);
		}
		printf("请输入文件名：");
		scanf("%s",&file_name)	;
		if(send(client_socket,file_name,sizeof(file_name),0) < 0)	//发送文件名
		{
			printf("Send Data to client %s Failed\n",file_name);
		}else{
			printf("发送的文件名 :  %s \n",file_name);
			fgets(Buffer,4096,stdin);
			send(client_socket,Buffer,sizeof(Buffer),0);
		}
		if(strcmp(command,"get") == 0)
		{
			//fp = fopen("123.txt","rw+");
			while(recv(client_socket,Buffer,sizeof(Buffer),0))
			{
				printf("接收文件:%s 内容:%s \n",file_name,Buffer);
				//fwrite(Buffer,sizeof(char),sizeof(Buffer),fp);
			}
		}
		if(strcmp(command,"input") == 0)
		{
			fgets(Buffer,4096,stdin);
			send(client_socket,Buffer,sizeof(Buffer),0);
		}
	}
	close(client_socket);
	exit(0);
}








