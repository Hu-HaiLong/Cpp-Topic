#include <winsock.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib") 
#include <Windows.h>
#include <string>

//采用select实现多个连接同时处理

int main()
{

	//初始化Windows Socket Application
	WORD SockVersion = MAKEWORD(2, 2);
	WSADATA WSAData;

	//WinSock的注册函数,初始化底层的Windows Sockets DLL
	if (WSAStartup(SockVersion, &WSAData) != 0)
		return 0;

	//创建服务器端监听套接字 
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cout << " create socket error!" << std::endl;
		return 0;
	}

	//绑定IP和端口  	
	sockaddr_in ServerAddr;
	ServerAddr.sin_family= AF_INET;
	ServerAddr.sin_port= htons(8888);//指定端口,将端口号转换为网络字节顺序
	ServerAddr.sin_addr.S_un.S_addr= INADDR_ANY;

	//bind()把socket绑定到特定的网络地址上
	if (bind(ServerSocket, (LPSOCKADDR) &ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		std::cout << " bind error!" << std::endl;
		return 0;
	}

	//开始监听
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
	{
		std::cout << " listen error !" << std::endl;
		return 0;
	}

	std::cout << " 服务器开启成功" << std::endl;

	FD_SET ReadSet;//创建文件描述集合
	FD_ZERO(&ReadSet);//清空文件描述集合
	FD_SET(ServerSocket, &ReadSet);//将监听套接字放入文件描述集合
	timeval TimeOut{ 0,0 };//select超时设置
	
	FD_SET TmpSet;

	while (true)
	{
		FD_ZERO(&TmpSet);
		TmpSet = ReadSet;

		//检查文件描述集合中的socket状态是否就绪
		int Result = select(0, &TmpSet, nullptr, nullptr, &TimeOut);//第五参数 nullptr为阻塞模式,时间为0则为非阻塞模式
		if (Result == SOCKET_ERROR)
		{
			continue;
		}

		for (size_t i = 0; i < TmpSet.fd_count; i++)
		{
			SOCKET CurSocket = TmpSet.fd_array[i];
			if (CurSocket == ServerSocket)//监听套接字有就绪
			{
				//循环接收数据
				SOCKET CurClientSocket;
				sockaddr_in RemoteAddr;
				int AddrLen = sizeof(RemoteAddr);
				//接收一个连接请求,并新建一个socket,原来的socket返回监听状态
				CurClientSocket = accept(ServerSocket, (SOCKADDR*)&RemoteAddr, &AddrLen);
				if (CurClientSocket == INVALID_SOCKET)
				{
					std::cout << " accept error !" << std::endl;
					continue;
				}
				//inet_ntoa()把长整型的IP地址数据转换成点分十进制的ASCII字符串	
				std::cout << " 接受一个连接: " << inet_ntoa(RemoteAddr.sin_addr)<< " :" << RemoteAddr.sin_port<< std::endl;
				FD_SET(CurClientSocket, &ReadSet);
			}
			else
			{
				//客户端连接的socket
				char Msg[10000];
				int Result = recv(CurSocket, Msg, 10000, 0);//接收数据
				if (Result == SOCKET_ERROR || Result == 0)
				{
					closesocket(CurSocket);
					FD_CLR(CurSocket, &ReadSet);
					std::cout << " 结束一个连接" << std::endl;
				}
				else
				{
					std::cout << " 接收到数据:" << Msg << std::endl;
				}
			}
		}
		Sleep(100);
	}

	closesocket(ServerSocket);
	//winsock的注销函数,从底层的Windows Sockets DLL 中撤销注册
	WSACleanup();
	system("pause");
	return 0;
}