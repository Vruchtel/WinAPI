#include "ArkanoidWindow.h"
#include "resource.h"

#include <algorithm>

#define BALL_RADIUS 12 // или оставить 9

#define PLATFORM_HEIGHT 15
#define PLATFORM_LENGTH 100

#define BRICK_HEIGHT 10  // высота каждого кирпичика

#define SCORE_WINDOW_WIDTH 80
#define SCORE_WINDOW_HEIGHT 28

#define DEFAULT_DELTA_TIME 5

#define DEFAULT_BRICKS_ROWS_NUM 4
#define BRICKS_COLS_NUM 9

CArkanoidWindow::CArkanoidWindow()
	: bricks(DEFAULT_BRICKS_ROWS_NUM, BRICKS_COLS_NUM)
{
	currentTime = 0;
	deltaTime = DEFAULT_DELTA_TIME;

	ballLeft = -10;
	ballRight = -10;
	ballTop = -10;
	ballBottom = -10;

	platformLeft = -10;
	platformRight = -10;
	platformTop = -10;
	platformBottom = -10;

	ballDirectionOnX = 1;
	ballDirectionOnY = -1;

	brickRowsNum = DEFAULT_BRICKS_ROWS_NUM;
	bricksColsNum = BRICKS_COLS_NUM;

	timesPaceChanged = 0;

	scoreWindow = CScoreWindow();
	endGameDialog = CEndGameDialog();
	settingsWindow = CSettingsWindow();
	beginGameDialog = CBeginGameDialog();

	isGameFinished = false;

	platformLastDirection = 0;
	timeBeforeForgetness = 0;
}

CArkanoidWindow::~CArkanoidWindow() {}

bool CArkanoidWindow::RegisterClass()
{
	WNDCLASSEX windowClass;

	// Заполним структуру соответствующими данными
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = CArkanoidWindow::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"ArkanoidWindow";
	windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	ATOM isRegistered = RegisterClassEx(&windowClass);
	if (!isRegistered) {
		return false;
	}

	return true;
}

bool CArkanoidWindow::Create()
{
	CreateWindowEx(0,
		L"ArkanoidWindow",
		L"Игра Арканоид",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		475,
		510,
		0,
		0,
		GetModuleHandle(0),
		this);

	if (handle == 0) {
		return false;
	}

	return true;
}

void CArkanoidWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);

	// Покажем также ScoreWindow
	scoreWindow.Show(cmdShow);
}

/*protected methods*/

void CArkanoidWindow::Restart()
{

	ballLeft = -10;
	ballRight = -10;
	ballTop = -10;
	ballBottom = -10;

	platformLeft = -10;
	platformRight = -10;
	platformTop = -10;
	platformBottom = -10;

	ballDirectionOnX = 1;
	ballDirectionOnY = -1;

	scoreWindow.SetScore(0);

	isGameFinished = false;

	std::vector<std::vector<int>> tmpE(brickRowsNum, std::vector<int>(bricksColsNum, 1));
	bricks.Existense = tmpE;

	platformLastDirection = 0;
	timeBeforeForgetness = 0;
}

void CArkanoidWindow::SetBrickRowsNum(int newBricksRowsNum)
{
	brickRowsNum = newBricksRowsNum;
	bricks = CBricks(brickRowsNum, bricksColsNum);
}

void CArkanoidWindow::OnCreate()
{
	// Зарегистрируем класс окна scoreWindow
	CScoreWindow::RegisterClass();

	// И создадим его
	scoreWindow.Create(handle);

	// Загружаем меню
	menu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));

	const int millisecondsToChange = 80;
	//const int millisecondsToChange = 70;
	timerId = SetTimer(handle, NULL, millisecondsToChange, NULL);

	// покажем также диалог-приветствие
	isGameFinished = true;
	beginGameDialog.Create(handle);
}

void CArkanoidWindow::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void CArkanoidWindow::OnDestroy()
{
	DestroyMenu(menu);
	KillTimer(handle, timerId);
	PostQuitMessage(0);
}

