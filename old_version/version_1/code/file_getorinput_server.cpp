////////////////////////////////////////////////////////////////////////
// file_server.c -- socketÎÄ¼þ´«Êä·þÎñÆ÷¶ËÊ¾Àý´úÂë
// /////////////////////////////////////////////////////////////////////
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
	char message[max_buffer_size];
    // set socket's address information
    // ÉèÖÃÒ»¸ösocketµØÖ·½á¹¹server_addr,´ú±í·þÎñÆ÷internetµÄµØÖ·ºÍ¶Ë¿Ú
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
	// ¶¨Òå¿Í»§¶ËµÄsocketµØÖ·½á¹¹client_addr£¬µ±ÊÕµ½À´×Ô¿Í»§¶ËµÄÇëÇóºó£¬µ÷ÓÃaccept
	// ½ÓÊÜ´ËÇëÇó£¬Í¬Ê±½«client¶ËµÄµØÖ·ºÍ¶Ë¿ÚµÈÐÅÏ¢Ð´Èëclient_addrÖÐ
	struct sockaddr_in client_addr;
	socklen_t          length = sizeof(client_addr);

	// ½ÓÊÜÒ»¸ö´Óclient¶Ëµ½´ïserver¶ËµÄÁ¬½ÓÇëÇó,½«¿Í»§¶ËµÄÐÅÏ¢±£´æÔÚclient_addrÖÐ
	// Èç¹ûÃ»ÓÐÁ¬½ÓÇëÇó£¬ÔòÒ»Ö±µÈ´ýÖ±µ½ÓÐÁ¬½ÓÇëÇóÎªÖ¹£¬ÕâÊÇacceptº¯ÊýµÄÌØÐÔ£¬¿ÉÒÔ
	// ÓÃselect()À´ÊµÏÖ³¬Ê±¼ì²â
	// accpet·µ»ØÒ»¸öÐÂµÄsocket,Õâ¸ösocketÓÃÀ´Óë´Ë´ÎÁ¬½Óµ½serverµÄclient½øÐÐÍ¨ÐÅ
	// ÕâÀïµÄnew_server_socket´ú±íÁËÕâ¸öÍ¨ÐÅÍ¨µÀ
	int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);	
	if(new_server_socket < 0)
	{
		printf("Server Accept Failed! \n");
	}
	scanf("%s",&message);

	if(send(new_server_socket,message,sizeof(message),0) < 0)
	{
		printf("Send Data to client %s Failed\n",message);
	}else{
		printf("发送的数据 :  %s \n",message);
	}
	close(new_server_socket);
	exit(0);

}




