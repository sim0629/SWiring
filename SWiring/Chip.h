/* Chip class header */

#pragma once

#include "Util.h"

enum Kind {
			NONE,
			INV,
			AND2,
			OR2,
			NAND2,
			NOR2,
			XOR2,
			AND3,
			NAND3,
			AND4,
			NAND4,
			MUX2,
			CPLD,
			CLK,
			DEC,
			DUMMY
		};	// 칩 종류 열거형

class Chip {
	friend class Board;
private:
	Kind kind;	// 칩 종류
	Point location;
	Points inputs;
	Points outputs;
	int selected;	// 음수:임시로 올라와 있음, 0:선택 안 됨, 1:선택 됨

public:
	Chip(Kind k, const Point& pl);

	void flip();
	void run(int** pins);
	void draw(HDC hdc, Point pm);

	static void layout(HDC hdc, Point pm);

	static Kind tool;
	static Point sizes[DUMMY];
	static LPCTSTR names[DUMMY];

};

typedef std::vector<Chip*> Chips;
