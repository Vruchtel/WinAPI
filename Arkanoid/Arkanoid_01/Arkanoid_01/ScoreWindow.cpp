#include "ScoreWindow.h"

#include <cstring>//////////////////
#include <atlstr.h>  
#include <sstream>  
#include <vector>


std::string ComposeFormat(const char *format, ...)
{
	std::vector<char> data;
	for (int loop = 0; loop < 2; loop++) {
		va_list args;
		va_start(args, format);
		int written_length = vsnprintf(
			data.data(), data.size(), format, args
		);
		va_end(args);
		// plus 1 for terminal 0 symbol in the end of the string
		data.resize(written_length + 1);
	}
	return data.data();
}

CScoreWindow::CScoreWindow()
{
	handle = 0;
	parentHandle = 0;
	currentTime = 0;
	score = 0;
}

CScoreWindow::~CScoreWindow() {}

bool CScoreWindow::RegisterClass()
{
	WNDCLASSEX windowClass;

	// «аполним структуру соответствующими данными
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = CScoreWindow::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"ScoreWindow";

	ATOM isRegistered = RegisterClassEx(&windowClass);
	if (!isRegistered) {
		return false;
	}

	return true;
}

bool CScoreWindow::Create(HWND hWndParent)
{
	parentHandle = hWndParent;

	CreateWindowEx(0,
		L"ScoreWindow",
		L"Score",
		WS_CHILD | WS_BORDER,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hWndParent,
		0,
		GetModuleHandle(0),
		this);

	if (handle == 0) {
		return false;
	}

	return true;
}

void CScoreWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

void CScoreWindow::OnCreate()
{
	const int millisecondsToChange = 80;
	timerId = SetTimer(handle, NULL, millisecondsToChange, NULL);
}

void CScoreWindow::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void CScoreWindow::OnDestroy()
{
	KillTimer(handle, timerId);
	PostQuitMessage(0);
}

void CScoreWindow::OnPaint()
{
	PAINTSTRUCT structToPaint;
	HDC displayHandle = BeginPaint(handle, &structToPaint);

	RECT clientRect; 
	GetClientRect(handle, &clientRect);

	HBRUSH backgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
	HGDIOBJ oldBrush = SelectObject(displayHandle, backgroundBrush);
	FillRect(displayHandle, &clientRect, backgroundBrush);

	std::string stringToPrint = ComposeFormat("Score: %d", score);
	TextOut(displayHandle, 0, 5, CString(stringToPrint.c_str()), stringToPrint.length());

	SelectObject(displayHandle, oldBrush);
	DeleteObject(backgroundBrush);

	EndPaint(handle, &structToPaint);
}

void CScoreWindow::OnTimer()
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	currentTime += 1;
	InvalidateRect(handle, &clientRect, TRUE);
}

LRESULT CScoreWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CScoreWindow* window = (CScoreWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);
	switch (message) {
	case WM_NCCREATE:
	{
		CScoreWindow* window = (CScoreWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLongPtr(handle, GWLP_HINSTANCE, (LONG)window);
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE:
	{
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		window->OnDestroy();
		return 0;
	}
	case WM_PAINT:
	{
		window->OnPaint();
		return 0;
	}
	case WM_TIMER:
	{
		window->OnTimer();
		return 0;
	}
	/*case WM_LBUTTONDOWN:
	{
		window->OnLButtonDown();
		return 0;
	}*/
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
