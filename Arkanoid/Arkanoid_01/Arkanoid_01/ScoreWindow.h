#pragma once
#include <Windows.h>

class CScoreWindow {
public:
	CScoreWindow();
	~CScoreWindow();

	// ���������������� ����� ����
	static bool	RegisterClass();

	// ������� ��������� ����
	bool Create(HWND hWndParent);

	// �������� ����
	void Show(int cmdShow);

	// ������� ����� ����
	HWND GetWindowHandle() { return handle; }

	// ������� ������� score
	int GetScore() { return score; }

	// ���������� ����� score
	void SetScore(int _score) { score = _score; }

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
	HWND parentHandle; // ����� ����-��������

	UINT_PTR timerId; // Id �������, ������� �������� �� ���������� ��������
	int currentTime; // ������� �����
	
	int score; // score, ������� ����� ������� �� �����

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};