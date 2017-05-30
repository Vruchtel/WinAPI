#include <atlstr.h> // CString

#include "OverlappedWindow.h"
#include "resource.h"

//#define MAX_PATH 256

COverlappedWindow::COverlappedWindow()
{
	childEditControlWindow = CEditControlWindow();
	dialogWindow = CDialogWindow();
	textWasEntered = false;
	activeBrush = NULL;
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
	windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	ATOM isRegistered = RegisterClassEx(&windowClass);
	if (!isRegistered) {
		return false;
	}

	return true;
}

bool COverlappedWindow::Create()
{
	CreateWindowEx(WS_EX_LAYERED,
		L"OverlappedWindow",
		L"Main window", // что-то непонятное происходит с названием!!!
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		GetModuleHandle(0),
		this);

	SetLayeredWindowAttributes(handle, 0, 255, LWA_COLORKEY);
	transparency = 255;

	if (handle == 0) {
		return false;
	}

	return true;
}

void COverlappedWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
	childEditControlWindow.Show(cmdShow);
}

void COverlappedWindow::OnCreate()
{
	fontColor = RGB(0, 0, 0);
	backgroundColor = RGB(255, 255, 255);

	childEditControlWindow.Create(handle);
	windowMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));

	LOGFONT logFont;
	logFont.lfHeight = 18;
	logFont.lfWidth = NULL;
	logFont.lfEscapement = NULL;
	logFont.lfOrientation = NULL;
	logFont.lfWeight = FW_DONTCARE;
	logFont.lfItalic = FALSE;
	logFont.lfUnderline = FALSE;
	logFont.lfStrikeOut = FALSE;
	logFont.lfCharSet = RUSSIAN_CHARSET;
	logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;

	HFONT hFont = CreateFontIndirect(&logFont);

	SendMessage(childEditControlWindow.GetWindowHandle(), WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
}

void COverlappedWindow::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void COverlappedWindow::OnSize()
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	SetWindowPos(childEditControlWindow.GetWindowHandle(), 
		HWND_TOP, clientRect.left, clientRect.top, 
		clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, 0);
}

void COverlappedWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case 0:
	{
		OnCommandMenu(wParam);
		break;
	}
	case 1:
	{
		OnCommandAccelerator(wParam);
		break;
	}
	case EN_CHANGE:
	{
		textWasEntered = true;
	}
	default:
		break;
	}
}

void COverlappedWindow::OnCommandMenu(WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case ID_VIEW_SETTINGS:
	{
		dialogWindow.Create(handle);
		break;
	}
	}
}

void COverlappedWindow::OnCommandAccelerator(WPARAM wParam)
{
	HWND focusHandle = GetFocus();
	switch (LOWORD(wParam))
	{
	case ID_ACCELERATORRIGHT:
	{
		break;
	}
	case ID_ACCELERATORLEFT:
	{
		break;
	}
	case ID_ACCELERATORDOWN:
	{
		break;
	}
	case ID_ACCELERATORUP:
	{
		break;
	}
	default:
		break;
	}
}

 bool COverlappedWindow::OnClose()
{
	if(textWasEntered) {
		int messageBoxCode = MessageBox(handle,
			CString("Сохранить введённый текст?"), 
			CString("Закрыть приложение"),
			MB_YESNOCANCEL);

		// Да - сохранить и закрыть, Нет - не сохранять и закрыть, Отмена - не закрывать

		switch (messageBoxCode)
		{
		case IDYES:
		{
			// сохранить текст
			saveText();
			return true; // закрыть
		}
		case IDNO:
		{
			return true; // закрыть
		}
		case IDCANCEL:
		{
			return false; // не закрывать
		}
		}
	}
	return true;
}

void COverlappedWindow::OnDestroy()
{
	if(activeBrush != NULL) {
		DeleteObject(activeBrush);
	}
	DestroyMenu(windowMenu);

	HFONT currentFont = reinterpret_cast<HFONT>(SendMessage(childEditControlWindow.GetWindowHandle(), WM_GETFONT, 0, 0));
	DeleteObject(currentFont);

	PostQuitMessage(0);
}

LRESULT COverlappedWindow::OnControlColorEdit(HDC hdc)
{
	if (activeBrush != NULL) {
		DeleteObject(activeBrush);
	}

	SetTextColor(hdc, fontColor);
	SetBkColor(hdc, backgroundColor);

	activeBrush = CreateSolidBrush(backgroundColor);
	return reinterpret_cast<LRESULT>(activeBrush);
}

void COverlappedWindow::saveText()
{
	// Получим длину текста edit-контрола
	int textLength = SendMessage(childEditControlWindow.GetWindowHandle(),
		WM_GETTEXTLENGTH, NULL, NULL);

	// Получим сам текст edit-контрола
	wchar_t *textFromECWin = new wchar_t[textLength];
	LRESULT gettingTextResult = SendMessage(childEditControlWindow.GetWindowHandle(),
		WM_GETTEXT, textLength, reinterpret_cast<LPARAM>(textFromECWin));

	// Сохранение текста в файл
	char savingFileName[MAX_PATH] = "";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = static_cast<LPCWSTR>(L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
	ofn.lpstrFile = reinterpret_cast<LPWSTR>(savingFileName);
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = static_cast<LPCWSTR>(L"txt");

	if(GetSaveFileName(&ofn)) {
		HANDLE createdFile = CreateFile(reinterpret_cast<LPCWSTR>(savingFileName),
			GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		WriteFile(createdFile, textFromECWin, textLength * sizeof(wchar_t), 0, 0);
		CloseHandle(createdFile);
	}

	delete[] textFromECWin;

	////////////////////////////////////
	// Отладочный MessageBox
	MessageBox(handle,
		CString("Текст сохранён в файл"),
		CString("Сохранение прошло успешно"),
		MB_OK);
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);
	switch (message) {
	case WM_CREATE:
	{
		window->OnCreate();
		return 0;
	}
	case WM_NCCREATE:
	{
		COverlappedWindow* window = (COverlappedWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLongPtr(handle, GWLP_HINSTANCE, (LONG)window);
		window->OnNCCreate(handle);
		return TRUE;
	}
	case WM_SIZE:
	{
		window->OnSize();
		return 0;
	}
	case WM_COMMAND:
	{
		window->OnCommand(wParam, lParam);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CLOSE:
	{	
		if(window->OnClose()) { // нужно закрыть окно
			return DefWindowProc(handle, message, wParam, lParam);
		} else {
			return 0;
		}
	}
	case WM_DESTROY:
	{
		window->OnDestroy();
		return 0;
	}
	case WM_CTLCOLOREDIT:
	{
		return window->OnControlColorEdit(reinterpret_cast<HDC>(wParam));
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
