// Try3.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

// �������� CString
#include <atlstr.h> 

// �������� ������� time
#include <time.h>

void PrintError(DWORD errorCode)
{
	// �����
	LPVOID lpMsgBuf;

	// �������� ������
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// ������� ������
	std::wcout << "Error (" << errorCode << "): " << static_cast<LPCTSTR>(lpMsgBuf) << std::endl;

	LocalFree(lpMsgBuf);
}

// ������������ ������ ����� ������
void MemoryFree(std::vector<LPVOID>& pointersToAllocatedMemory, DWORD allocationType)
{
	LPVOID lpvResultPtr;

	for (LPVOID pointer : pointersToAllocatedMemory) {
		VirtualFree(pointer, 0, MEM_RELEASE);
	}
	pointersToAllocatedMemory.clear();

	// ��������, ��� ������ ������������
	lpvResultPtr = VirtualAlloc(NULL, 1000000, allocationType, PAGE_READWRITE);
	if (lpvResultPtr == NULL) {
		std::cout << "Sorry, memory has not been freed" << std::endl;
		exit(-1);
	}
	VirtualFree(lpvResultPtr, 0, MEM_RELEASE);
}

// 1 Task: ����������� ������������� ������ ������, ������� ����� ��������
void Task1(DWORD allocationType)  // allocationType ����� ���� ����� MEM_COMMIT ��� MEM_RESERVE
{
	// ���������� ��� �������� ����������� ��������� �� ���������� ������
	LPVOID lpvResultPtr;

	int memoryCounter = 0;
	const int bigAllocationSize = 4096 * 1000;
	// ������ ������ ��� ����������� ������ ����������� �����
	const int smallAllocationSize = 4096;

	std::vector<LPVOID> pointersToAllocatedMemory;
	pointersToAllocatedMemory.reserve(10000); // ������� �������� ������ ��� ���������

	// ��������� �������� ��� ���� �������� (�������� � ����������) allocationSize 
	// ��� ����� ��� ���� ����� �� ������ ������� ����� ��������� ������, 
	// �� ��� ���� ��������� ��� ��������� ������
	for(int allocationSize : {bigAllocationSize, smallAllocationSize}) {
		while(true) {
			// �������� �������� ����� ������ ������� allocationSize
			lpvResultPtr = VirtualAlloc(NULL, allocationSize, allocationType, PAGE_READWRITE);
			if(lpvResultPtr == NULL) {
				// ���� �� ���������� �������� �� ������� �� �����
				break;
			} else {
				// ���� ���������� ��������, �� ��������� �����, ����� ����� �������� ������
				pointersToAllocatedMemory.push_back(lpvResultPtr);
			}
			std::cout << lpvResultPtr << std::endl;
			// ����������� ������� ���������� ������
			memoryCounter += allocationSize;
		}
	}

	// �������� ��� ������
	DWORD errorCode = GetLastError();

	// ��������� ������
	// (������� ��� �����, ����� ���������� ������ �� ����� ����)
	MemoryFree(pointersToAllocatedMemory, allocationType);

	// ����� ����������
	std::cout << "Memory allocated: " << memoryCounter << " bytes" << std::endl; // ~1904496640
	std::cout << "Last error: ";
	PrintError(errorCode); // 8 - ERROR_NOT_ENOUGH_MEMORY
}

// 2 Task: ������������� ������������ ������
void Task2()
{
	int currentAllocationSize;
	bool isPageGuard;  // ����������, ����� �� ���������� ���� PAGE_GUARD
	LPVOID lpvResultPtr;  // ���������� ��� �������� ����������� ��������� �� ���������� ������
	int memoryCounter = 0;  // ������� ���������� ���������� ������
	std::vector<LPVOID> pointersToAllocatedMemory;
	pointersToAllocatedMemory.reserve(10000); // ������� �������� ������ ��� ���������

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
		// ���� ������ �������� ����������, ��������� ��������� �� �� � ������
		pointersToAllocatedMemory.push_back(lpvResultPtr);
		std::cout << lpvResultPtr << std::endl;
		memoryCounter += currentAllocationSize;
	}
	// �������� ��� ������
	DWORD errorCode = GetLastError();
	std::cout << "Error code: " << errorCode << std::endl;

	// ����������� ������
	MemoryFree(pointersToAllocatedMemory, MEM_COMMIT);

	// ����� ����������
	std::cout << "Memory allocated: " << memoryCounter << " bytes" << std::endl; // ~1904496640
	std::cout << "Last error: ";
	PrintError(errorCode); // 8 - ERROR_NOT_ENOUGH_MEMORY
}

// 3 Task: ������� �������� ������� ����� �������� ������ ������ �������� 
void Task3a()
{
	// ������� ������� ��������
	int handleCount = 0;
	// ������� �����
	HANDLE createdEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (createdEvent == NULL) {
		std::cout << "Impossible to create event" << std::endl;
	}
	std::cout << "Created event: " << createdEvent << std::endl;
	LPHANDLE currentDuplicatedHandle = NULL;
	std::vector<LPHANDLE> duplicatedHandles;
	duplicatedHandles.reserve(10000); // ������� �������� ������ ��� ���������

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

	// ������� ������
	CloseHandle(createdEvent);
	duplicatedHandles.clear();	

	// ������� ��������� ������
	//std::cout << "Last error:" << std::endl;
	//PrintError(errorCode);
	std::cout << "Error code: " << errorCode << std::endl;
}

// ������� �������� �������� ���� �������� �������
void Task3b()
{
	HANDLE createdEvent;
	std::vector<HANDLE> createdKernelObjects;
	createdKernelObjects.reserve(10000);
	int kernelObjectCount = 0;  // ������� �������� ����

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

	// ������������ ������
	for (HANDLE handle : createdKernelObjects) {
		CloseHandle(handle);
	}
	createdKernelObjects.clear();

	std::cout << "Last error: " << std::endl;
	PrintError(errorCode);
}

