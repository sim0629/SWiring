/* Wire class header */

#pragma once

#include "Util.h"

class Wire {
	friend class Board;
private:
	Point input;
	Point output;
	int selected;	// 음수:임시로 올라와 있음, 0:선택 안 됨, 1:선택 됨

public:
	Wire(const Point& pi, const Point& po);

	void run(int** pins);
	void draw(HDC hdc, Point pm);

	static void layout(HDC hdc, Point ps, Point pm);

};

typedef std::vector<Wire*> Wires;
