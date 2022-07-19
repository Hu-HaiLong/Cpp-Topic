#include <winsock.h> 
#include<iostream>
#pragma comment(lib,"ws2_32.lib") 

#include <Windows.h>
#include<string>


int main()
{
	//初始化Windows Socket Application
	WORD SockVersion = MAKEWORD(2, 2);
	WSADATA WSAData;

	//WinSock的注册函数,初始化底层的Windows Sockets DLL
	if (WSAStartup(SockVersion, &WSAData) != 0)
		return 0;

	// 创建一个socket并返回socket的标识符
	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cout << "invalid socket!" << std::endl;
		return 0;
	}

	// IP 端口
	sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(8888);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// 连接
	if (connect(ClientSocket, (sockaddr*)& ServerAddr, sizeof(ServerAddr))  == SOCKET_ERROR)
	{
		//连接失败 	
		std::cout << "connect error !" << std::endl;
		closesocket(ClientSocket);
		return 0;
	}

	std::string Data = "";

	int Index = 1;
	while (1)
	{
		std::cout << "Input: ";
		std::cin >> Data;

		send(ClientSocket, Data.c_str(), 100, 0);//一次发送多,接收少

		if (Index++ >= 5)
		{
			break;
		}

		Sleep(1000);
	}

	//关闭
	closesocket(ClientSocket);
	WSACleanup();
	system("pause");
	return 0;
}