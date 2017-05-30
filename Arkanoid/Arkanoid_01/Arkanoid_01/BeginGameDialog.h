#pragma once

#include <Windows.h>
#include "resource.h"

class CBeginGameDialog {
public:
	CBeginGameDialog();
	~CBeginGameDialog();

	// Создать и показать экземпляр окна
	bool Create(HWND hWndParent);

	// Вернуть хендл окна
	HWND GetWindowHandle() { return handle; }

protected:
	// Вызывается при получении сообщения WM_INITDIALOG
	void OnInit(HWND _handle);

	// Вызывается при получении сообщения WM_COMMAND
	INT_PTR OnCommand(WPARAM wParam);

private:
	HWND handle;  // хэндл окна
	HWND parentHandle;

	static INT_PTR _stdcall dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam);
};