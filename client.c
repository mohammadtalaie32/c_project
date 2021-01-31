#include<stdio.h>
#include<winsock2.h>
#define MAX 80
#define PORT 135

#pragma comment(lib,"ws2_32.lib") //Winsock Library

struct sockaddr_in server;
int fill_recieve_buffer(int s);

int main(int argc , char *argv[])
{
	WSADATA wsa;
	SOCKET s;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	
	printf("Initialised.\n");
	
	
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");

	memset(&server , 0 , sizeof(server));
	server.sin_addr.s_addr = inet_addr("127.0.0.1");  // اشاره به ادرسی سیستم خودمون
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");
	fill_recieve_buffer(s);
	return 0;
}


int fill_recieve_buffer(int s) {
	char buff[MAX];
	int n;
	for(;;) { 
		memset(buff , '\0' , sizeof(buff));
		printf("Enter Your Message: ");
		n = 0;
		while((buff[n++] = getchar()) != '\n') 
			;
		if( send(s , buff , strlen(buff) , 0) < 0)  // مقادیر داخل بافر را به سرور ارسال میکنیم اگر ارسال با ایراد مواجه شود پیام ارسال با شکست مواجه شد را دریافت میکنیم
		{
			puts("Send failed");
			return 1;
		}
		memset(buff , '\0' , sizeof(buff));
		printf("Message Sent! \n");
		
		if((n = recv(s , buff , MAX , 0)) == SOCKET_ERROR)
		{
			puts("recv failed");
		}
	
		puts("Reply received\n");

		//Add a NULL terminating character to make it a proper string before printing
		buff[n] = '\0';
		printf("New Message: %s" , buff);
	}
}