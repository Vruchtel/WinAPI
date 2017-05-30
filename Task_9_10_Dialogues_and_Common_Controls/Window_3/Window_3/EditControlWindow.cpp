#include "EditControlWindow.h"

CEditControlWindow::CEditControlWindow()
{
}

CEditControlWindow::~CEditControlWindow() {}

bool CEditControlWindow::Create(HWND hWndParent)
{
	handle = CreateWindowEx(0,
		L"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hWndParent,
		0,
		GetModuleHandle(0),
		0);

	SetFocus(handle);

	if (handle == 0) {
		return false;
	}
	return true;
}

void CEditControlWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}