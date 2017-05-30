#pragma once

#include <Windows.h>
#include <vector>

struct BricksCoordinates {
	int left;
	int right;
	int top;
	int bottom;
};

// �����, � ������� ����� ��������� ���������� � ��������� ����������
class CBricks {
public:
	// �������� ������ ������� ��������� � ����������
	CBricks(int rowsNum, int colsNum);
	
	std::vector<std::vector<int>> Existense; // �������: 0 - ��������� ���, 1 - ����
	std::vector<std::vector<BricksCoordinates>> Coordinates; // ���������� ���� ����������

	int currentBricksNumber; // ������� ���������� ����������
	int rowsNum; // ���������� ����� ����������
	int colsNum; // ���������� ���������� � ������ ������
};