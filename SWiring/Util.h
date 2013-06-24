/* Utility header */

#pragma once

#include <windows.h>
#include <math.h>
#include <vector>

#define BLACK	RGB(0, 0, 0)
#define LGRAY	RGB(192, 192, 192)
#define GRAY	RGB(128, 128, 128)
#define WHITE	RGB(255, 255, 255)
#define RED		RGB(255, 0, 0)
#define GREEN	RGB(0, 255, 0)
#define LBLUE	RGB(64, 64, 255)
#define BLUE	RGB(0, 0, 255)
#define CYAN	RGB(0, 255, 255)
#define MAGENTA	RGB(255, 0, 255)
#define YELLOW	RGB(255, 255, 0)

class Point {
public:
	int X, Y;
	
	Point() : X(0), Y(0) {}
	Point(int x, int y) : X(x), Y(y) {}
	Point(const Point& p) : X(p.X), Y(p.Y) {}

	Point& operator= (const Point& p) { X = p.X; Y = p.Y; return *this; }
	Point operator+ (const Point& p) const { return Point(X + p.X, Y + p.Y); }
	Point operator- (const Point& p) const { return Point(X - p.X, Y - p.Y); }
	Point operator* (int c) const { return Point(X * c, Y * c); }
	Point operator/ (int c) { return Point(X / c, Y / c); }
	bool operator== (const Point& p) const { return (X == p.X) && (Y == p.Y); }
	bool operator!= (const Point& p) const { return (X != p.X) || (Y != p.Y); }
	int length() const { return (int)sqrt((double)(X * X + Y * Y)); }
	
	static int inner(const Point& pl, const Point& pr) { return pl.X * pr.X + pl.Y * pr.Y; }

};
typedef std::vector<Point> Points;

// 전역 변수들
extern HWND hWndMain;
extern int ppp;
extern Point cpt;
extern Point origin;
extern HPEN g_hPen;
extern HBRUSH g_hBrush;

Point L2A(const Point& p);	// Convert Logical coord to Actual coord
Point A2L(const Point& p);	// Convert Actual coord to Logical coord

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
BOOL Circle(HDC hdc, int x, int y, int r);
void ChangePen(HDC hdc, int width, COLORREF color);
void ChangeBrush(HDC hdc, COLORREF color);
