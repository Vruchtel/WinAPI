#pragma once

#include <Windows.h>
#include "resource.h"

class CBeginGameDialog {
public:
	CBeginGameDialog();
	~CBeginGameDialog();

	// ������� � �������� ��������� ����
	bool Create(HWND hWndParent);

	// ������� ����� ����
	HWND GetWindowHandle() { return handle; }

protected:
	// ���������� ��� ��������� ��������� WM_INITDIALOG
	void OnInit(HWND _handle);

	// ���������� ��� ��������� ��������� WM_COMMAND
	INT_PTR OnCommand(WPARAM wParam);

private:
	HWND handle;  // ����� ����
	HWND parentHandle;

	static INT_PTR _stdcall dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam);
};