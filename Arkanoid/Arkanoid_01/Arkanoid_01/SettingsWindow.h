#pragma once

#include <Windows.h>
#include "resource.h"

// �����, ���������� � ���� ���������� � ���������� �����������
class CDisplaySettings {
public:
	DWORD rowsNum; // ���������� �����-����������
	DWORD ballPace; // �������� ������
};

class CSettingsWindow {
public:
	CSettingsWindow();
	~CSettingsWindow();

	// ������� � �������� ��������� ����
	bool Create(HWND hWndParent);

	// ������� ����� ����
	HWND GetWindowHandle() { return handle; }

protected:
	// ���������� ��� ��������� ��������� WM_INITDIALOG
	void OnInit(HWND _handle);

	// ���������� ��� ��������� ��������� WM_COMMAND
	INT_PTR OnCommand(WPARAM wParam);

	// ���������� ��� ��������� ��������� WM_HSCROLL
	void OnScroll(WPARAM wParam, LPARAM lParam);

	// ���������� ��� ��������� ��������� WM_DESTROY
	//void OnDestroy(WPARAM wParam, LPARAM lParam);

private:
	HWND handle;  // ����� ����

	CDisplaySettings oldSettings; // ������������� ���������
	CDisplaySettings newSettings; // ���������, ������� ����� ���� �����������

	HWND parentHandle;

	void changeSettingsTo(const CDisplaySettings &settings);

	//void setNewColorTo(DWORD &whatToChange);

	static INT_PTR _stdcall dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam);
};