void CArkanoidWindow::OnPaint()
{
	if(isGameFinished) {
		return;
	}
	PAINTSTRUCT structToPaint;
	HDC displayHandle = BeginPaint(handle, &structToPaint);

	RECT clientRect;  // структура, в которой будут координаты клиентской области
					  // Получим координаты клиентской области
	GetClientRect(handle, &clientRect);

	// Создаём кисть для закрашивания фона
	HBRUSH outsideEllipsBrush = CreateSolidBrush(RGB(250, 225, 100));
	// Делаем кисть для закрашивания фона активной
	SelectObject(displayHandle, outsideEllipsBrush);
	// Закрашиваем фон
	FillRect(displayHandle, &clientRect, outsideEllipsBrush);

	HDC bufferDC = CreateCompatibleDC(displayHandle);
	HBITMAP bufferBitmap = CreateCompatibleBitmap(displayHandle,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top);
	SelectObject(bufferDC, bufferBitmap);

	// Помещаем шарик на его законное место
	placeBall(displayHandle, clientRect);

	// Помещаем платформу на её законное место
	placePlatform(displayHandle, clientRect);

	// Помещаем кирпичики на их законные места
	placeBricks(displayHandle, clientRect);

	BitBlt(displayHandle, clientRect.left, clientRect.top, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top, displayHandle, 0, 0, SRCCOPY);

	// Удаляем использованные объекты
	DeleteObject(outsideEllipsBrush);
	DeleteDC(bufferDC);
	DeleteObject(bufferBitmap);

	EndPaint(handle, &structToPaint);
}

void CArkanoidWindow::OnTimer()
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	//currentTime += deltaTime;
	currentTime += 1;
	InvalidateRect(handle, &clientRect, TRUE);
}

void CArkanoidWindow::OnSize()
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);

	SetWindowPos(scoreWindow.GetWindowHandle(),
		HWND_TOP,
		clientRect.right - SCORE_WINDOW_WIDTH, 
		clientRect.top,
		SCORE_WINDOW_WIDTH,
		SCORE_WINDOW_HEIGHT,
		NULL);
}

void CArkanoidWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case 0:
	{
		OnCommandMenu(wParam);
		break;
	}
	case 1:
	{
		OnCommandAccelerator(wParam);
		break;
	}
	default:
		break;
	}
}

void CArkanoidWindow::OnCommandAccelerator(WPARAM wParam)
{
	RECT clientRect;
	GetClientRect(handle, &clientRect);

	switch (LOWORD(wParam))
	{
	case ID_LEFT:
	{
		if(platformLeft > clientRect.left + 1) {
			platformLeft -= deltaTime;
			platformRight -= deltaTime;

			timeBeforeForgetness = 10; ///////////////////////////////////
			platformLastDirection = -1;
		}
		break;
	}
	case ID_RIGHT:
	{
		if(platformRight < clientRect.right - 1) {
			platformLeft += deltaTime;
			platformRight += deltaTime;

			timeBeforeForgetness = 10; ///////////////////////////////////
			platformLastDirection = 1;
		}
		break;
	}
	case ID_SPACE:
	{
		timesPaceChanged += 1;
		if(timesPaceChanged % 2 == 0) {
			deltaTime /= 2;
		} else {
			deltaTime *= 2;
		}

		break;
	}
	default:
		break;
	}
	
}

void CArkanoidWindow::OnCommandMenu(WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case ID_SETTINGS:
	{
		settingsWindow.Create(handle);
		break;
	}
	}
}


/*private methods*/

void CArkanoidWindow::calculateBallCoordinates(const RECT & clientRect)
{
	if (ballLeft == -10) {
		ballLeft = clientRect.right / 2 - BALL_RADIUS;
		ballRight = clientRect.right / 2 + BALL_RADIUS;
		ballTop = clientRect.bottom - 2 * BALL_RADIUS - PLATFORM_HEIGHT;
		ballBottom = clientRect.bottom - PLATFORM_HEIGHT;
	}
	else {
		if(handle == GetFocus()) {
			ballLeft += 2 * ballDirectionOnX;
			ballRight += 2 * ballDirectionOnX;
			ballTop += 2 * ballDirectionOnY;
			ballBottom += 2 * ballDirectionOnY;
		}
	}
}

