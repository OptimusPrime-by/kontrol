// kont.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// client// client// client// client// client// client// client

#include <windows.h>
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	setlocale(0, "");
	char computerName[80];
	char pipeName[80];
	HANDLE hNamedPipe;
	DWORD dwBytesWritten;
	DWORD dwBytesRead;
	char Message[100];
	int nMessageLength;

	cout << "Введите имя сервера компьютера: ";
	cin.getline(computerName, 80);

	wsprintf(pipeName, "\\\\%s\\pipe\\MyPipe", computerName);  //преобразуем введённое имя канала в нужный формат

	if (!WaitNamedPipe(pipeName,     //когда канал pipeName будет доступен для соединения
		NMPWAIT_WAIT_FOREVER       //INTERVAL PENDING
	)) {
		cout << "Канал не существует или занят" << endl
			<< "Error: " << GetLastError() << endl;
		cin.get();
		return 0;
	}
	else cout << "Канал свободен." << endl;

	hNamedPipe = CreateFile(                                //подключение к каналу
		pipeName,
		GENERIC_READ | GENERIC_WRITE,    //way to access 
		0,      // way to share file   
		NULL,             //security attributes
		OPEN_EXISTING,             //action when creating or opening a file
		0,    //attribute file
		NULL);           //inherited another file attributes

	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Ошибка поключения к каналу." << endl
			<< "Error: " << GetLastError() << endl;
		cin.get();
		system("pause");
		return 0;
	}
	else cout << "Вы подключились к каналу. Можно начинать общение! " << endl;


	while (true)
	{

		cin.getline(Message, 100);             //ввод сообщения

		nMessageLength = strlen(Message) + 1;

		if (strcmp(Message, "exit") == 0) {
			cout << "Чат закрыт." << endl;
			break;
		}

		if (!WriteFile(                                 //отправка сообщения    
			hNamedPipe,
			Message,
			sizeof(char) * strlen(Message) + 1,
			&dwBytesWritten,
			NULL
		)) {
			cerr << "Запись не удалась." << endl
				<< "Error: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");

			return 0;
		}


		if (!ReadFile(                        //чтение сообщения
			hNamedPipe,
			Message,
			sizeof(Message),
			&dwBytesRead,
			NULL
		))
		{
			cerr << "Сбой чтения данных из канала." << endl << "Error: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");
			return 0;
		}
		else {
			if (strcmp(Message, "exit") == 0)
			{
				cout << "Чат закрыт." << endl;
				break;
			}
			else {
				cout << Message << endl;
			}
		}

	}
	DisconnectNamedPipe(hNamedPipe);
	CloseHandle(hNamedPipe);
	system("pause");
	return 0;

}