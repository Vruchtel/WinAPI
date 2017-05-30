#include "Bricks.h"

#define BRICK_HEIGHT 10  // высота каждого кирпичика

CBricks::CBricks(int _rowsNum, int _colsNum)
{
	rowsNum = _rowsNum;
	colsNum = _colsNum;
	//clientRectWidth = _clientRectWidth; // nope

	std::vector<std::vector<int>> tmpBE(_rowsNum, std::vector<int>(_colsNum, 1));
	Existense = tmpBE;

	Coordinates.resize(_rowsNum);
	for(int i = 0; i < _rowsNum; i++) {
		Coordinates[i].resize(_colsNum);
	}

	currentBricksNumber = _rowsNum * _colsNum;
}

