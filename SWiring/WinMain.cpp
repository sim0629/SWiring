#include "Util.h"
#include "Board.h"

// 전역 변수들
Board* board;
int ppp = 0;	// pixels per pin
Point cpt;	// client point
Point origin;	// 원점의 실제 좌표
HPEN g_hPen = CreatePen(PS_SOLID, 1, BLACK);
HBRUSH g_hBrush = CreateSolidBrush(WHITE);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("SWiring");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while(GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HBITMAP hBit;
void Refresh(){
	static Point cptOld;
	HDC hdc, hMemDC;
	HBITMAP hOldBit;

	hdc = GetDC(hWndMain);
	if(cpt != cptOld) {
		DeleteObject(hBit);
		hBit = CreateCompatibleBitmap(hdc, cpt.X, cpt.Y);
		cptOld = cpt;
	}
	hMemDC = CreateCompatibleDC(hdc);
	hOldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	//draw begin
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWndMain, &pt);
	Point pm(pt.x, pt.y), p;
	p = A2L(pm + Point(ppp / 2, -ppp / 2));
	board->draw(hMemDC, p);
	//draw end

	SelectObject(hMemDC, hOldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);

	InvalidateRgn(hWndMain, NULL, FALSE);
}

void open(char* fn) {
	if(fn) {
		int i, n[4];
		FILE* f;
		if(fopen_s(&f, fn, "r") == 0) {
			fscanf_s(f, "%d%d", &n[0], &n[1]);
			board = new Board(Point(n[0], n[1]));
			
			fscanf_s(f, "%d", &i);
			while(i--) {
				fscanf_s(f, "%d%d%d", &n[0], &n[1], &n[2]);
				board->newChip((Kind)n[0], Point(n[1], n[2]));
			}
			fscanf_s(f, "%d", &i);
			while(i--) {
				fscanf_s(f, "%d%d%d%d", &n[0], &n[1], &n[2], &n[3]);
				board->newWire(Point(n[0], n[1]), Point(n[2], n[3]));
			}
			fscanf_s(f, "%d", &i);
			while(i--) {
				fscanf_s(f, "%d", &n[0]);
				board->buses.push_back(n[0]);
			}
			fclose(f);
			return;
		}
	}
	board = new Board;
}

void save() {
	FILE* f;
	char buf[64];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(buf, "%04d%02d%02d%02d%02d%02d.swr.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	if(fopen_s(&f, buf, "w") == 0) {
		fprintf_s(f, board->toString().c_str());
		fclose(f);
		MessageBox(hWndMain, TEXT("Saved"), lpszClass, MB_OK);
	}
}

