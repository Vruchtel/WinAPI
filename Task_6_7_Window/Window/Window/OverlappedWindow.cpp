#include "OverlappedWindow.h"

COverlappedWindow::COverlappedWindow() 
{
	currentTime = 0;
}

COverlappedWindow::~COverlappedWindow() {}

bool COverlappedWindow::RegisterClass()
{
	WNDCLASSEX windowClass;

	// Заполним структуру соответствующими данными
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = COverlappedWindow::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"OverlappedWindow";
	
	ATOM isRegistered = RegisterClassEx(&windowClass);
	if(!isRegistered) {
		return false;
	}

	return true;
}

bool COverlappedWindow::Create()
{
	CreateWindowEx(0,
		L"OverlappedWindow", 
		L"My first window",
		WS_EX_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		GetModuleHandle(0),
		this);

	if(handle == 0) {
		return false;
	}

	return true;
}

void COverlappedWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

void COverlappedWindow::OnCreate()
{
	const int millisecondsToChange = 80;
	timerId = SetTimer(handle, NULL, millisecondsToChange, NULL);
}

void COverlappedWindow::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void COverlappedWindow::OnDestroy()
{
	KillTimer(handle, timerId);
	PostQuitMessage(0);
}

void COverlappedWindow::OnPaint()
{
	PAINTSTRUCT structToPaint;
	HDC displayHandle = BeginPaint(handle,	&structToPaint);

	RECT clientRect;  // структура, в которой будут координаты клиентской области
					  // Получим координаты клиентской области
	GetClientRect(handle, &clientRect);

	// Создаём кисть для закрашивания фона
	HBRUSH outsideEllipsBrush = CreateSolidBrush(RGB(250, 225, 100));
	// Делаем кисть для закрашивания фона активной
	SelectObject(displayHandle, outsideEllipsBrush);
	// Закрашиваем фон
	FillRect(displayHandle, &clientRect, outsideEllipsBrush);

	
	HDC bufferDC = CreateCompatibleDC(displayHandle);
	HBITMAP bufferBitmap = CreateCompatibleBitmap(displayHandle,
												  clientRect.right - clientRect.left,
												  clientRect.bottom - clientRect.top);
	SelectObject(bufferDC, bufferBitmap);

	//makePicture(displayHandle, 100 + currentTime, 100 + currentTime, 300 + currentTime, 400 + currentTime);

	if(currentTime % 100 < 50) {
		makePicture(displayHandle,
					100 + currentTime % 100,
					100 + currentTime % 100,
					300 + currentTime % 100,
					400 + currentTime % 100);
	} else {
		makePicture(displayHandle,
			100 - currentTime % 100 + 100,
			100 - currentTime % 100 + 100,
			300 - currentTime % 100 + 100,
			400 - currentTime % 100 + 100);
	}

	BitBlt(displayHandle, clientRect.left, clientRect.top, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top, displayHandle, 0, 0, SRCCOPY);

	// Удаляем использованные объекты
	DeleteObject(outsideEllipsBrush);
	DeleteDC(bufferDC);
	DeleteObject(bufferBitmap);

	EndPaint(handle, &structToPaint);
}

void COverlappedWindow::OnTimer()
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	currentTime += 1;
	InvalidateRect(handle, &clientRect, TRUE);
}

void COverlappedWindow::makePicture(HDC hdc, int a, int b, int c, int d)
{
	// Создаём кисть для рисования эллипса
	HBRUSH insideEllipsBrush = CreateSolidBrush(RGB(0, 255, 0));
	// Делаем кисть для рисования эллипса активной
	SelectObject(hdc, insideEllipsBrush);

	Ellipse(hdc, a, b, c, d);

	// Заканчиваем использование кисти
	DeleteObject(insideEllipsBrush);
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_NCCREATE:
	{
		COverlappedWindow* window = (COverlappedWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLongPtr(handle, GWLP_HINSTANCE, (LONG)window);
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);
		window->OnDestroy();
		return 0;
	}
	case WM_PAINT:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);
		window->OnPaint();
		return 0;
	}
	case WM_TIMER:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);
		window->OnTimer();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
