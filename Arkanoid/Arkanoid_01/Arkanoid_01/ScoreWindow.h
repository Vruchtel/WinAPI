#pragma once
#include <Windows.h>

class CScoreWindow {
public:
	CScoreWindow();
	~CScoreWindow();

	// Зарегистрировать класс окна
	static bool	RegisterClass();

	// Создать экземпляр окна
	bool Create(HWND hWndParent);

	// Показать окно
	void Show(int cmdShow);

	// Вернуть хендл окна
	HWND GetWindowHandle() { return handle; }

	// Вернуть текущий score
	int GetScore() { return score; }

	// Установить новый score
	void SetScore(int _score) { score = _score; }

protected:
	// Вызывается при получении сообщения WM_CREATE (создание окна)
	void OnCreate();

	// Вызывается при получении сообщения WM_NCCREATE (создание контекста окна)
	void OnNCCreate(HWND handle);

	// Вызывается при получении сообщения WM_DESTROY
	void OnDestroy();

	// Вызывается при получении WM_PAINT
	void OnPaint();

	// Вызывается при получении WM_TIMER
	void OnTimer();

private:

	HWND handle;  // хэндл окна
	HWND parentHandle; // хэндл окна-родителя

	UINT_PTR timerId; // Id таймера, который сообщает об обновлении анимации
	int currentTime; // Текущее время
	
	int score; // score, который нужно вывести на экран

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};