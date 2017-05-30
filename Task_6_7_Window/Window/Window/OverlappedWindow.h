#pragma once
#include <Windows.h>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();

	// Зарегистрировать класс окна
	static bool	RegisterClass();

	// Создать экземпляр окна
	bool Create();

	// Показать окно
	void Show(int cmdShow);

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
	UINT_PTR timerId; // Id таймера, который сообщает об обновлении анимации
	int currentTime; // Текущее време

	// Создаёт картинку, которую надо будет нарисовать (эллипс в определённой точке)
	void makePicture(HDC hdc, int a, int b, int c, int d);

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};