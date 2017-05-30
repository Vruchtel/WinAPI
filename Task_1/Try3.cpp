// Try3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

// Включаем CString
#include <atlstr.h> 

// Содержит функцию time
#include <time.h>

void PrintError(DWORD errorCode)
{
	// Буфер
	LPVOID lpMsgBuf;

	// Получаем ошибку
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Выводим ошибку
	std::wcout << "Error (" << errorCode << "): " << static_cast<LPCTSTR>(lpMsgBuf) << std::endl;

	LocalFree(lpMsgBuf);
}

// Освобождение памяти после работы
void MemoryFree(std::vector<LPVOID>& pointersToAllocatedMemory, DWORD allocationType)
{
	LPVOID lpvResultPtr;

	for (LPVOID pointer : pointersToAllocatedMemory) {
		VirtualFree(pointer, 0, MEM_RELEASE);
	}
	pointersToAllocatedMemory.clear();

	// проверим, что память освободилась
	lpvResultPtr = VirtualAlloc(NULL, 1000000, allocationType, PAGE_READWRITE);
	if (lpvResultPtr == NULL) {
		std::cout << "Sorry, memory has not been freed" << std::endl;
		exit(-1);
	}
	VirtualFree(lpvResultPtr, 0, MEM_RELEASE);
}

// 1 Task: Определение максимального объёма памяти, который можно выделить
void Task1(DWORD allocationType)  // allocationType может быть равен MEM_COMMIT или MEM_RESERVE
{
	// Переменная для хранения полученного указателя на выделенную память
	LPVOID lpvResultPtr;

	int memoryCounter = 0;
	const int bigAllocationSize = 4096 * 1000;
	// Размер выбран как минимальный размер выделяемого блока
	const int smallAllocationSize = 4096;

	std::vector<LPVOID> pointersToAllocatedMemory;
	pointersToAllocatedMemory.reserve(10000); // Заранее выделяем память под указатели

	// Повторяем действия для двух значений (большого и маленького) allocationSize 
	// это нужно для того чтобы не делать слишком много выделений памяти, 
	// но при этом вычерпать всю доступную память
	for(int allocationSize : {bigAllocationSize, smallAllocationSize}) {
		while(true) {
			// Пытаемся выделить кусок памяти размера allocationSize
			lpvResultPtr = VirtualAlloc(NULL, allocationSize, allocationType, PAGE_READWRITE);
			if(lpvResultPtr == NULL) {
				// Если не получилось выделить то выходим из цикла
				break;
			} else {
				// Если получилось выделить, то сохраняем адрес, чтобы потом очистить память
				pointersToAllocatedMemory.push_back(lpvResultPtr);
			}
			std::cout << lpvResultPtr << std::endl;
			// увеличиваем счетчик выделенной памяти
			memoryCounter += allocationSize;
		}
	}

	// Получаем код ошибки
	DWORD errorCode = GetLastError();

	// Освободим память
	// (сделаем это сразу, чтобы недостаток памяти не мешал жить)
	MemoryFree(pointersToAllocatedMemory, allocationType);

	// вывод информации
	std::cout << "Memory allocated: " << memoryCounter << " bytes" << std::endl; // ~1904496640
	std::cout << "Last error: ";
	PrintError(errorCode); // 8 - ERROR_NOT_ENOUGH_MEMORY
}

// 2 Task: Искусственная фрагментация памяти
void Task2()
{
	int currentAllocationSize;
	bool isPageGuard;  // Определяет, нужно ли выставлять флаг PAGE_GUARD
	LPVOID lpvResultPtr;  // Переменная для хранения полученного указателя на выделенную память
	int memoryCounter = 0;  // Счётчик количества выделенной памяти
	std::vector<LPVOID> pointersToAllocatedMemory;
	pointersToAllocatedMemory.reserve(10000); // Заранее выделяем память под указатели

	while(true) {
		currentAllocationSize = 1024 * 1024 * 64;
		std::cout << "Current allocation size: " << currentAllocationSize << std::endl;
		isPageGuard = static_cast<bool>(rand() % 2);
		std::cout << "is page guard: " << isPageGuard << std::endl;
		if(isPageGuard) {
			lpvResultPtr = VirtualAlloc(NULL, currentAllocationSize, MEM_COMMIT, PAGE_READWRITE | PAGE_GUARD);  // 87 - the parameter is incorrect
		} else {
			lpvResultPtr = VirtualAlloc(NULL, currentAllocationSize, MEM_COMMIT, PAGE_READWRITE);
		}
		//lpvResultPtr = VirtualAlloc(NULL, currentAllocationSize, MEM_COMMIT, PAGE_READWRITE);

		if (lpvResultPtr == NULL) {
			break;
		}
		// Если память выделить получилось, добавляем указатель на неё в массив
		pointersToAllocatedMemory.push_back(lpvResultPtr);
		std::cout << lpvResultPtr << std::endl;
		memoryCounter += currentAllocationSize;
	}
	// Получаем код ошибки
	DWORD errorCode = GetLastError();
	std::cout << "Error code: " << errorCode << std::endl;

	// Освобождаем память
	MemoryFree(pointersToAllocatedMemory, MEM_COMMIT);

	// вывод информации
	std::cout << "Memory allocated: " << memoryCounter << " bytes" << std::endl; // ~1904496640
	std::cout << "Last error: ";
	PrintError(errorCode); // 8 - ERROR_NOT_ENOUGH_MEMORY
}