/* WndProc
 * 메시지 처리 루틴
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	Point p;

	switch(iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		
		CreateWindow(TEXT("button"), TEXT("NONE"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
			0, NONE * 15, 75, 15, hWnd, (HMENU)NONE, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("INV"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, INV * 15, 75, 15, hWnd, (HMENU)INV, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("AND2"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, AND2 * 15, 75, 15, hWnd, (HMENU)AND2, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("OR2"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, OR2 * 15, 75, 15, hWnd, (HMENU)OR2, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("NAND2"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, NAND2 * 15, 75, 15, hWnd, (HMENU)NAND2, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("NOR2"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, NOR2 * 15, 75, 15, hWnd, (HMENU)NOR2, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("XOR2"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, XOR2 * 15, 75, 15, hWnd, (HMENU)XOR2, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("AND3"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, AND3 * 15, 75, 15, hWnd, (HMENU)AND3, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("NAND3"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, NAND3 * 15, 75, 15, hWnd, (HMENU)NAND3, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("AND4"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, AND4 * 15, 75, 15, hWnd, (HMENU)AND4, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("NAND4"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, NAND4 * 15, 75, 15, hWnd, (HMENU)NAND4, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("MUX2"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, MUX2 * 15, 75, 15, hWnd, (HMENU)MUX2, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("CPLD"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, CPLD * 15, 75, 15, hWnd, (HMENU)CPLD, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("CLK"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, CLK * 15, 75, 15, hWnd, (HMENU)CLK, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("DEC"),
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			0, DEC * 15, 75, 15, hWnd, (HMENU)DEC, g_hInst, NULL);

		CheckRadioButton(hWnd, NONE, DUMMY - 1, NONE);

		open("open.swr.txt");	// initial file
		
		SetTimer(hWnd, 0, 20, NULL);
		return 0;

	case WM_TIMER:
		Refresh();
		return 0;

	case WM_SIZE:
		cpt.X = LOWORD(lParam); cpt.Y = HIWORD(lParam);
		if(ppp == 0) {	// ppp 한 번만 초기화
			ppp = 15;
			origin.X = 0; origin.Y = cpt.Y;
		}
		return 0;

	case WM_MOUSEMOVE:
		// wParam에 MK_들 필요할 때 처리
		if(!Chip::tool) board->mouse(0, Point(LOWORD(lParam), HIWORD(lParam)));
		return 0;

	case WM_LBUTTONDOWN:
		// wParam에 MK_들 필요할 때 처리
		p.X = LOWORD(lParam); p.Y = HIWORD(lParam);
		p = p + Point(ppp / 2, -ppp / 2);
		p = A2L(p);
		if(!Chip::tool) {
			if(GetKeyState(VK_CONTROL) >> 1) board->setPin(p, 1);
			else {
				if(GetKeyState(VK_SHIFT) >> 1) {
					board->mouse(2, Point(LOWORD(lParam), HIWORD(lParam)));
				}else if(GetKeyState(VK_MENU) >> 1) {
					board->mouse(4, Point(LOWORD(lParam), HIWORD(lParam)));
				}else {
					board->mouse(1, Point(LOWORD(lParam), HIWORD(lParam)));
				}
			}
		}else {
			board->newChip(Chip::tool, p);
		}
		return 0;
	
	case WM_RBUTTONDOWN:
		// wParam에 MK_들 필요할 때 처리
		if(!Chip::tool) {
			p.X = LOWORD(lParam); p.Y = HIWORD(lParam);
			p = p + Point(ppp / 2, -ppp / 2);
			p = A2L(p);
			if(GetKeyState(VK_CONTROL) >> 1) board->setPin(p, -1);
			else {
				board->mouse(3, Point(LOWORD(lParam), HIWORD(lParam)));
			}
		}else {
			Chip::tool = NONE;
			CheckRadioButton(hWnd, NONE, DUMMY - 1, NONE);
		}
		return 0;

	case WM_KEYDOWN:
		if(Board::nSelectedChip == 0) {
			// origin 움직이기
			if(GetKeyState(VK_LEFT) >> 1)	origin.X += ppp;
			if(GetKeyState(VK_UP) >> 1)		origin.Y += ppp;
			if(GetKeyState(VK_RIGHT) >> 1)	origin.X -= ppp;
			if(GetKeyState(VK_DOWN) >> 1)	origin.Y -= ppp;
		}else if(Board::nSelectedChip == 1) {
			// 칩 움직이기 하나씩 밖에 못할 듯..
			switch(wParam) {
			case VK_LEFT:
				board->move(0); break;
			case VK_UP:
				board->move(1); break;
			case VK_RIGHT:
				board->move(2); break;
			case VK_DOWN:
				board->move(3); break;
			}
		}
		if(Board::nSelectedChip + Board::nSelectedWire != 0) if(wParam == VK_DELETE) board->delSelected();
		switch(wParam) {
		case VK_F1:
			if(ppp > 1) ppp -= 2;	// 축소
			break;
		case VK_F2:
			if(ppp < 100) ppp += 2;	// 확대
			break;
		case VK_F4:
			board->reset();
			break;
		case VK_F5:
			board->simulate(101);
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (wParam - '0' < DUMMY) {
				Chip::tool = (Kind)(wParam - '0');
				CheckRadioButton(hWnd, NONE, DUMMY - 1, Chip::tool);
			}
			break;
		case VK_SPACE:
			board->swap();
			break;
		case 'S':
			if(GetKeyState(VK_CONTROL) >> 1) save();
			break;
		case VK_ESCAPE:
			Chip::tool = NONE;
			CheckRadioButton(hWnd, NONE, DUMMY - 1, NONE);
			board->selectedPin = Point(-1, -1);
			break;
		}
		return 0;

	case WM_COMMAND:
		if(LOWORD(wParam) >= NONE && LOWORD(wParam) < DUMMY)
			Chip::tool = (Kind)LOWORD(wParam);
		SetFocus(hWnd);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if(hBit) DrawBitmap(hdc, 0, 0, hBit);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		if(hBit) DeleteObject(hBit);
		if(g_hPen) DeleteObject(g_hPen);
		if(g_hBrush) DeleteObject(g_hBrush);
		delete board;
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
