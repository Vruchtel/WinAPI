#include <Windows.h>
#include <atlstr.h> // CString
#include <cstdlib>

#include "OverlappedWindow.h"
#include "resource.h"

void PrintError(char* errorText)
{
	int lastError = static_cast<int>(GetLastError());
	char errorString[10];
	itoa(lastError, errorString, 10);

	MessageBox(NULL,
		CString(errorString),
		CString(errorText),
		MB_DEFBUTTON1 | //Выделеная кнопка
		MB_ICONEXCLAMATION | //Тип иконки
		MB_DEFAULT_DESKTOP_ONLY);
}

// Выводит сообщение, когда всё отработало успешно
void PrintHappyMessage(char* messageText) {
	MessageBox(NULL,
		CString(messageText),
		CString("Ура!"),
		MB_DEFBUTTON1 | //Выделеная кнопка
		MB_DEFAULT_DESKTOP_ONLY);
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR commandLine, int cmdShow)
{
	COverlappedWindow myWindow;
	bool registrationSuccess = myWindow.RegisterClass();

	if (!registrationSuccess) {
		PrintError("Ошибка! Зарегистрировать класс окна не получилось! Код ошибки:");
		exit(-1);
	}
	//PrintHappyMessage("Регистрация окна прошла успешно!");

	bool creatingSuccess = myWindow.Create();/////////////////////////

	if (!creatingSuccess) {
		PrintError("Ошибка! Создать окно не получилось! Код ошибки:");
		exit(-1);
	}
	//PrintHappyMessage("Окно было успешно создано!");

	myWindow.Show(cmdShow);

	HACCEL acceleratorsTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	// Запускаем цикл обработки сообщений
	MSG message;
	BOOL getMessageSucceed = false;
	while ((getMessageSucceed = GetMessage(&message, 0, 0, 0)) != 0) {
		if (getMessageSucceed == -1) {
			exit(-1);
		}
		if (!TranslateAccelerator(myWindow.GetWindowHandle(), acceleratorsTable, &message)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	DestroyAcceleratorTable(acceleratorsTable);

	//PrintHappyMessage("Всё успешно отработало!");

	return 0;
}