// 3 Task: Сколько максимум хэндлов можно получить внутри одного процесса 
void Task3a()
{
	// Счётчик хэндлов процесса
	int handleCount = 0;
	// Получем хэндл
	HANDLE createdEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (createdEvent == NULL) {
		std::cout << "Impossible to create event" << std::endl;
	}
	std::cout << "Created event: " << createdEvent << std::endl;
	LPHANDLE currentDuplicatedHandle = NULL;
	std::vector<LPHANDLE> duplicatedHandles;
	duplicatedHandles.reserve(10000); // Заранее выделяем память под указатели

	while (true) {
		bool successOfDuplication = DuplicateHandle(GetCurrentProcess(),
			createdEvent,
			GetCurrentProcess(),
			currentDuplicatedHandle,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS);
		if (successOfDuplication == 0) {
			std::cout << "Can't duplicate a handle" << std::endl;
			break;
		}
		handleCount++;
		duplicatedHandles.push_back(currentDuplicatedHandle);
		//std::cout << "Duplicated handle: " << currentDuplicatedHandle << std::endl;
		//std::cout << "With result: " << successOfDuplication << std::endl;
	}
	DWORD errorCode = GetLastError();
	std::cout << "Handles created: " << handleCount << std::endl;

	// Очистка памяти
	CloseHandle(createdEvent);
	duplicatedHandles.clear();	

	// Выводим последнюю ошибку
	//std::cout << "Last error:" << std::endl;
	//PrintError(errorCode);
	std::cout << "Error code: " << errorCode << std::endl;
}

// Сколько максимум объектов ядра возможно создать
void Task3b()
{
	HANDLE createdEvent;
	std::vector<HANDLE> createdKernelObjects;
	createdKernelObjects.reserve(10000);
	int kernelObjectCount = 0;  // Счётчик объектов ядра

	while(true)
	{
		createdEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
		if(createdEvent == NULL) {
			std::cout << "Impossible to create event" << std::endl;
			break;
		}
		//std::cout << "Created event: " << createdEvent << std::endl;
		kernelObjectCount++;
		createdKernelObjects.push_back(createdEvent);
	}
	DWORD errorCode = GetLastError();

	std::cout << "Count of created kernel pbjects: " << kernelObjectCount << std::endl;

	// Освобождение памяти
	for (HANDLE handle : createdKernelObjects) {
		CloseHandle(handle);
	}
	createdKernelObjects.clear();

	std::cout << "Last error: " << std::endl;
	PrintError(errorCode);
}

// 4 Task: Поиск практического предела создания объектов для рисования
void Task4()
{
	HPEN createdPenHandle;
	int penCount = 0;  // Счётчик ручек
	std::vector<HPEN> handlesToPens;
	handlesToPens.reserve(10000); // Заранее выделяем память под указатели
	while(true) {
		createdPenHandle = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		//std::cout << createdPenHandle << std::endl;
		if(createdPenHandle == NULL) {  // Создать ручку не удалось
			std::cout << "Cannot create a pen" << std::endl;
			break;
		}

		penCount++;
		handlesToPens.push_back(createdPenHandle);
	}
	DWORD errorCode = GetLastError();

	std::cout << "Count o pens: " << penCount << std::endl;
	std::cout << "Last error: ";

	// Очистить память
	for(HPEN hpen : handlesToPens) {
		DeleteObject(hpen);
	}
	handlesToPens.clear();

	PrintError(errorCode);
}


// 5, 6 Task: подсчёт, сколько памяти занимает вызов функций
int stackTestCounter;

