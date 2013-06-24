/* Utility code */

#include "Util.h"

Point L2A(const Point& p) {
	Point q;
	q = p * ppp;
	q.Y = -q.Y;
	q = origin + q;
	return q;
}

Point A2L(const Point& p) {
	Point q;
	q = p - origin;
	q.Y = -q.Y;
	q = q / ppp;
	return q;
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
	HDC hMemDC;
	HBITMAP hOldBit;
	int bx, by;
	BITMAP bit;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth; by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBit);
	DeleteDC(hMemDC);
}

BOOL Circle(HDC hdc, int x, int y, int r) {
	return Ellipse(hdc, x - r, y - r, x + r, y + r);
}

void ChangePen(HDC hdc, int width, COLORREF color) {
	DeleteObject(g_hPen);
	g_hPen = CreatePen(PS_SOLID, width, color);
	SelectObject(hdc, g_hPen);
}

void ChangeBrush(HDC hdc, COLORREF color) {
	DeleteObject(g_hBrush);
	g_hBrush = CreateSolidBrush(color);
	SelectObject(hdc, g_hBrush);
}
