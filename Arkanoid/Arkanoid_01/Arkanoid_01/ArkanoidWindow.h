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

	// Зарегистрировать класс окна
	static bool	RegisterClass();

	// Создать экземпляр окна
	bool Create();

	// Показать окно
	void Show(int cmdShow);

	// Вернуть хендл окна
	HWND GetWindowHandle() { return handle; }
	
	// Вызывается, когда пользователь нажимает на кнопку "Начать заново"
	void Restart();

	// Вернуть количество строк-кирпичиков
	int GetBricksRowsNum() { return brickRowsNum; }

	// Вернуть скорость шарика
	int GetDeltaTime() { return deltaTime; }

	// Вернуть handle окна настроек
	HWND GetSettingsWindowHandle() { return settingsWindow.GetWindowHandle(); }

	// Установить новое количество строк-кирпичиков
	void SetBrickRowsNum(int newBricksRowsNum);

	// Установить новую скорость шарика
	void SetDeltaTime(int newDeltaTime) { deltaTime = newDeltaTime; }

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

	// Вызывается при получении сообщения WM_SIZE
	void OnSize();

	// Вызывается при получении сообщения WM_COMMAND
	void OnCommand(WPARAM wParam, LPARAM lParam);

	// Вызывается, если получена команда акселератора
	void OnCommandAccelerator(WPARAM wParam);

	// Вызвается, если получена команда акселератора
	void OnCommandMenu(WPARAM wParam);

private:
	HWND handle;  // хэндл окна
	UINT_PTR timerId; // Id таймера, который сообщает об обновлении анимации
	int currentTime; // Текущее время

	int deltaTime; // скорость движения

	// Координаты шарика в текущий момент времени
	int ballLeft;
	int ballRight;
	int ballTop;
	int ballBottom;

	// Координаты платформы в текущий момент времени
	int platformLeft;
	int platformRight;
	int platformTop;
	int platformBottom;

	int ballDirectionOnX; // Направление движения шарика по оси x (1 или -1)
	int ballDirectionOnY; // Направление движения шарика по оси y (1 или -1)

	int brickRowsNum;
	int bricksColsNum;

	CBricks bricks; // Кирпичики, которые должен сбивать шарик

	int timesPaceChanged; // количество раз, которое была изменена скорость движения шарика
	// нечётн - ускорение, чётн - замедление

	CScoreWindow scoreWindow; // Окошко, в котором отображаются набранные очки

	CEndGameDialog endGameDialog; // Окно, открывающееся в случае окончания игры

	bool isGameFinished; // Флажок, показывающий, что игра окончена

	int platformLastDirection; // +1 - платформа последний раз двигалась вправо
							   // -1 - платформа последний раз двигалась влево	
							   // 0 - платорма давно не двигалась

	int timeBeforeForgetness; // Время в обновлениях экрана, до того, как будет забыто последнее
							  // направление движения платформы

	HMENU menu; // хэндл меню окна

	CSettingsWindow settingsWindow; // Диалоговое окно настроек

	CBeginGameDialog beginGameDialog; // Диалоговое окно, открывающееся перед началом игры

	/*private methods*/

	// Для текущего размера окна определяет текущее местоположение шарика
	// и задаёт значение координат в соответствующих полях класса
	void calculateBallCoordinates(const RECT &clientRect);

	// Определяет направление движения шарика
	void calculateBallDirection(const RECT &clientRect);

	// Определяет текущее положение каждого из кирпичиков
	void calculateBricksCoordinates(const RECT &clientRect);

	// Помещает шарик в нужное место в окне
	void placeBall(HDC hdc, const RECT &clientRect);

	// Помещает платформу в нужное место в окне
	void placePlatform(HDC hdc, const RECT &clientRect);

	// Помещает кирпичики в нужное место в окне
	void placeBricks(HDC hdc, const RECT &clientRect);

	// Определяет результат взаимодействия шарика с кирпичиками
	void InteractionOfBricksAndBall(const RECT &clientRect);

	void ChangeBallDirectionAfterBrickKill(const RECT &clientRect);

	static LRESULT _stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};