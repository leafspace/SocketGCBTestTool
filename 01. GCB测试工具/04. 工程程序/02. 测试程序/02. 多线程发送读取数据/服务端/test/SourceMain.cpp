#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib") //��������Ŀ���Ե�������������ĸ��������������ws2_32.lib
#define  PORT 10000

HANDLE hMutexSocketLinkRecv;                                                // ���������̵߳��ź�
HANDLE hMutexSocketLinkSend;                                                // ���������̵߳��ź�

SOCKET sAccept;

DWORD WINAPI ThreadSocketLinkRecv(LPVOID lpParamter)
{
	WaitForSingleObject(hMutexSocketLinkRecv, INFINITE);
	do {
		char* msg = new char[500];
		int msgLen = recv(sAccept, msg, 500, 0);
		if (msg > 0) {
			printf("���������յ�����Ϣ\n");
		}
	} while (true);

	ReleaseMutex(hMutexSocketLinkRecv);

	return 0;
}

DWORD WINAPI ThreadSocketLinkSend(LPVOID lpParamter)
{
	WaitForSingleObject(hMutexSocketLinkSend, INFINITE);
	do
	{
		char buf[5] = { 0 };
		int iSend = send(sAccept, buf, sizeof(buf), 0);
		printf("��������������Ϣ\n");
	} while(true);

	ReleaseMutex(hMutexSocketLinkSend);
	return 0;
}

void main()
{
	int port = PORT;
	WSADATA wsaData;
	SOCKET sListen;

	int iLen;  //�ͻ���ַ����
	int iSend;  //�������ݳ���
	char buf[] = "hello,how are you";//��Ҫ���͵���Ϣ
	struct sockaddr_in serv, client;//���������ͻ��ĵ�ַ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Winsock load failed\n");
		return;
	}

	sListen = socket(AF_INET, SOCK_STREAM, 0);//�����׽���
	if (sListen == INVALID_SOCKET)
	{
		//�����׽���ʧ��
		printf("socket failed:%d\n", WSAGetLastError());//�������
		return;
	}

	//������������ַ
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);//��һ��˫�ֽ������ֽ�˳�������ת��Ϊ�����ֽ�˳��
	serv.sin_addr.s_addr = htonl(INADDR_ANY);//�����ֽ������ֽ�˳��ת��Ϊ�����ֽ�˳��INADDR_ANYΪϵͳָ����IP��ַ

	//��
	if (bind(sListen, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
	{
		//��ʧ��
		printf("bind() failed:%d\n", WSAGetLastError());
		return;
	}

	//�������״̬
	if (listen(sListen, 5) == SOCKET_ERROR)//���ڵȴ����ӵ���������5
	{
		//��������
		printf("listen() failed:%d\n", WSAGetLastError());
		return;
	}

	iLen = sizeof(client);//��ʼ���ͻ���ַ����

	//����ѭ���ȴ��ͻ�����������

	sAccept = accept(sListen, (struct sockaddr*)&client, &iLen);
	if (sAccept == INVALID_SOCKET)
	{
		printf("accept() failed:%d\n", WSAGetLastError());
		exit(1);
	}

	CreateThread(NULL, 0, ThreadSocketLinkRecv, NULL, 0, NULL);
	CreateThread(NULL, 0, ThreadSocketLinkSend, NULL, 0, NULL);

	Sleep(600000);
	closesocket(sListen);
	WSACleanup();
}