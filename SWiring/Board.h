/* Board class header */

#pragma once

#include "Util.h"
#include "Chip.h"
#include "Wire.h"

class Board {
private:
	Chips chips;
	Wires wires;
	int** pins;
	Point size;
	// + ���� ����? New, Saved, Changed ?

	int findChip(Point apm);	// ���콺�� ��ġ�� ���� (actual point of mouse)
	int findWire(Point apm);	// ���콺�� ��ġ�� ����
	
	void swapPoints(Point a, Point b);
	Points aPins;
	std::vector<int> aChips;

public:
	std::vector<int> buses;
	Point selectedPin;	// ���̾��� ������

	Board(const Point& p = Point(48, 66));
	// + file�� ����
	~Board();	// + ���� ����?

	void swap();
	void move(int d);

	void reset();
	void simulate(int n);
	inline void setPin(Point p, int v) {
		if(p.X >= 0 && p.X < size.X && p.Y >= 0 && p.Y < size.Y)
			pins[p.X][p.Y] = (v < 0 ? -1 : !pins[p.X][p.Y]);
	}

	void mouse(int state, Point apm);	// state (0:none click, 1:click without shift, 2:click with shift, 3:right, 4:alt)

	void newChip(Kind kind, Point pm);
	void newWire(Point pb, Point pe);
	void delSelected();

	void draw(HDC hdc, Point pm);

	std::string toString();

	static int nSelectedChip;
	static int nSelectedWire;

};