// "Пустая" функция: не содержит локальных переменных
void testStackEmptyFunction()
{
	stackTestCounter++;
	if(stackTestCounter % 10 == 0) {  // Периодически выводим текущую глубину рекурсии
		std::cout << "Depth of recursion: " << stackTestCounter << std::endl;
	}
	testStackEmptyFunction();
}

// Функция с двумя локальными переменными
void testStackWith8BytesFunction() 
{
	int a = 4, b = a + 1;
	stackTestCounter++;
	if(stackTestCounter % 10 == 0) {  // Периодически выводим текущую глубину рекурсии
		std::cout << "Depth of recursion: " << stackTestCounter << std::endl;
	}
	testStackWith8BytesFunction();
}

//падает достигая 4840  (стек 1MB)
// 47230 после увеличения стека до 10 MB
// ~222 байта занимает вызов одной пустой функции
void Task5()
{
	stackTestCounter = 0;
	testStackEmptyFunction();
}

// падает достигая 4350 (стек 1MB)
// 42400 после увеличения стека до 10 MB
// ~247 байт на вызов одной функции с двумя локальными переменными
// довольно странно видимо присходит выравнивание какое-то
void Task6() 
{
	stackTestCounter = 0;
	testStackWith8BytesFunction();
}

// 8 Task: какой максимальный путь можно создать в файловой системе Windows для корректной работы с ним через WinAPI

// функция пытается создать файл с именем fileName
// возвращает: получилось ли создать
bool IsPossibleToCreateFile(LPCTSTR fileName) 
{
	HANDLE file = CreateFile(fileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == file) {
		return false;
	}
	CloseHandle(file);
	return true;
}

int getFullFilePathLength(CString fileName) 
{
	// Логика отсюда https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa364963(v=vs.85).aspx
	
	// Полный путь будет написан сюда
	const DWORD bufferSize = 10000;
	TCHAR  buffer[bufferSize] = TEXT("");

	// Укзатель на ненужный нам буфер
	TCHAR** lppPart = { NULL };

	// Спрашиваем полный путь
	DWORD retval = GetFullPathName(fileName,
		bufferSize,
		buffer,
		lppPart);

	// Возвращаем длину этого пути в символах
	return CString(buffer).GetLength();
}


// 8 задание
// результаты довольно странные, но вертятся около 255
void Task8() 
{
	// Создадим временную директорию,
	// чтобы не засорять каталог проекта
	CreateDirectory(CString("tmp"), NULL);

	// Будем создавать файлики все большей длины
	CString fileName = "tmp/file_0";
	for(int i = 0; ; i++) {
		if(!IsPossibleToCreateFile((LPCTSTR)fileName)) {
			std::cout << "Can't create file with length of name " << fileName.GetLength() << std::endl;
			break;
		}
		// Каждый раз удлинняем имя файла
		fileName.Append(CString("0"));
	}
	
	// Вторая часть 
	fileName = "f";
	//fileName = "fileeeee"; // если написать так, то можно больше 255 длину получить
	CString dirName = "tmp\\u";
	// Будем создавать вложенные папочки с короткими именами
	for(int i = 0; ; i++) {
		// Создаем новую вложенную директорию
		CreateDirectory(dirName, NULL);
		// Проверяем можно ли создать в ней файл
		if(!IsPossibleToCreateFile((LPCTSTR)(dirName + "\\" + fileName))) {
			std::cout << "Can't create file and directories with length of path "
				<< getFullFilePathLength(dirName + "\\" + fileName) << std::endl;
			break;
		}
		// в следующий раз будем создавать еще более вложенную директорию
		dirName.Append(CString("\\u"));
	}
	system("pause");
	// удалим лишнее за собой
	// /S - удалять рекурсивно
	// /Q - не просить подтверждения
	system("del /S /Q tmp");
	exit(0);
}

int main()
{
	srand(time(NULL));

	// Включаем русский язык в консоли, чтобы выводить ошибку
	setlocale(LC_ALL, "Russian");

	//Task1(MEM_COMMIT); // ~2041970688 bytes ~ 1950 Mb
	//Task1(MEM_RESERVE); // ~ предыдущий ответ с погрешностью: ~2038132736 bytes
	//Task2();
	Task3a();  // 16711626 handles (1509360 согласно Process Explorer), error 1450: Error_No_System_Resources 
	//Task3b(); // 1671627 obj, err 1450: недостаточно системных ресурсов для завершения операции
	//Task4();  // Практический предел: 9996
	//Task5();
	//Task6();
	//Task8();

    return 0;
}