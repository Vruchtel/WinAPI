#pragma once

#include <Windows.h>
#include <vector>

struct BricksCoordinates {
	int left;
	int right;
	int top;
	int bottom;
};

// Класс, в котором будет храниться информация о сбиваемых кирпичиках
class CBricks {
public:
	// Вычислит ширину каждого кирпичика и координаты
	CBricks(int rowsNum, int colsNum);
	
	std::vector<std::vector<int>> Existense; // матрица: 0 - кирпичика нет, 1 - есть
	std::vector<std::vector<BricksCoordinates>> Coordinates; // координаты всех кирпичиков

	int currentBricksNumber; // Текущее количество кирпичиков
	int rowsNum; // количество строк кирпичиков
	int colsNum; // количество кирпичиков в каждой строке
};