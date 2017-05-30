#include "EllipseWindow.h"

CEllipseWindow::CEllipseWindow()
{
	currentTime = 0;
}

CEllipseWindow::~CEllipseWindow() {}

bool CEllipseWindow::RegisterClass()
{
	WNDCLASSEX windowClass;

	// Заполним структуру соответствующими данными
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = CEllipseWindow::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"EllipseWindow";

	ATOM isRegistered = RegisterClassEx(&windowClass);
	if (!isRegistered) {
		return false;
	}

	return true;
}

bool CEllipseWindow::Create(HWND hWndParent)
{
	CreateWindowEx(0,
		L"EllipseWindow",
		L"Small window",
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

void CEllipseWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

void CEllipseWindow::OnCreate()
{
	const int millisecondsToChange = 80;
	timerId = SetTimer(handle, NULL, millisecondsToChange, NULL);
}

void CEllipseWindow::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void CEllipseWindow::OnDestroy()
{
	KillTimer(handle, timerId);
	PostQuitMessage(0);
}

void CEllipseWindow::OnPaint()
{
	PAINTSTRUCT structToPaint;
	HDC displayHandle = BeginPaint(handle, &structToPaint);

	RECT clientRect;  // структура, в которой будут координаты клиентской области
					  // Получим координаты клиентской области
	GetClientRect(handle, &clientRect);

	// Создаём кисть для закрашивания фона
	HBRUSH outsideEllipsBrush = CreateSolidBrush(RGB(250, 225, 100));
	// Делаем кисть для закрашивания фона активной
	HGDIOBJ replacedBrush = SelectObject(displayHandle, outsideEllipsBrush);
	// Закрашиваем фон
	FillRect(displayHandle, &clientRect, outsideEllipsBrush);

	HDC bufferDC = CreateCompatibleDC(displayHandle);
	HBITMAP bufferBitmap = CreateCompatibleBitmap(displayHandle,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top);
	HGDIOBJ replacedBitmap = SelectObject(bufferDC, bufferBitmap);

	if(handle == GetFocus()) {
		currentTime += 1;
	}
	if (currentTime % 50 < 25) {
		makePicture(displayHandle,
			50 + currentTime % 50,
			50 + currentTime % 50,
			150 + currentTime % 50,
			150 + currentTime % 50);
	}
	else {
		makePicture(displayHandle,
			50 - currentTime % 50 + 50,
			50 - currentTime % 50 + 50,
			150 - currentTime % 50 + 50,
			150 - currentTime % 50 + 50);
	}

	

	BitBlt(displayHandle, clientRect.left, clientRect.top, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top, displayHandle, 0, 0, SRCCOPY);

	// Удаляем использованные объекты
	SelectObject(displayHandle, replacedBrush);
	DeleteObject(outsideEllipsBrush);
	SelectObject(bufferDC, replacedBitmap);
	DeleteObject(bufferBitmap);
	DeleteDC(bufferDC);

	EndPaint(handle, &structToPaint);
}

void CEllipseWindow::OnTimer()
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	//currentTime += 1;
	InvalidateRect(handle, &clientRect, TRUE);
}

void CEllipseWindow::OnLButtonDown()
{
	SetFocus(handle);
}

void CEllipseWindow::makePicture(HDC hdc, int left, int top, int right, int bottom)
{
	// Создаём кисть для рисования эллипса
	HBRUSH insideEllipsBrush;

	if(handle == GetFocus()) {
		insideEllipsBrush = CreateSolidBrush(RGB(255, 0, 0));
	} else {
		insideEllipsBrush = CreateSolidBrush(RGB(0, 255, 0));
	}
		
	// Делаем кисть для рисования эллипса активной
	SelectObject(hdc, insideEllipsBrush);

	Ellipse(hdc, left, top, right, bottom);

	// Заканчиваем использование кисти
	DeleteObject(insideEllipsBrush);
}

LRESULT CEllipseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);
	switch (message) {
	case WM_CREATE:
	{
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_NCCREATE:
	{
		CEllipseWindow* window = (CEllipseWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLongPtr(handle, GWLP_HINSTANCE, (LONG)window);
		window->OnNCCreate(handle);
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
	case WM_LBUTTONDOWN:
	{
		window->OnLButtonDown();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
