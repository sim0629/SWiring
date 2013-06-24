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
		};	// Ĩ ���� ������

class Chip {
	friend class Board;
private:
	Kind kind;	// Ĩ ����
	Point location;
	Points inputs;
	Points outputs;
	int selected;	// ����:�ӽ÷� �ö�� ����, 0:���� �� ��, 1:���� ��

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
