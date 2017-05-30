#pragma once
#include <Windows.h>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();

	// ���������������� ����� ����
	static bool	RegisterClass();

	// ������� ��������� ����
	bool Create();

	// �������� ����
	void Show(int cmdShow);

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

private:
	HWND handle;  // ����� ����
	UINT_PTR timerId; // Id �������, ������� �������� �� ���������� ��������
	int currentTime; // ������� �����

	// ������ ��������, ������� ���� ����� ���������� (������ � ����������� �����)
	void makePicture(HDC hdc, int a, int b, int c, int d);

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};