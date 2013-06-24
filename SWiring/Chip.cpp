/* Chip class code */

#include "Chip.h"

Kind Chip::tool = NONE;

Point Chip::sizes[DUMMY] =	{
							Point(0, 0),
							Point(3, 6),	// INV
							Point(3, 6),	// AND2
							Point(3, 6),	// OR2
							Point(3, 6),	// NAND2
							Point(3, 6),	// NOR2
							Point(3, 6),	// XOR2
							Point(3, 6),	// AND3
							Point(3, 6),	// NAND3
							Point(3, 6),	// AND4
							Point(3, 6),	// NAND4
							Point(3, 7),	// MUX2
							Point(7, 7),	// CPLD
							Point(6, 3),	// CLK
							Point(3, 7),	// DEC
						};
LPCTSTR Chip::names[DUMMY] =	{
							TEXT(""),
							TEXT("INV"),
							TEXT("AND2"),
							TEXT("OR2"),
							TEXT("NAND2"),
							TEXT("NOR2"),
							TEXT("XOR2"),
							TEXT("AND3"),
							TEXT("NAND3"),
							TEXT("AND4"),
							TEXT("NAND4"),
							TEXT("MUX2"),
							TEXT("CPLD"),
							TEXT("CLK"),
							TEXT("DEC"),
						};

