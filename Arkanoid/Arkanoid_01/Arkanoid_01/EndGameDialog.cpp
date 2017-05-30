#include "EndGameDilog.h"

#include "ArkanoidWindow.h"

CEndGameDialog::CEndGameDialog()
{
	handle = 0;
	parentHandle = 0;
}

CEndGameDialog::~CEndGameDialog() {}

bool CEndGameDialog::Create(HWND hWndParent)
{
	parentHandle = hWndParent;
	CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE);

	CreateDialogParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_END_DIALOG), hWndParent, dialogProc,
		reinterpret_cast<LPARAM>(this));
	ShowWindow(handle, SW_SHOW);

	if (handle == 0) {
		return false;
	}

	return true;
}

void CEndGameDialog::OnInit(HWND _handle)
{
	handle = _handle;

	CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(parentHandle, GWLP_HINSTANCE);
}

INT_PTR CEndGameDialog::OnCommand(WPARAM wParam)
{
	switch LOWORD(wParam) {
	case IDC_RESTART:
	{
		// restart (спец. функция в ArkanoidWindow)
		CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(parentHandle, GWLP_HINSTANCE);
		parentWindow->Restart();
		EndDialog(handle, wParam);

		return TRUE;
	}
	case IDC_EXIT:
	{
		SendMessage(parentHandle, WM_CLOSE, 0, 0);
		return TRUE;
	}
	}
	return FALSE;
	
}

INT_PTR CEndGameDialog::dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	CEndGameDialog* dialog = reinterpret_cast<CEndGameDialog*>(GetWindowLongPtr(handleDlg, GWLP_HINSTANCE));

	switch (message) {
	case WM_INITDIALOG:
	{
		dialog = reinterpret_cast<CEndGameDialog*>(lParam);
		SetWindowLongPtr(handleDlg, GWLP_HINSTANCE, (LONG)dialog);
		dialog->OnInit(handleDlg);
		return TRUE;
	}
	case WM_COMMAND:
	{
		return dialog->OnCommand(wParam);
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