// 4 Task: ����� ������������� ������� �������� �������� ��� ���������
void Task4()
{
	HPEN createdPenHandle;
	int penCount = 0;  // ������� �����
	std::vector<HPEN> handlesToPens;
	handlesToPens.reserve(10000); // ������� �������� ������ ��� ���������
	while(true) {
		createdPenHandle = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		//std::cout << createdPenHandle << std::endl;
		if(createdPenHandle == NULL) {  // ������� ����� �� �������
			std::cout << "Cannot create a pen" << std::endl;
			break;
		}

		penCount++;
		handlesToPens.push_back(createdPenHandle);
	}
	DWORD errorCode = GetLastError();

	std::cout << "Count o pens: " << penCount << std::endl;
	std::cout << "Last error: ";

	// �������� ������
	for(HPEN hpen : handlesToPens) {
		DeleteObject(hpen);
	}
	handlesToPens.clear();

	PrintError(errorCode);
}


// 5, 6 Task: �������, ������� ������ �������� ����� �������
int stackTestCounter;

// "������" �������: �� �������� ��������� ����������
void testStackEmptyFunction()
{
	stackTestCounter++;
	if(stackTestCounter % 10 == 0) {  // ������������ ������� ������� ������� ��������
		std::cout << "Depth of recursion: " << stackTestCounter << std::endl;
	}
	testStackEmptyFunction();
}

// ������� � ����� ���������� �����������
void testStackWith8BytesFunction() 
{
	int a = 4, b = a + 1;
	stackTestCounter++;
	if(stackTestCounter % 10 == 0) {  // ������������ ������� ������� ������� ��������
		std::cout << "Depth of recursion: " << stackTestCounter << std::endl;
	}
	testStackWith8BytesFunction();
}

//������ �������� 4840  (���� 1MB)
// 47230 ����� ���������� ����� �� 10 MB
// ~222 ����� �������� ����� ����� ������ �������
void Task5()
{
	stackTestCounter = 0;
	testStackEmptyFunction();
}

// ������ �������� 4350 (���� 1MB)
// 42400 ����� ���������� ����� �� 10 MB
// ~247 ���� �� ����� ����� ������� � ����� ���������� �����������
// �������� ������� ������ ��������� ������������ �����-��
void Task6() 
{
	stackTestCounter = 0;
	testStackWith8BytesFunction();
}

// 8 Task: ����� ������������ ���� ����� ������� � �������� ������� Windows ��� ���������� ������ � ��� ����� WinAPI

// ������� �������� ������� ���� � ������ fileName
// ����������: ���������� �� �������
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
	// ������ ������ https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa364963(v=vs.85).aspx
	
	// ������ ���� ����� ������� ����
	const DWORD bufferSize = 10000;
	TCHAR  buffer[bufferSize] = TEXT("");

	// �������� �� �������� ��� �����
	TCHAR** lppPart = { NULL };

	// ���������� ������ ����
	DWORD retval = GetFullPathName(fileName,
		bufferSize,
		buffer,
		lppPart);

	// ���������� ����� ����� ���� � ��������
	return CString(buffer).GetLength();
}


// 8 �������
// ���������� �������� ��������, �� �������� ����� 255
void Task8() 
{
	// �������� ��������� ����������,
	// ����� �� �������� ������� �������
	CreateDirectory(CString("tmp"), NULL);

	// ����� ��������� ������� ��� ������� �����
	CString fileName = "tmp/file_0";
	for(int i = 0; ; i++) {
		if(!IsPossibleToCreateFile((LPCTSTR)fileName)) {
			std::cout << "Can't create file with length of name " << fileName.GetLength() << std::endl;
			break;
		}
		// ������ ��� ��������� ��� �����
		fileName.Append(CString("0"));
	}
	
	// ������ ����� 
	fileName = "f";
	//fileName = "fileeeee"; // ���� �������� ���, �� ����� ������ 255 ����� ��������
	CString dirName = "tmp\\u";
	// ����� ��������� ��������� ������� � ��������� �������
	for(int i = 0; ; i++) {
		// ������� ����� ��������� ����������
		CreateDirectory(dirName, NULL);
		// ��������� ����� �� ������� � ��� ����
		if(!IsPossibleToCreateFile((LPCTSTR)(dirName + "\\" + fileName))) {
			std::cout << "Can't create file and directories with length of path "
				<< getFullFilePathLength(dirName + "\\" + fileName) << std::endl;
			break;
		}
		// � ��������� ��� ����� ��������� ��� ����� ��������� ����������
		dirName.Append(CString("\\u"));
	}
	system("pause");
	// ������ ������ �� �����
	// /S - ������� ����������
	// /Q - �� ������� �������������
	system("del /S /Q tmp");
	exit(0);
}

int main()
{
	srand(time(NULL));

	// �������� ������� ���� � �������, ����� �������� ������
	setlocale(LC_ALL, "Russian");

	//Task1(MEM_COMMIT); // ~2041970688 bytes ~ 1950 Mb
	//Task1(MEM_RESERVE); // ~ ���������� ����� � ������������: ~2038132736 bytes
	//Task2();
	Task3a();  // 16711626 handles (1509360 �������� Process Explorer), error 1450: Error_No_System_Resources 
	//Task3b(); // 1671627 obj, err 1450: ������������ ��������� �������� ��� ���������� ��������
	//Task4();  // ������������ ������: 9996
	//Task5();
	//Task6();
	//Task8();

    return 0;
}