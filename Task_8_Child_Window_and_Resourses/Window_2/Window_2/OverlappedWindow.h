#pragma once
#include <Windows.h>
#include <vector>

#include "EllipseWindow.h"

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

	// ������� ����� ����
	HWND GetWindowHandle() { return handle; }

protected:
	// ���������� ��� ��������� ��������� WM_CREATE (�������� ����)
	void OnCreate();

	// ���������� ��� ��������� ��������� WM_NCCREATE (�������� ��������� ����)
	void OnNCCreate(HWND handle);

	// ���������� ��� ��������� ��������� WM_DESTROY
	void OnDestroy();

	// ���������� ��� ��������� ��������� WM_SIZE (�� ��������� �������� ����)
	void OnSize();

	// ���������� ��� ��������� ��������� WM_COMMAND
	void OnCommand(WPARAM wParam, LPARAM lParam);

	// ����������, ���� �������� ������� ������������
	void OnCommandAccelerator(WPARAM wParam);

private:
	HWND handle;  // ����� ����
	std::vector<CEllipseWindow> childWindows; // �������� ���� ����
	// 0 - ���� ����� ������
	// 1 - ���� ������ ������
	// 2 - ���� ����� �����
	// 3 - ���� ������ �����

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};