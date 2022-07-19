#include <winsock.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib") 
#include <Windows.h>
#include <string>

//����selectʵ�ֶ������ͬʱ����

int main()
{

	//��ʼ��Windows Socket Application
	WORD SockVersion = MAKEWORD(2, 2);
	WSADATA WSAData;

	//WinSock��ע�ắ��,��ʼ���ײ��Windows Sockets DLL
	if (WSAStartup(SockVersion, &WSAData) != 0)
		return 0;

	//�����������˼����׽��� 
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cout << " create socket error!" << std::endl;
		return 0;
	}

	//��IP�Ͷ˿�  	
	sockaddr_in ServerAddr;
	ServerAddr.sin_family= AF_INET;
	ServerAddr.sin_port= htons(8888);//ָ���˿�,���˿ں�ת��Ϊ�����ֽ�˳��
	ServerAddr.sin_addr.S_un.S_addr= INADDR_ANY;

	//bind()��socket�󶨵��ض��������ַ��
	if (bind(ServerSocket, (LPSOCKADDR) &ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		std::cout << " bind error!" << std::endl;
		return 0;
	}

	//��ʼ����
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
	{
		std::cout << " listen error !" << std::endl;
		return 0;
	}

	std::cout << " �����������ɹ�" << std::endl;

	FD_SET ReadSet;//�����ļ���������
	FD_ZERO(&ReadSet);//����ļ���������
	FD_SET(ServerSocket, &ReadSet);//�������׽��ַ����ļ���������
	timeval TimeOut{ 0,0 };//select��ʱ����
	
	FD_SET TmpSet;

	while (true)
	{
		FD_ZERO(&TmpSet);
		TmpSet = ReadSet;

		//����ļ����������е�socket״̬�Ƿ����
		int Result = select(0, &TmpSet, nullptr, nullptr, &TimeOut);//������� nullptrΪ����ģʽ,ʱ��Ϊ0��Ϊ������ģʽ
		if (Result == SOCKET_ERROR)
		{
			continue;
		}

		for (size_t i = 0; i < TmpSet.fd_count; i++)
		{
			SOCKET CurSocket = TmpSet.fd_array[i];
			if (CurSocket == ServerSocket)//�����׽����о���
			{
				//ѭ����������
				SOCKET CurClientSocket;
				sockaddr_in RemoteAddr;
				int AddrLen = sizeof(RemoteAddr);
				//����һ����������,���½�һ��socket,ԭ����socket���ؼ���״̬
				CurClientSocket = accept(ServerSocket, (SOCKADDR*)&RemoteAddr, &AddrLen);
				if (CurClientSocket == INVALID_SOCKET)
				{
					std::cout << " accept error !" << std::endl;
					continue;
				}
				//inet_ntoa()�ѳ����͵�IP��ַ����ת���ɵ��ʮ���Ƶ�ASCII�ַ���	
				std::cout << " ����һ������: " << inet_ntoa(RemoteAddr.sin_addr)<< " :" << RemoteAddr.sin_port<< std::endl;
				FD_SET(CurClientSocket, &ReadSet);
			}
			else
			{
				//�ͻ������ӵ�socket
				char Msg[10000];
				int Result = recv(CurSocket, Msg, 10000, 0);//��������
				if (Result == SOCKET_ERROR || Result == 0)
				{
					closesocket(CurSocket);
					FD_CLR(CurSocket, &ReadSet);
					std::cout << " ����һ������" << std::endl;
				}
				else
				{
					std::cout << " ���յ�����:" << Msg << std::endl;
				}
			}
		}
		Sleep(100);
	}

	closesocket(ServerSocket);
	//winsock��ע������,�ӵײ��Windows Sockets DLL �г���ע��
	WSACleanup();
	system("pause");
	return 0;
}