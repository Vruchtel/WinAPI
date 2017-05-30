#pragma once
#include <Windows.h>
#include <vector>

#include "EllipseWindow.h"

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

	// Вернуть хендл окна
	HWND GetWindowHandle() { return handle; }

protected:
	// Вызывается при получении сообщения WM_CREATE (создание окна)
	void OnCreate();

	// Вызывается при получении сообщения WM_NCCREATE (создание контекста окна)
	void OnNCCreate(HWND handle);

	// Вызывается при получении сообщения WM_DESTROY
	void OnDestroy();

	// Вызывается при получении сообщения WM_SIZE (об изменении размеров окна)
	void OnSize();

	// Вызывается при получении сообщения WM_COMMAND
	void OnCommand(WPARAM wParam, LPARAM lParam);

	// Вызывается, если получена команда акселератора
	void OnCommandAccelerator(WPARAM wParam);

private:
	HWND handle;  // хэндл окна
	std::vector<CEllipseWindow> childWindows; // дочерние дети окна
	// 0 - окно слева сверху
	// 1 - окно справа сверху
	// 2 - окно слева снизу
	// 3 - окно справа снизу

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};