Chip::Chip(Kind k, const Point& pl) : kind(k), location(pl), selected(0) {
	switch(kind) {
	case INV:
		//	7404
		
		//	i0	VCC
		//	o0	i3
		//	i1	o3
		//	o1	i4
		//	i2	o4
		//	o2	i5
		//	GND	o5
		
		inputs.push_back(location + Point(0, 6));
		inputs.push_back(location + Point(0, 4));
		inputs.push_back(location + Point(0, 2));
		inputs.push_back(location + Point(3, 5));
		inputs.push_back(location + Point(3, 3));
		inputs.push_back(location + Point(3, 1));
		
		outputs.push_back(location + Point(0, 5));
		outputs.push_back(location + Point(0, 3));
		outputs.push_back(location + Point(0, 1));
		outputs.push_back(location + Point(3, 4));
		outputs.push_back(location + Point(3, 2));
		outputs.push_back(location + Point(3, 0));
		break;

	case AND2:
	case OR2:
	case NAND2:
	case XOR2:
		// 7408 7432 7400 7486
		
		// i0	VCC
		// i1	i4
		// o0	i5
		// i2	o2
		// i3	i6
		// o1	i7
		// GND	o3

		inputs.push_back(location + Point(0, 6));
		inputs.push_back(location + Point(0, 5));
		inputs.push_back(location + Point(0, 3));
		inputs.push_back(location + Point(0, 2));
		inputs.push_back(location + Point(3, 5));
		inputs.push_back(location + Point(3, 4));
		inputs.push_back(location + Point(3, 2));
		inputs.push_back(location + Point(3, 1));

		outputs.push_back(location + Point(0, 4));
		outputs.push_back(location + Point(0, 1));
		outputs.push_back(location + Point(3, 3));
		outputs.push_back(location + Point(3, 0));
		break;

	case NOR2:
		// 7402
		
		// o0	VCC
		// i0	o2
		// i1	i4
		// o1	i5
		// i2	o3
		// i3	i6
		// GND	i7

		inputs.push_back(location + Point(0, 5));
		inputs.push_back(location + Point(0, 4));
		inputs.push_back(location + Point(0, 2));
		inputs.push_back(location + Point(0, 1));
		inputs.push_back(location + Point(3, 4));
		inputs.push_back(location + Point(3, 3));
		inputs.push_back(location + Point(3, 1));
		inputs.push_back(location + Point(3, 0));

		outputs.push_back(location + Point(0, 6));
		outputs.push_back(location + Point(0, 3));
		outputs.push_back(location + Point(3, 5));
		outputs.push_back(location + Point(3, 2));
		break;

	case AND3:
	case NAND3:
		// 7411 7410
		
		// i0	VCC
		// i1	i2
		// i3	o0
		// i4	i6
		// i5	i7
		// o1	i8
		// GND	o2

		inputs.push_back(location + Point(0, 6));
		inputs.push_back(location + Point(0, 5));
		inputs.push_back(location + Point(3, 5));
		inputs.push_back(location + Point(0, 4));
		inputs.push_back(location + Point(0, 3));
		inputs.push_back(location + Point(0, 2));
		inputs.push_back(location + Point(3, 3));
		inputs.push_back(location + Point(3, 2));
		inputs.push_back(location + Point(3, 1));

		outputs.push_back(location + Point(3, 4));
		outputs.push_back(location + Point(0, 1));
		outputs.push_back(location + Point(3, 0));
		break;

	case AND4:
	case NAND4:
		// 7421 7420

		// i0	VCC
		// i1	i4
		// NC	i5
		// i2	NC
		// i3	i6
		// o0	i7
		// GND	o1

		inputs.push_back(location + Point(0, 6));
		inputs.push_back(location + Point(0, 5));
		inputs.push_back(location + Point(0, 3));
		inputs.push_back(location + Point(0, 2));
		inputs.push_back(location + Point(3, 5));
		inputs.push_back(location + Point(3, 4));
		inputs.push_back(location + Point(3, 2));
		inputs.push_back(location + Point(3, 1));

		outputs.push_back(location + Point(0, 1));
		outputs.push_back(location + Point(3, 0));
		break;

	case MUX2:
		// 74157

		// i8.S	VCC
		// i0	GND.G
		// i1	i4
		// o0	i5
		// i2	o2
		// i3	i6
		// o1	i7
		// GND	o3

		inputs.push_back(location + Point(0, 6));
		inputs.push_back(location + Point(0, 5));
		inputs.push_back(location + Point(0, 3));
		inputs.push_back(location + Point(0, 2));
		inputs.push_back(location + Point(3, 5));
		inputs.push_back(location + Point(3, 4));
		inputs.push_back(location + Point(3, 2));
		inputs.push_back(location + Point(3, 1));
		
		inputs.push_back(location + Point(0, 7));

		outputs.push_back(location + Point(0, 4));
		outputs.push_back(location + Point(0, 1));
		outputs.push_back(location + Point(3, 3));
		outputs.push_back(location + Point(3, 0));
		break;

	case CPLD:
		break;

	case CLK:
		break;

	case DEC:
		// 7447

		// o4,e	GND
		// o3,d	i0
		// o2,c	i3
		// o1,b	NC
		// o0,a	NC
		// o6,g	NC
		// o5,f	i2
		// VCC	i1

		inputs.push_back(location + Point(3, 6));
		inputs.push_back(location + Point(3, 0));
		inputs.push_back(location + Point(3, 1));
		inputs.push_back(location + Point(3, 5));

		outputs.push_back(location + Point(0, 3));
		outputs.push_back(location + Point(0, 4));
		outputs.push_back(location + Point(0, 5));
		outputs.push_back(location + Point(0, 6));
		outputs.push_back(location + Point(0, 7));
		outputs.push_back(location + Point(0, 1));
		outputs.push_back(location + Point(0, 2));

		break;

	}
}

