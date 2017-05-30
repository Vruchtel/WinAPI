#pragma once
#include <Windows.h>

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	// Зарегистрировать класс окна
	static bool	RegisterClass();

	// Создать экземпляр окна
	bool Create(HWND hWndParent);

	// Показать окно
	void Show(int cmdShow);

	// Вернуть хендл окна
	HWND GetWindowHandle() { return handle; }

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

	// Вызывается при получении сообщения WM_LBUTTONDOWN
	void OnLButtonDown();

private:
	HWND handle;  // хэндл окна
	UINT_PTR timerId; // Id таймера, который сообщает об обновлении анимации
	int currentTime; // Текущее время

	// Создаёт картинку, которую надо будет нарисовать (эллипс в определённой точке)
	void makePicture(HDC hdc, int left, int top, int right, int bottom);

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};