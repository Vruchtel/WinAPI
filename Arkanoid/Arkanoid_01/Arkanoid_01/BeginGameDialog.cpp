#include "BeginGameDialog.h"

#include "ArkanoidWindow.h"

CBeginGameDialog::CBeginGameDialog()
{
	handle = 0;
	parentHandle = 0;
}

CBeginGameDialog::~CBeginGameDialog() {}

bool CBeginGameDialog::Create(HWND hWndParent)
{
	parentHandle = hWndParent;
	CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE);

	CreateDialogParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_BEGIN_DIALOG), hWndParent, dialogProc,
		reinterpret_cast<LPARAM>(this));
	ShowWindow(handle, SW_SHOW);

	if (handle == 0) {
		return false;
	}

	return true;
}

void CBeginGameDialog::OnInit(HWND _handle)
{
	handle = _handle;
}

INT_PTR CBeginGameDialog::OnCommand(WPARAM wParam)
{
	switch LOWORD(wParam) {
	case IDC_START_GAME:
	{
		CArkanoidWindow* parentWindow = (CArkanoidWindow*)GetWindowLongPtr(parentHandle, GWLP_HINSTANCE);
		EndDialog(handle, wParam);
		parentWindow->Restart();
		return TRUE;
	}
	case IDC_CALL_EXIT:
	{
		SendMessage(parentHandle, WM_CLOSE, 0, 0);
		return TRUE;
	}
	}
	return FALSE;

}

INT_PTR CBeginGameDialog::dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	CBeginGameDialog* dialog = reinterpret_cast<CBeginGameDialog*>(GetWindowLongPtr(handleDlg, GWLP_HINSTANCE));

	switch (message) {
	case WM_INITDIALOG:
	{
		dialog = reinterpret_cast<CBeginGameDialog*>(lParam);
		SetWindowLongPtr(handleDlg, GWLP_HINSTANCE, (LONG)dialog);
		dialog->OnInit(handleDlg);
		return TRUE;
	}
	case WM_COMMAND:
	{
		return dialog->OnCommand(wParam);
	}
	default:
		return FALSE;
	}
}


