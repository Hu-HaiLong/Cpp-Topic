#include <winsock.h> 
#include<iostream>
#pragma comment(lib,"ws2_32.lib") 

#include <Windows.h>
#include<string>


int main()
{
	//��ʼ��Windows Socket Application
	WORD SockVersion = MAKEWORD(2, 2);
	WSADATA WSAData;

	//WinSock��ע�ắ��,��ʼ���ײ��Windows Sockets DLL
	if (WSAStartup(SockVersion, &WSAData) != 0)
		return 0;

	// ����һ��socket������socket�ı�ʶ��
	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cout << "invalid socket!" << std::endl;
		return 0;
	}

	// IP �˿�
	sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(8888);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// ����
	if (connect(ClientSocket, (sockaddr*)& ServerAddr, sizeof(ServerAddr))  == SOCKET_ERROR)
	{
		//����ʧ�� 	
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

		send(ClientSocket, Data.c_str(), 100, 0);//һ�η��Ͷ�,������

		if (Index++ >= 5)
		{
			break;
		}

		Sleep(1000);
	}

	//�ر�
	closesocket(ClientSocket);
	WSACleanup();
	system("pause");
	return 0;
}