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
	char Buffer[max_buffer_size];
	if(argc != 2)
	{
		printf("Client:input parameters error,parameters must == 2 \n");
		exit(1);
	}
	    // ÉèÖÃÒ»¸ösocketµØÖ·½á¹¹client_addr, ´ú±í¿Í»§»úµÄinternetµØÖ·ºÍ¶Ë¿Ú  
	struct sockaddr_in client_addr;
	bzero(&client_addr,sizeof(client_addr));
	client_addr.sin_family = AF_INET; // internetÐ­Òé×å  
	client_addr.sin_addr.s_addr = htons(INADDR_ANY); // INADDR_ANY±íÊ¾±¾»úµÄËùÓÐIP
    client_addr.sin_port = htons(0); // auto allocated, ÈÃÏµÍ³×Ô¶¯·ÖÅäÒ»¸ö¿ÕÏÐ¶Ë?
    // ´´½¨ÓÃÓÚinternetµÄÁ÷Ð­Òé(TCP)ÀàÐÍsocket£¬ÓÃclient_socket´ú±í¿Í»§¶Ësocket  
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);  
    if (client_socket < 0)  
    {  
        printf("Client: Create Socket Failed!\n");  
        exit(1);  
    } 
    // ÉèÖÃÒ»¸ösocketµØÖ·½á¹¹server_addr,´ú±í·þÎñÆ÷µÄinternetµØÖ·ºÍ¶Ë¿Ú  
    struct sockaddr_in  server_addr;  
    bzero(&server_addr, sizeof(server_addr));  
    server_addr.sin_family = AF_INET; 
    // ·þÎñÆ÷µÄIPµØÖ·À´×Ô³ÌÐòµÄ²ÎÊý   

	//inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    if (inet_pton(AF_INET,argv[1], &server_addr.sin_addr) == 0)  
    {  
        printf("Server IP Address Error!\n");  
        exit(1);  
    }
	server_addr.sin_port = htons(communication_port);
   // Ïò·þÎñÆ÷·¢ÆðÁ¬½ÓÇëÇó£¬Á¬½Ó³É¹¦ºóclient_socket´ú±í¿Í»§¶ËºÍ·þÎñÆ÷¶ËµÄÒ»¸ösocketÁ¬½Ó  
   if(connect(client_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
	{
		printf("Can not Connect to %s \n",argv[1]);
		exit(1);
	}	
	else
	{
		printf("socket 连接成功.\n");
	}
	if(recv(client_socket,Buffer,max_buffer_size,0) < 0)
	{
		printf("Receive Data from server %s Failed\n",argv[1]);
	}else{
		printf("接收到的数据 : %s \n",Buffer);
	}
	close(client_socket);
	exit(0);
}