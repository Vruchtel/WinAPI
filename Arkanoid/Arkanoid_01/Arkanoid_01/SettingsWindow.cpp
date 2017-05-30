#include "SettingsWindow.h"

#include "ArkanoidWindow.h"
#include <Commctrl.h>

CSettingsWindow::CSettingsWindow()
{
	handle = 0;
	parentHandle = 0;

	ZeroMemory(&oldSettings, sizeof(CDisplaySettings));
	ZeroMemory(&newSettings, sizeof(CDisplaySettings));
}

CSettingsWindow::~CSettingsWindow()
{
}

bool CSettingsWindow::Create(HWND hWndParent)
{
	parentHandle = hWndParent;
	CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE);

	CreateDialogParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_SETTINGS), hWndParent, dialogProc,
		reinterpret_cast<LPARAM>(this));
	ShowWindow(handle, SW_SHOW);

	if(handle == 0) {
		return false;
	}
	return true;
}

void CSettingsWindow::OnInit(HWND _handle)
{
	handle = _handle;
	CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(parentHandle, GWLP_HINSTANCE);

	// «ададим границы дл€ изменени€ количества строк-кирпичиков
	SendMessage(GetDlgItem(handle, IDC_ROWS_NUM), TBM_SETRANGE, TRUE, MAKELONG(1, 12));
	// «ададим границы дл€ изменени€ скорости шарика
	SendMessage(GetDlgItem(handle, IDC_BALL_PACE), TBM_SETRANGE, TRUE, MAKELONG(2, 35));

	// получим текущие установленные свойства
	newSettings.rowsNum = parentWindow->GetBricksRowsNum();
	newSettings.ballPace = parentWindow->GetDeltaTime();

	oldSettings = newSettings;

	SendMessage(GetDlgItem(handle, IDC_ROWS_NUM), TBM_SETPOS, true, newSettings.rowsNum);
	SendMessage(GetDlgItem(handle, IDC_BALL_PACE), TBM_SETPOS, true, newSettings.ballPace);	
}

INT_PTR CSettingsWindow::OnCommand(WPARAM wParam)
{
	switch LOWORD(wParam) {
	case ID_OK:
	{
		CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(parentHandle, GWLP_HINSTANCE);
		changeSettingsTo(newSettings);
		EndDialog(handle, wParam);
		parentWindow->Restart();
		return TRUE;
	}
	case ID_CANCEL:
	{
		changeSettingsTo(oldSettings);
		EndDialog(handle, wParam);
		return TRUE;
	}
	}

	return FALSE;
}

void CSettingsWindow::OnScroll(WPARAM wParam, LPARAM lParam)
{
	HWND cmdHandle = reinterpret_cast<HWND>(lParam);

	if (cmdHandle == GetDlgItem(handle, IDC_ROWS_NUM)) {
		newSettings.rowsNum = SendMessage(GetDlgItem(handle, IDC_ROWS_NUM), TBM_GETPOS, 0, 0);
	}
	else if (cmdHandle == GetDlgItem(handle, IDC_BALL_PACE)) {
		newSettings.ballPace = SendMessage(GetDlgItem(handle, IDC_BALL_PACE), TBM_GETPOS, 0, 0);
	}
}

void CSettingsWindow::changeSettingsTo(const CDisplaySettings & settings)
{
	CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(parentHandle, GWLP_HINSTANCE);

	parentWindow->SetBrickRowsNum(settings.rowsNum);
	parentWindow->SetDeltaTime(settings.ballPace);
}

INT_PTR CSettingsWindow::dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	CSettingsWindow* dialog = reinterpret_cast<CSettingsWindow*>(GetWindowLongPtr(handleDlg, GWLP_HINSTANCE));

	switch (message) {
	case WM_INITDIALOG:
	{
		CSettingsWindow* dialog = reinterpret_cast<CSettingsWindow*>(lParam);
		SetWindowLongPtr(handleDlg, GWLP_HINSTANCE, (LONG)dialog);
		dialog->OnInit(handleDlg);
		return TRUE;
	}
	case WM_COMMAND:
	{
		return dialog->OnCommand(wParam);
	}
	case WM_HSCROLL:
	{
		dialog->OnScroll(wParam, lParam);
		return FALSE;
	}
	/*case WM_DESTROY:
	{
	dialog->OnDestroy(wParam, lParam);
	return FALSE;
	}*/
	default:
		return FALSE;
	}
}

