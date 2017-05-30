#pragma once
#include <Windows.h>

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	// ���������������� ����� ����
	static bool	RegisterClass();

	// ������� ��������� ����
	bool Create(HWND hWndParent);

	// �������� ����
	void Show(int cmdShow);

	// ������� ����� ����
	HWND GetWindowHandle() { return handle; }

protected:
	// ���������� ��� ��������� ��������� WM_CREATE (�������� ����)
	void OnCreate();

	// ���������� ��� ��������� ��������� WM_NCCREATE (�������� ��������� ����)
	void OnNCCreate(HWND handle);

	// ���������� ��� ��������� ��������� WM_DESTROY
	void OnDestroy();

	// ���������� ��� ��������� WM_PAINT
	void OnPaint();

	// ���������� ��� ��������� WM_TIMER
	void OnTimer();

	// ���������� ��� ��������� ��������� WM_LBUTTONDOWN
	void OnLButtonDown();

private:
	HWND handle;  // ����� ����
	UINT_PTR timerId; // Id �������, ������� �������� �� ���������� ��������
	int currentTime; // ������� �����

	// ������ ��������, ������� ���� ����� ���������� (������ � ����������� �����)
	void makePicture(HDC hdc, int left, int top, int right, int bottom);

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};