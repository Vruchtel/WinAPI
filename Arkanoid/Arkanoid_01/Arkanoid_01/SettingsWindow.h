#pragma once

#include <Windows.h>
#include "resource.h"

// Класс, содержащий в себе информацию о настройках отображения
class CDisplaySettings {
public:
	DWORD rowsNum; // количество строк-кирпичиков
	DWORD ballPace; // скорость шарика
};

class CSettingsWindow {
public:
	CSettingsWindow();
	~CSettingsWindow();

	// Создать и показать экземпляр окна
	bool Create(HWND hWndParent);

	// Вернуть хендл окна
	HWND GetWindowHandle() { return handle; }

protected:
	// Вызывается при получении сообщения WM_INITDIALOG
	void OnInit(HWND _handle);

	// Вызывается при получении сообщения WM_COMMAND
	INT_PTR OnCommand(WPARAM wParam);

	// Вызывается при получении сообщения WM_HSCROLL
	void OnScroll(WPARAM wParam, LPARAM lParam);

	// Вызывается при получении сообщения WM_DESTROY
	//void OnDestroy(WPARAM wParam, LPARAM lParam);

private:
	HWND handle;  // хэндл окна

	CDisplaySettings oldSettings; // установленные настройки
	CDisplaySettings newSettings; // настройки, которые могут быть установлены

	HWND parentHandle;

	void changeSettingsTo(const CDisplaySettings &settings);

	//void setNewColorTo(DWORD &whatToChange);

	static INT_PTR _stdcall dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam);
};