void CArkanoidWindow::calculateBallDirection(const RECT & clientRect)
{
	if(isGameFinished) {
		return;
	}
	if(ballRight >= clientRect.right) {
		ballDirectionOnX = -1;
	}
	if(ballTop <= clientRect.top) {
		ballDirectionOnY = 1;
	}
	if(ballLeft <= clientRect.left) {
		ballDirectionOnX = 1;
	}
	// Шарик касется платформы
	if(ballBottom >= platformTop && ballBottom < clientRect.bottom) {
		if((ballRight - BALL_RADIUS > platformLeft) && (ballRight - BALL_RADIUS < platformRight)) {
			ballDirectionOnY = -1;

			// немного сдвигаем шарик, если последнее время платформа двигалась
			// (эфект инерции)
			//////////////////////////////////////////////
			ballRight += platformLastDirection * 15; 
			ballLeft += platformLastDirection * 15;
		}
	}
	if(ballBottom >= clientRect.bottom) {
		// Dialog window, сообщающий об окончании игры		
		isGameFinished = true;
		endGameDialog.Create(handle);
		SetFocus(endGameDialog.GetWindowHandle());
	}

	// Взаимоедйствие шарика с кирпичами
	int bricksBottom = bricks.Coordinates[brickRowsNum - 1][0].bottom;
	int bricksTop = bricks.Coordinates[0][0].top;
	if((ballTop <= bricksBottom) && (ballBottom >= bricksTop)) {
		InteractionOfBricksAndBall(clientRect);
	}
}

void CArkanoidWindow::calculateBricksCoordinates(const RECT & clientRect)
{
	// Посчитаем ширину одного кирпичика
	int brickWidth = (clientRect.right - clientRect.left) / bricks.colsNum;

	int centerCoordinate = clientRect.left + (clientRect.right - clientRect.left) / 2;
	int centralBrickIndex = bricks.colsNum / 2;

	// Теперь нужно вычислить координаты всех кирпичиков
	if (bricks.colsNum % 2 == 1) { // один кирпичик по центру

		for (int row = 0; row < bricks.rowsNum; row++) {
			bricks.Coordinates[row][centralBrickIndex].left = centerCoordinate - brickWidth / 2;
			bricks.Coordinates[row][centralBrickIndex].right = centerCoordinate + brickWidth / 2;

			bricks.Coordinates[row][centralBrickIndex].top = BRICK_HEIGHT * (row + 4);
			bricks.Coordinates[row][centralBrickIndex].bottom = BRICK_HEIGHT * (row + 5);
		}

	}
	else { // по центру кирпичики соприкасаются
		for (int row = 0; row < bricks.rowsNum; row++) {
			bricks.Coordinates[row][centralBrickIndex].right = centerCoordinate + brickWidth;
			bricks.Coordinates[row][centralBrickIndex].left = centerCoordinate;

			bricks.Coordinates[row][centralBrickIndex].top = BRICK_HEIGHT * (row + 4);
			bricks.Coordinates[row][centralBrickIndex].bottom = BRICK_HEIGHT * (row + 5);
		}
	}

	for (int row = 0; row < bricks.rowsNum; row++) {
		for (int col = centralBrickIndex - 1; col >= 0; col--) {
			bricks.Coordinates[row][col].right = bricks.Coordinates[row][col + 1].left;
			bricks.Coordinates[row][col].left = bricks.Coordinates[row][col].right - brickWidth;

			bricks.Coordinates[row][col].top = BRICK_HEIGHT * (row + 4);
			bricks.Coordinates[row][col].bottom = BRICK_HEIGHT * (row + 5);
		}

		for (int col = centralBrickIndex + 1; col < bricks.colsNum; col++) {
			bricks.Coordinates[row][col].left = bricks.Coordinates[row][col - 1].right;
			bricks.Coordinates[row][col].right = bricks.Coordinates[row][col].left + brickWidth;

			bricks.Coordinates[row][col].top = BRICK_HEIGHT * (row + 4);
			bricks.Coordinates[row][col].bottom = BRICK_HEIGHT * (row + 5);
		}
	}
}

void CArkanoidWindow::placeBall(HDC hdc, const RECT &clientRect)
{
	for(int iter = 0; iter < deltaTime; iter++) {
		calculateBallCoordinates(clientRect);
		calculateBallDirection(clientRect);
	}

	// Создаём кисть для рисования эллипса
	HBRUSH insideEllipsBrush = CreateSolidBrush(RGB(0, 255, 0));
	// Делаем кисть для рисования эллипса активной
	HGDIOBJ replacedBrush = SelectObject(hdc, insideEllipsBrush);
	
	Ellipse(hdc, ballLeft, ballTop, ballRight, ballBottom);

	// Заканчиваем использование кисти
	SelectObject(hdc, replacedBrush);
	DeleteObject(insideEllipsBrush);
}

