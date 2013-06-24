/* Wire class code */

#include "Wire.h"

Wire::Wire(const Point& pi, const Point& po) : selected(0), input(pi), output(po) {
}

void Wire::run(int** pins) {
	pins[output.X][output.Y] = pins[input.X][input.Y];
}

void Wire::draw(HDC hdc, Point pm) {
	if(selected > 0)		ChangePen(hdc, 1, MAGENTA);	// 선택
	else if(selected < 0)	ChangePen(hdc, 1, RED);		// 임시
	else					ChangePen(hdc, 1, BLUE);	// 안됨
	
	Point p, q, v;
	p = L2A(input);
	MoveToEx(hdc, p.X, p.Y, NULL);
	p = L2A(output);
	LineTo(hdc, p.X, p.Y);
	
	v = output - input;
	
	p.X = v.X - v.Y; p.Y = -(v.X + v.Y);
	int len = p.length();
	if(len == 0) len = 1;
	p = (p * ppp) / len / 2;
	q = L2A(output) - p;
	LineTo(hdc, q.X, q.Y);

	p = L2A(output);
	MoveToEx(hdc, p.X, p.Y, NULL);

	p.X = v.X + v.Y; p.Y = -(-v.X + v.Y);
	len = p.length();
	if(len == 0 ) len = 1;
	p = (p * ppp) / len / 2;
	q = L2A(output) - p;
	LineTo(hdc, q.X, q.Y);
	
}

void Wire::layout(HDC hdc, Point ps, Point pm) {
	if(ps.X >= 0) {
		ChangePen(hdc, 1, LBLUE);
		ps = L2A(ps);
		MoveToEx(hdc, ps.X, ps.Y, NULL);
		pm = L2A(pm);
		LineTo(hdc, pm.X, pm.Y);
	}
}