void Chip::run(int** pins) {
	int n;
	switch(kind) {
	case INV:
		for(int i = 0; i < 6; i++) {
			if(pins[inputs[i].X][inputs[i].Y] >= 0)
				pins[outputs[i].X][outputs[i].Y] = !pins[inputs[i].X][inputs[i].Y];
		}
		break;
	case AND2:
		for(int i = 0; i < 4; i++) {
			if(pins[inputs[i*2].X][inputs[i*2].Y] < 0 && pins[inputs[i*2+1].X][inputs[i*2+1].Y] < 0) 
				pins[outputs[i].X][outputs[i].Y] = -1;
			else
				pins[outputs[i].X][outputs[i].Y] = pins[inputs[i*2].X][inputs[i*2].Y] * pins[inputs[i*2+1].X][inputs[i*2+1].Y];
		}
		break;
	case OR2:
		for(int i = 0; i < 4; i++) {
			if(pins[inputs[i*2].X][inputs[i*2].Y] == 0 && pins[inputs[i*2+1].X][inputs[i*2+1].Y] == 0)
				pins[outputs[i].X][outputs[i].Y] = 0;
			else
				if(pins[inputs[i*2].X][inputs[i*2].Y] == 1 || pins[inputs[i*2+1].X][inputs[i*2+1].Y] == 1)
					pins[outputs[i].X][outputs[i].Y] = 1;
				else
					pins[outputs[i].X][outputs[i].Y] = -1;
		}
		break;
	case NAND2:
		for(int i = 0; i < 4; i++) {
			if(pins[inputs[i*2].X][inputs[i*2].Y] * pins[inputs[i*2+1].X][inputs[i*2+1].Y] == 0) 
				pins[outputs[i].X][outputs[i].Y] = 1;
			else
				if(pins[inputs[i*2].X][inputs[i*2].Y] < 0 || pins[inputs[i*2+1].X][inputs[i*2+1].Y] < 0)
					pins[outputs[i].X][outputs[i].Y] = -1;
				else
					pins[outputs[i].X][outputs[i].Y] = 0;
		}
		break;
	case NOR2:
		for(int i = 0; i < 4; i++) {
			if(pins[inputs[i*2].X][inputs[i*2].Y] == 1 || pins[inputs[i*2+1].X][inputs[i*2+1].Y] == 1) 
				pins[outputs[i].X][outputs[i].Y] = 0;
			else
				if(pins[inputs[i*2].X][inputs[i*2].Y] < 0 || pins[inputs[i*2+1].X][inputs[i*2+1].Y] < 0)
					pins[outputs[i].X][outputs[i].Y] = -1;
				else
					pins[outputs[i].X][outputs[i].Y] = 1;
		}
		break;
	case XOR2:
		for(int i = 0; i < 4; i++) {
			if(pins[inputs[i*2].X][inputs[i*2].Y] < 0 || pins[inputs[i*2+1].X][inputs[i*2+1].Y] < 0) 
				pins[outputs[i].X][outputs[i].Y] = -1;
			else
				pins[outputs[i].X][outputs[i].Y] = pins[inputs[i*2].X][inputs[i*2].Y] ^ pins[inputs[i*2+1].X][inputs[i*2+1].Y];
		}
		break;
	case AND3:
		for(int i = 0; i < 3; i++) {
			if(pins[inputs[i*3].X][inputs[i*3].Y] < 0 || pins[inputs[i*3+1].X][inputs[i*3+1].Y] < 0 || pins[inputs[i*3+2].X][inputs[i*3+2].Y] < 0) {
				if(pins[inputs[i*3].X][inputs[i*3].Y] * pins[inputs[i*3+1].X][inputs[i*3+1].Y] * pins[inputs[i*3+2].X][inputs[i*3+2].Y] == 0)
					pins[outputs[i].X][outputs[i].Y] = 0;
				else
					pins[outputs[i].X][outputs[i].Y] = -1;
			}else {
				pins[outputs[i].X][outputs[i].Y] = pins[inputs[i*3].X][inputs[i*3].Y] & pins[inputs[i*3+1].X][inputs[i*3+1].Y] & pins[inputs[i*3+2].X][inputs[i*3+2].Y];
			}
		}
		break;
	case NAND3:
		for(int i = 0; i < 3; i++) {
			if(pins[inputs[i*3].X][inputs[i*3].Y] < 0 || pins[inputs[i*3+1].X][inputs[i*3+1].Y] < 0 || pins[inputs[i*3+2].X][inputs[i*3+2].Y] < 0) {
				if(pins[inputs[i*3].X][inputs[i*3].Y] * pins[inputs[i*3+1].X][inputs[i*3+1].Y] * pins[inputs[i*3+2].X][inputs[i*3+2].Y] == 0)
					pins[outputs[i].X][outputs[i].Y] = 1;
				else
					pins[outputs[i].X][outputs[i].Y] = -1;
			}else {
				pins[outputs[i].X][outputs[i].Y] = !(pins[inputs[i*3].X][inputs[i*3].Y] & pins[inputs[i*3+1].X][inputs[i*3+1].Y] & pins[inputs[i*3+2].X][inputs[i*3+2].Y]);
			}
		}
		break;
	case AND4:
		for(int i = 0; i < 2; i++) {
			if(pins[inputs[i*4].X][inputs[i*4].Y] < 0 || pins[inputs[i*4+1].X][inputs[i*4+1].Y] < 0 || pins[inputs[i*4+2].X][inputs[i*4+2].Y] < 0 || pins[inputs[i*4+3].X][inputs[i*4+3].Y] < 0) {
				if(pins[inputs[i*4].X][inputs[i*4].Y] * pins[inputs[i*4+1].X][inputs[i*4+1].Y] * pins[inputs[i*4+2].X][inputs[i*4+2].Y] * pins[inputs[i*4+3].X][inputs[i*4+3].Y] == 0)
					pins[outputs[i].X][outputs[i].Y] = 0;
				else
					pins[outputs[i].X][outputs[i].Y] = -1;
			}else {
				pins[outputs[i].X][outputs[i].Y] = pins[inputs[i*4].X][inputs[i*4].Y] & pins[inputs[i*4+1].X][inputs[i*4+1].Y] & pins[inputs[i*4+2].X][inputs[i*4+2].Y] & pins[inputs[i*4+3].X][inputs[i*4+3].Y];
			}
		}
		break;
	case NAND4:
		for(int i = 0; i < 2; i++) {
			if(pins[inputs[i*4].X][inputs[i*4].Y] < 0 || pins[inputs[i*4+1].X][inputs[i*4+1].Y] < 0 || pins[inputs[i*4+2].X][inputs[i*4+2].Y] < 0 || pins[inputs[i*4+3].X][inputs[i*4+3].Y] < 0) {
				if(pins[inputs[i*4].X][inputs[i*4].Y] * pins[inputs[i*4+1].X][inputs[i*4+1].Y] * pins[inputs[i*4+2].X][inputs[i*4+2].Y] * pins[inputs[i*4+3].X][inputs[i*4+3].Y] == 0)
					pins[outputs[i].X][outputs[i].Y] = 1;
				else
					pins[outputs[i].X][outputs[i].Y] = -1;
			}else {
				pins[outputs[i].X][outputs[i].Y] = !(pins[inputs[i*4].X][inputs[i*4].Y] & pins[inputs[i*4+1].X][inputs[i*4+1].Y] & pins[inputs[i*4+2].X][inputs[i*4+2].Y] & pins[inputs[i*4+3].X][inputs[i*4+3].Y]);
			}
		}
		break;
	case MUX2:
		n = pins[inputs[8].X][inputs[8].Y];
		for(int i = 0; i < 4; i++) {
			if(n < 0)
				pins[outputs[i].X][outputs[i].Y] = -1;
			else
				pins[outputs[i].X][outputs[i].Y] = pins[inputs[i*2+n].X][inputs[i*2+n].Y];
		}
		break;
	case CPLD:
		break;
	case CLK:
		break;
	case DEC:
		n = pins[inputs[3].X][inputs[3].Y] * 8 + pins[inputs[2].X][inputs[2].Y] * 4 + pins[inputs[1].X][inputs[1].Y] * 2 + pins[inputs[0].X][inputs[0].Y];
		for(int i = 0; i < 7; i++) pins[outputs[i].X][outputs[i].Y] = 0;
		switch(n) {
		case 1: case 4: case 11: case 13:
			pins[outputs[0].X][outputs[0].Y] = 1;
		}
		switch(n) {
		case 5: case 6: case 11: case 12: case 14: case 15:
			pins[outputs[0].X][outputs[1].Y] = 1;
		}
		switch(n) {
		case 2: case 12: case 14: case 15:
			pins[outputs[0].X][outputs[2].Y] = 1;
		}
		switch(n) {
		case 1: case 4: case 7: case 10: case 15:
			pins[outputs[0].X][outputs[3].Y] = 1;
		}
		switch(n) {
		case 1: case 3: case 4: case 5: case 7: case 9:
			pins[outputs[0].X][outputs[4].Y] = 1;
		}
		switch(n) {
		case 1: case 2: case 3: case 7: case 13:
			pins[outputs[0].X][outputs[5].Y] = 1;
		}
		switch(n) {
		case 0: case 1: case 7: case 12:
			pins[outputs[0].X][outputs[6].Y] = 1;
		}
		break;
	}
}

