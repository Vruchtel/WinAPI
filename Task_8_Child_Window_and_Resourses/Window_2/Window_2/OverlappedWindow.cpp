#include "OverlappedWindow.h"
#include "resource.h"

#define CHILD_COUNT 4

COverlappedWindow::COverlappedWindow()
{
	for(int childId = 0; childId < CHILD_COUNT; childId++) {
		childWindows.push_back(CEllipseWindow());
	}
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
	if (!isRegistered) {
		return false;
	}

	return true;
}

bool COverlappedWindow::Create()
{
	CreateWindowEx(0,
		L"OverlappedWindow",
		L"Big window",
		WS_OVERLAPPEDWINDOW,//WS_EX_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		GetModuleHandle(0),
		this);

	if (handle == 0) {
		return false;
	}

	return true;
}

void COverlappedWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);

	// Покажем также все дочерние окна
	for(int childId = 0; childId < CHILD_COUNT; childId++) {
		childWindows[childId].Show(cmdShow);
	}
}

void COverlappedWindow::OnCreate()
{
	// Зарегистрируем класс окна CEditControlWindow
	CEllipseWindow::RegisterClass();

	// И создадим дочерние окошки
	for(int childId = 0; childId < CHILD_COUNT; childId++) {
		childWindows[childId].Create(handle);
	}
}

void COverlappedWindow::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void COverlappedWindow::OnDestroy()
{
	PostQuitMessage(0);
}

void COverlappedWindow::OnSize()
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	
	// Ширина и высота каждого из дочерних окон
	int childWidth = (clientRect.right - clientRect.left) / 2;
	int childHeight = (clientRect.bottom - clientRect.top) / 2;

	// Центральные координаты окна
	int centerX = (clientRect.left + clientRect.right) / 2;
	int centerY = (clientRect.bottom + clientRect.top) / 2;

	// Установим местоположение каждого из дочерних окон
	SetWindowPos(childWindows[0].GetWindowHandle(), 
		HWND_TOP, clientRect.left, clientRect.top, childWidth, childHeight, NULL);
	SetWindowPos(childWindows[1].GetWindowHandle(),
		HWND_TOP, centerX, clientRect.top, childWidth, childHeight, NULL);
	SetWindowPos(childWindows[2].GetWindowHandle(),
		HWND_TOP, clientRect.left, centerY, childWidth, childHeight, NULL);
	SetWindowPos(childWindows[3].GetWindowHandle(),
		HWND_TOP, centerX, centerY, childWidth, childHeight, NULL);
}

void COverlappedWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case 1:
	{
		OnCommandAccelerator(wParam);
		break;
	}
	default:
		break;
	}
}

void COverlappedWindow::OnCommandAccelerator(WPARAM wParam)
{
	HWND focusHandle = GetFocus();
	switch (LOWORD(wParam))
	{
	case ID_ACCELERATORRIGHT:
	{
		if (focusHandle == childWindows[0].GetWindowHandle()) {
			SetFocus(childWindows[1].GetWindowHandle());
		}
		if (focusHandle == childWindows[2].GetWindowHandle()) {
			SetFocus(childWindows[3].GetWindowHandle());
		}
		break;
	}
	case ID_ACCELERATORLEFT:
	{
		if (focusHandle == childWindows[1].GetWindowHandle()) {
			SetFocus(childWindows[0].GetWindowHandle());
		}
		if (focusHandle == childWindows[3].GetWindowHandle()) {
			SetFocus(childWindows[2].GetWindowHandle());
		}
		break;
	}
	case ID_ACCELERATORDOWN:
	{
		if (focusHandle == childWindows[0].GetWindowHandle()) {
			SetFocus(childWindows[2].GetWindowHandle());
		}
		if (focusHandle == childWindows[1].GetWindowHandle()) {
			SetFocus(childWindows[3].GetWindowHandle());
		}
		break;
	}
	case ID_ACCELERATORUP:
	{
		if (focusHandle == childWindows[2].GetWindowHandle()) {
			SetFocus(childWindows[0].GetWindowHandle());
		}
		if (focusHandle == childWindows[3].GetWindowHandle()) {
			SetFocus(childWindows[1].GetWindowHandle());
		}
		break;
	}
	default:
		break;
	}
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);
	switch (message) {
	case WM_CREATE:
	{
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
	case WM_SIZE:
	{
		window->OnSize();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_COMMAND:
	{
		window->OnCommand(wParam, lParam);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		window->OnDestroy();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
