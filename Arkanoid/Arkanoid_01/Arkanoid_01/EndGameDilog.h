#pragma once

#include <Windows.h>
#include "resource.h"

class CEndGameDialog {
public:
	CEndGameDialog();
	~CEndGameDialog();

	// ������� � �������� ��������� ����
	bool Create(HWND hWndParent);

	// ������� ����� ����
	HWND GetWindowHandle() { return handle; }

protected:
	// ���������� ��� ��������� ��������� WM_INITDIALOG
	void OnInit(HWND _handle);

	// ���������� ��� ��������� ��������� WM_COMMAND
	INT_PTR OnCommand(WPARAM wParam);

	// ���������� ��� ��������� ��������� WM_DESTROY
	//void OnDestroy(WPARAM wParam, LPARAM lParam);

private:
	HWND handle;  // ����� ����
	HWND parentHandle;

	static INT_PTR _stdcall dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam);
};