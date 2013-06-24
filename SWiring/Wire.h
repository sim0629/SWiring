/* Wire class header */

#pragma once

#include "Util.h"

class Wire {
	friend class Board;
private:
	Point input;
	Point output;
	int selected;	// ����:�ӽ÷� �ö�� ����, 0:���� �� ��, 1:���� ��

public:
	Wire(const Point& pi, const Point& po);

	void run(int** pins);
	void draw(HDC hdc, Point pm);

	static void layout(HDC hdc, Point ps, Point pm);

};

typedef std::vector<Wire*> Wires;
