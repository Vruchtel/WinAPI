#include <Windows.h>
#include <atlstr.h> // CString
#include <cstdlib>

#include "OverlappedWindow.h"

void PrintError(char* errorText)
{
	int lastError = static_cast<int>(GetLastError());
	char errorString[10];
	itoa(lastError, errorString, 10);

	MessageBox(NULL,
		CString(errorString),
		CString(errorText),
		MB_DEFBUTTON1 | //��������� ������
		MB_ICONEXCLAMATION | //��� ������
		MB_DEFAULT_DESKTOP_ONLY);
}

// ������� ���������, ����� �� ���������� �������
void PrintHappyMessage(char* messageText) {
	MessageBox(NULL,
		CString(messageText),
		CString("���!"),
		MB_DEFBUTTON1 | //��������� ������
		MB_DEFAULT_DESKTOP_ONLY);
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR commandLine, int cmdShow)
{
	COverlappedWindow myWindow;
	bool registrationSuccess = myWindow.RegisterClass();

	if (!registrationSuccess) {
		PrintError("������! ���������������� ����� ���� �� ����������! ��� ������:");
		exit(-1);
	} 
	//PrintHappyMessage("����������� ���� ������ �������!");

	bool creatingSuccess = myWindow.Create();

	if (!creatingSuccess) {
		PrintError("������! ������� ���� �� ����������! ��� ������:");
		exit(-1);
	}
	//PrintHappyMessage("���� ���� ������� �������!");

	myWindow.Show(cmdShow);

	// ��������� ���� ��������� ���������
	MSG message;
	BOOL getMessageSucceed = 0;
	while ((getMessageSucceed = GetMessage(&message, 0, 0, 0)) != 0) {
		if (getMessageSucceed == -1) {
			exit(-1);
		}
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	//PrintHappyMessage("�� ������� ����������!");

	return 0;
}