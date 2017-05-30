#pragma once
#include <Windows.h>

#include "Bricks.h"
#include "ScoreWindow.h"
#include "EndGameDilog.h"
#include "SettingsWindow.h"
#include "BeginGameDialog.h"

class CArkanoidWindow {
public:
	CArkanoidWindow();
	~CArkanoidWindow();

	// ���������������� ����� ����
	static bool	RegisterClass();

	// ������� ��������� ����
	bool Create();

	// �������� ����
	void Show(int cmdShow);

	// ������� ����� ����
	HWND GetWindowHandle() { return handle; }
	
	// ����������, ����� ������������ �������� �� ������ "������ ������"
	void Restart();

	// ������� ���������� �����-����������
	int GetBricksRowsNum() { return brickRowsNum; }

	// ������� �������� ������
	int GetDeltaTime() { return deltaTime; }

	// ������� handle ���� ��������
	HWND GetSettingsWindowHandle() { return settingsWindow.GetWindowHandle(); }

	// ���������� ����� ���������� �����-����������
	void SetBrickRowsNum(int newBricksRowsNum);

	// ���������� ����� �������� ������
	void SetDeltaTime(int newDeltaTime) { deltaTime = newDeltaTime; }

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

	// ���������� ��� ��������� ��������� WM_SIZE
	void OnSize();

	// ���������� ��� ��������� ��������� WM_COMMAND
	void OnCommand(WPARAM wParam, LPARAM lParam);

	// ����������, ���� �������� ������� ������������
	void OnCommandAccelerator(WPARAM wParam);

	// ���������, ���� �������� ������� ������������
	void OnCommandMenu(WPARAM wParam);

private:
	HWND handle;  // ����� ����
	UINT_PTR timerId; // Id �������, ������� �������� �� ���������� ��������
	int currentTime; // ������� �����

	int deltaTime; // �������� ��������

	// ���������� ������ � ������� ������ �������
	int ballLeft;
	int ballRight;
	int ballTop;
	int ballBottom;

	// ���������� ��������� � ������� ������ �������
	int platformLeft;
	int platformRight;
	int platformTop;
	int platformBottom;

	int ballDirectionOnX; // ����������� �������� ������ �� ��� x (1 ��� -1)
	int ballDirectionOnY; // ����������� �������� ������ �� ��� y (1 ��� -1)

	int brickRowsNum;
	int bricksColsNum;

	CBricks bricks; // ���������, ������� ������ ������� �����

	int timesPaceChanged; // ���������� ���, ������� ���� �������� �������� �������� ������
	// ������ - ���������, ���� - ����������

	CScoreWindow scoreWindow; // ������, � ������� ������������ ��������� ����

	CEndGameDialog endGameDialog; // ����, ������������� � ������ ��������� ����

	bool isGameFinished; // ������, ������������, ��� ���� ��������

	int platformLastDirection; // +1 - ��������� ��������� ��� ��������� ������
							   // -1 - ��������� ��������� ��� ��������� �����	
							   // 0 - �������� ����� �� ���������

	int timeBeforeForgetness; // ����� � ����������� ������, �� ����, ��� ����� ������ ���������
							  // ����������� �������� ���������

	HMENU menu; // ����� ���� ����

	CSettingsWindow settingsWindow; // ���������� ���� ��������

	CBeginGameDialog beginGameDialog; // ���������� ����, ������������� ����� ������� ����

	/*private methods*/

	// ��� �������� ������� ���� ���������� ������� �������������� ������
	// � ����� �������� ��������� � ��������������� ����� ������
	void calculateBallCoordinates(const RECT &clientRect);

	// ���������� ����������� �������� ������
	void calculateBallDirection(const RECT &clientRect);

	// ���������� ������� ��������� ������� �� ����������
	void calculateBricksCoordinates(const RECT &clientRect);

	// �������� ����� � ������ ����� � ����
	void placeBall(HDC hdc, const RECT &clientRect);

	// �������� ��������� � ������ ����� � ����
	void placePlatform(HDC hdc, const RECT &clientRect);

	// �������� ��������� � ������ ����� � ����
	void placeBricks(HDC hdc, const RECT &clientRect);

	// ���������� ��������� �������������� ������ � �����������
	void InteractionOfBricksAndBall(const RECT &clientRect);

	void ChangeBallDirectionAfterBrickKill(const RECT &clientRect);

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};