void Chip::draw(HDC hdc, Point pm) {
	Point p = L2A(location);

	if(selected > 0)		ChangePen(hdc, ppp / 8 + 1, MAGENTA);	// 선택
	else if(selected < 0)	ChangePen(hdc, ppp / 8 + 1, RED);		// 임시
	else					ChangePen(hdc, ppp / 8 + 1, BLACK);	// 안됨
	ChangeBrush(hdc, LGRAY);
	Rectangle(hdc, p.X, p.Y - sizes[kind].Y * ppp, p.X + sizes[kind].X * ppp, p.Y);	// LAYOUT
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, p.X, p.Y, names[kind], lstrlen(names[kind]));

	if(selected > 0)		ChangePen(hdc, 1, MAGENTA);	// 선택
	else if(selected < 0)	ChangePen(hdc, 1, RED);		// 임시
	else					ChangePen(hdc, 1, BLACK);	// 안됨
	HBRUSH hBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	for(WORD i = 0; i < inputs.size(); i++) {
		p = L2A(inputs[i]);
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);
	}
	for(WORD i = 0; i < outputs.size(); i++) {
		p = L2A(outputs[i]);
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);
	}
	SelectObject(hdc, hBrush);

	switch(kind) {
	case INV:
	case AND2:
	case OR2:
	case NAND2:
	case NOR2:
	case XOR2:
	case AND3:
	case NAND3:
	case AND4:
	case NAND4:
		p = L2A(location);
		ChangeBrush(hdc, BLUE);
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);	// GND

		p = L2A(location + sizes[kind]);
		ChangeBrush(hdc, RED);
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);	// VCC
		break;

	case DEC:
		p = L2A(location);
		ChangeBrush(hdc, RED);
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);	// VCC

		p = L2A(location + sizes[kind]);
		ChangeBrush(hdc, BLUE);
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);	// GND
		break;

	case MUX2:
		p = L2A(location);
		ChangeBrush(hdc, BLUE);
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);	// GND

		p = L2A(location + sizes[kind] - Point(0, 1));
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);	// GND

		p = L2A(location + sizes[kind]);
		ChangeBrush(hdc, RED);
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);	// VCC
		break;
		
	case CPLD:
		break;

	case CLK:
		break;

	}
}

void Chip::layout(HDC hdc, Point pm) {
	HBRUSH hBrush;
	Point p = L2A(pm);
	switch(tool) {
	case INV:
	case AND2:
	case OR2:
	case NAND2:
	case NOR2:
	case XOR2:
	case AND3:
	case NAND3:
	case AND4:
	case NAND4:
	case MUX2:
	case CPLD:
	case CLK:
	case DEC:
		ChangePen(hdc, ppp / 8 + 1, GRAY);
		hBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Rectangle(hdc, p.X, p.Y - sizes[tool].Y * ppp, p.X + sizes[tool].X * ppp, p.Y);
		SelectObject(hdc, hBrush);
		break;
	}
}