void CArkanoidWindow::placePlatform(HDC hdc, const RECT & clientRect)
{
	// Создаём кисть для рисования платформы
	HBRUSH platformBrush = CreateSolidBrush(RGB(255, 0, 0));
	// Делаем кисть для рисования платформы активной
	HGDIOBJ replacedBrush = SelectObject(hdc, platformBrush);
	
	if(platformLeft == -10) {
		platformLeft = clientRect.right / 2 - PLATFORM_LENGTH / 2;
		platformRight = clientRect.right / 2 + PLATFORM_LENGTH / 2;
		platformTop = clientRect.bottom - PLATFORM_HEIGHT;
		platformBottom = clientRect.bottom;
	}

	RoundRect(hdc, platformLeft, platformTop, platformRight, platformBottom, 5, 5);

	// Уменьшаем время, которое помним последнее движение платформы
	if(timeBeforeForgetness != 0) {
		timeBeforeForgetness--;
	}
	if(timeBeforeForgetness == 0) {
		platformLastDirection = 0;
	}

	// Заканчиваем использование кисти
	SelectObject(hdc, replacedBrush);
	DeleteObject(platformBrush);
}

void CArkanoidWindow::placeBricks(HDC hdc, const RECT & clientRect)
{
	calculateBricksCoordinates(clientRect);

	// Создаём кисть для рисования платформы
	HBRUSH bricksBrush = CreateSolidBrush(RGB(0, 0, 255));
	// Делаем кисть для рисования платформы активной
	HGDIOBJ replacedBrush = SelectObject(hdc, bricksBrush);

	for(int row = 0; row < bricks.rowsNum; row++) {
		for(int col = 0; col < bricks.colsNum; col++) {
			if(bricks.Existense[row][col]) {
				RoundRect(hdc, 
					bricks.Coordinates[row][col].left,
					bricks.Coordinates[row][col].top,
					bricks.Coordinates[row][col].right,
					bricks.Coordinates[row][col].bottom,
					2, 2);
			}
		}
	}

	// Заканчиваем использование кисти
	SelectObject(hdc, replacedBrush);
	DeleteObject(bricksBrush);
}

void CArkanoidWindow::InteractionOfBricksAndBall(const RECT & clientRect)
{
	for(int row = 0; row < brickRowsNum; row++) {
		for(int col = 0; col < bricksColsNum; col++) {
			// Рассматриваем взаимодействие с заданным кирпичём только если он ещё существует
			if(bricks.Existense[row][col]) {
				if (ballTop <= bricks.Coordinates[row][col].bottom && 
						ballBottom >= bricks.Coordinates[row][col].top &&
						ballRight >= bricks.Coordinates[row][col].left &&
						ballLeft <= bricks.Coordinates[row][col].right) {
					bricks.Existense[row][col] = 0;
					bricks.currentBricksNumber -= 1;

					scoreWindow.SetScore(scoreWindow.GetScore() + 1);
					if(scoreWindow.GetScore() == brickRowsNum * bricksColsNum) {
						isGameFinished = true;
						endGameDialog.Create(handle);
						SetFocus(endGameDialog.GetWindowHandle());
					}
					
					ChangeBallDirectionAfterBrickKill(clientRect);
				}
			}
		}
	}
}

void CArkanoidWindow::ChangeBallDirectionAfterBrickKill(const RECT &clientRect)
{
	ballDirectionOnY *= -1;

	if(ballLeft >= clientRect.right / 2) {
		ballDirectionOnX = 1;
	} else {
		ballDirectionOnX = -1;
	}
}

LRESULT CArkanoidWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CArkanoidWindow* window = (CArkanoidWindow*)GetWindowLongPtr(handle, GWLP_HINSTANCE);

	switch (message) {
	case WM_NCCREATE:
	{
		CArkanoidWindow* window = (CArkanoidWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLongPtr(handle, GWLP_HINSTANCE, (LONG)window);
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE:
	{
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_PAINT:
	{
		window->OnPaint();
		return TRUE;
	}
	case WM_TIMER:
	{
		window->OnTimer();
		return TRUE;
	}
	case WM_COMMAND:
	{
		window->OnCommand(wParam, lParam);
		return TRUE;
	}
	case WM_SIZE:
	{
		window->OnSize();
		return TRUE;
	}
	case WM_DESTROY:
	{
		window->OnDestroy();
		return TRUE;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
