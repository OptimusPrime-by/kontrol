//server//server//server//server//server//server//server//server//server//server//server//server//server//server//server//server

#include <Windows.h>
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	setlocale(0, "");

	HANDLE hNamedPipe;
	DWORD dwBytesRead;
	DWORD dwBytesWrite;
	char Message[100];
	int nMessageLength;


	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	// атрибуты безопасности и  отключе наследование дескрипторов


	sa.bInheritHandle = FALSE;
	sa.nLength = sizeof(sa);

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE); //устанавливаем доступ(разрешён всем)

	sa.lpSecurityDescriptor = &sd;   // созданный дескриптор используем в SECURITY_ATTRIBUTES

	hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\MyPipe", // name pipe
		PIPE_ACCESS_DUPLEX, // channel attributes
		PIPE_TYPE_MESSAGE | PIPE_WAIT, //data transfer mode
		1,//maximum number of channel instances
		0,//output buffer size
		0,//INPUT
		INFINITE,//CLIENT WAITING TIME
		&sa//security attributes
	);

	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		cerr << "Не удалось создать канал." << endl
			<< "Error: " << GetLastError() << endl;
		cin.get();
		return 0;
	}

	cout << "Сервер ждет соединения с клиентом." << endl;

	if (!ConnectNamedPipe(
		hNamedPipe,//PIPE HANDLE
		NULL  // ассинхроность 
	))
	{
		cerr << "Не удалось подключиться к каналу." << endl
			<< "Error: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cin.get();
		system("pause");
		return 0;
	}
	else cout << "Соединение с клиентом установлено" << endl;

	while (true) {

		if (!ReadFile(
			hNamedPipe,
			Message,
			sizeof(Message),
			&dwBytesRead,
			NULL
		)) {
			if (GetLastError() == 109) {
				cout << "End of chat" << endl;
				break;

			}
			cerr << "Сбой чтения данных из канала." << endl
				<< "Error: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");

			return 0;
		}
		else {
			if (strcmp(Message, "exit") == 0) {
				cout << "Чат закрыт." << endl;
				break;
			}
			else {
				cout << Message << endl;
			}
		}
		cin.getline(Message, 100);

		if (!WriteFile(
			hNamedPipe,
			Message,
			sizeof(char) * strlen(Message) + 1,
			&dwBytesWrite,
			NULL
		)) {
			cerr << "Запись не удалась." << endl
				<< "Error: " << GetLastError() << endl;

			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");
			return 0;
		}

		if (strcmp(Message, "exit") == 0) {
			cout << "Чат закрыт." << endl;
			//break;
		}

	}

	DisconnectNamedPipe(hNamedPipe);
	CloseHandle(hNamedPipe);
	system("pause");
	return 0;
}



/*

PIPE_ACCESS_INBOUND — клиент пишет, а сервер читает данные
*/

