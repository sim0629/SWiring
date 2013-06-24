/* Board class code */

#include "Board.h"

int Board::nSelectedChip = 0;
int Board::nSelectedWire = 0;

Board::Board(const Point& p) : size(p) {
	origin.X = 0; origin.Y = cpt.Y;

	pins = new int*[size.X];
	for(int x = 0; x < size.X; x++) {
		pins[x] = new int[size.Y];
		for(int y = 0; y < size.Y; y++) {
			pins[x][y] = -1;
		}
	}
	aPins.push_back(Point(-1, -1));
	aPins.push_back(Point(-1, -1));
	aChips.push_back(-1);
	aChips.push_back(-1);
	
	selectedPin = Point(-1, -1);
}

Board::~Board() {
	for(WORD i = 0; i < chips.size(); i++) delete chips[i];
	for(WORD i = 0; i < wires.size(); i++) delete wires[i];
	for(int x = 0; x < size.X; x++) delete[] pins[x];
	delete[] pins;
}

void Board::reset() {
	for(int x = 0; x < size.X; x++)
		for(int y = 0; y < size.Y; y++)
			pins[x][y] = -1;
}

void Board::simulate(int n) {
	while(n--) {
		for(WORD i = 0; i < chips.size(); i++) chips[i]->run(pins);
		for(WORD i = 0; i < wires.size(); i++) wires[i]->run(pins);
		for(WORD i = 0; i < buses.size(); i++) {
			for(int x = 0; x < size.X; x++) pins[x][buses[i]] = 1;
		}
	}
}

void Board::draw(HDC hdc, Point pm) {
	RECT rt;
	SetRect(&rt, 0, 0, cpt.X, cpt.Y);
	FillRect(hdc, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));

	ChangePen(hdc, 1, BLACK);
	Rectangle(hdc, origin.X - ppp, origin.Y - size.Y * ppp, origin.X + size.X * ppp, origin.Y + ppp);
	for(int x = 0; x < size.X; x++) {
		for(int y = 0; y < size.Y; y++) {
			Point p(x, y);
			p = L2A(p);
			switch(pins[x][y]) {
			case 0:
				ChangeBrush(hdc, GRAY);
				Circle(hdc, p.X, p.Y, ppp / 4 + 1);
				break;
			case 1:
				ChangeBrush(hdc, YELLOW);
				Circle(hdc, p.X, p.Y, ppp / 4 + 1);
				break;
			case -1:
				ChangeBrush(hdc, WHITE);
				Circle(hdc, p.X, p.Y, 1);
				break;
			}
		}
	}

	for(WORD i = 0; i < chips.size(); i++) chips[i]->draw(hdc, pm);
	for(WORD i = 0; i < wires.size(); i++) wires[i]->draw(hdc, pm);

	if(!Chip::tool) {
		Point p = L2A(pm);
		ChangePen(hdc, 1, RED);
		HBRUSH hBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Circle(hdc, p.X, p.Y, ppp / 4 + 1);
		SelectObject(hdc, hBrush);
		Wire::layout(hdc, selectedPin, pm);
	}else {
		Chip::layout(hdc, pm);
	}
	if(selectedPin.X >= 0) {
		Point p = L2A(selectedPin);
		ChangePen(hdc, 1, LBLUE);
		ChangeBrush(hdc, LBLUE);
		Circle(hdc, p.X, p.Y, ppp / 6 + 1);
	}

	ChangePen(hdc, 1, CYAN);
	ChangeBrush(hdc, CYAN);
	for(int i = 0; i < 2; i++) {
		if(aPins[i].X >= 0) {
			Point p = L2A(aPins[i]);
			Circle(hdc, p.X, p.Y, ppp / 3 + 1);
		}
		if(aChips[i] >= 0) {
			Point p = L2A(chips[aChips[i]]->location) + Point(Chip::sizes[chips[aChips[i]]->kind].X, -Chip::sizes[chips[aChips[i]]->kind].Y) * ppp / 2;
			Circle(hdc, p.X, p.Y, ppp / 3 + 1);
		}
	}

	ChangePen(hdc, 1, RED);
	for(WORD i = 0; i < buses.size(); i++) {
		Point p = L2A(Point(-1, buses[i]));
		MoveToEx(hdc, p.X, p.Y, NULL);
		p = L2A(Point(size.X, buses[i]));
		LineTo(hdc, p.X, p.Y);
	}

}

int Board::findChip(Point apm) {
	for(WORD i = 0; i < chips.size(); i++) {
		Point pl = L2A(chips[i]->location);
		if(pl.X + ppp / 4 < apm.X && pl.X + Chip::sizes[chips[i]->kind].X * ppp - ppp / 4 > apm.X
			&& pl.Y - Chip::sizes[chips[i]->kind].Y * ppp + ppp / 4 < apm.Y && pl.Y - ppp / 4 > apm.Y) return i;
	}
	return -1;
}

int Board::findWire(Point apm) {
	for(WORD i = 0; i < wires.size(); i++) {
		Point pi = L2A(wires[i]->input), po = L2A(wires[i]->output);
		int distance = ppp;
		if(L2A(A2L(apm + Point(ppp / 2, -ppp / 2))) != pi) {
			if(Point::inner(po - pi, apm - pi) < 0)			distance = (apm - pi).length();
			else if(Point::inner(pi - po, apm - po) < 0)	distance = (apm - po).length();
			else {
				Point l = po - pi;
				Point p = apm - pi;
				double a = (double)Point::inner(l, p);
				a = (double)Point::inner(p, p) - a * a / (double)Point::inner(l, l);
				distance = (int)sqrt(a);
			}
		}
		if(distance <= (ppp / 4 + 1)) return i;
	}
	return -1;
}

void Board::swapPoints(Point a, Point b) {
	if(a != b) {
		for(WORD i = 0; i < wires.size(); i++) {
			if(wires[i]->input == a) wires[i]->input = b;
			else if(wires[i]->input == b) wires[i]->input = a;
			if(wires[i]->output == a) wires[i]->output = b;
			else if(wires[i]->output == b) wires[i]->output = a;
		}
	}
}

void Board::swap() {
	if(aPins[0].X >= 0) swapPoints(aPins[0], aPins[1]);
	else if(aChips[0] >= 0) {
		int w = Chip::sizes[chips[aChips[0]]->kind].X;
		int h = Chip::sizes[chips[aChips[0]]->kind].Y;
		if(Chip::sizes[chips[aChips[1]]->kind].X > w) w = Chip::sizes[chips[aChips[1]]->kind].X;
		if(Chip::sizes[chips[aChips[1]]->kind].Y > h) h = Chip::sizes[chips[aChips[1]]->kind].Y;
		for(int i = 0; i < w + 1; i++)
			for(int j = 0; j < h + 1; j++)
				swapPoints(chips[aChips[0]]->location + Point(i, j), chips[aChips[1]]->location + Point(i, j));
		Point p = chips[aChips[1]]->location - chips[aChips[0]]->location;
		chips[aChips[0]]->location = chips[aChips[0]]->location + p;
		for(WORD i = 0; i < chips[aChips[0]]->inputs.size(); i++) chips[aChips[0]]->inputs[i] = chips[aChips[0]]->inputs[i] + p;
		for(WORD i = 0; i < chips[aChips[0]]->outputs.size(); i++) chips[aChips[0]]->outputs[i] = chips[aChips[0]]->outputs[i] + p;
		chips[aChips[1]]->location = chips[aChips[1]]->location - p;
		for(WORD i = 0; i < chips[aChips[1]]->inputs.size(); i++) chips[aChips[1]]->inputs[i] = chips[aChips[1]]->inputs[i] - p;
		for(WORD i = 0; i < chips[aChips[1]]->outputs.size(); i++) chips[aChips[1]]->outputs[i] = chips[aChips[1]]->outputs[i] - p;
	}
}

void Board::move(int d) {	// left up right down
	Point p = Point(0, 0);
	int n = -1;
	for(WORD i = 0; i < chips.size(); i++) {
		if(chips[i]->selected == 1) {
			n = i;
			break;
		}
	}
	if(n < 0) return;
	switch(d) {
	case 0:
		p = Point(-1, 1);
	case 3:
		p = p + Point(0, -1);
		for(int i = 0; i < Chip::sizes[chips[n]->kind].X + 1; i++)
			for(int j = 0; j < Chip::sizes[chips[n]->kind].Y + 1; j++)
				swapPoints(chips[n]->location + Point(i, j), chips[n]->location + Point(i, j) + p);
		for(WORD i = 0; i < chips[n]->inputs.size(); i++)
			chips[n]->inputs[i] = chips[n]->inputs[i] + p;
		for(WORD i = 0; i < chips[n]->outputs.size(); i++)
			chips[n]->outputs[i] = chips[n]->outputs[i] + p;
		chips[n]->location = chips[n]->location + p;
		break;
	case 1:
		p = Point(-1, 1);
	case 2:
		p = p + Point(1, 0);
		for(int i = Chip::sizes[chips[n]->kind].X; i >= 0; i--)
			for(int j = Chip::sizes[chips[n]->kind].Y; j >= 0; j--)
				swapPoints(chips[n]->location + Point(i, j), chips[n]->location + Point(i, j) + p);
		for(WORD i = 0; i < chips[n]->inputs.size(); i++)
			chips[n]->inputs[i] = chips[n]->inputs[i] + p;
		for(WORD i = 0; i < chips[n]->outputs.size(); i++)
			chips[n]->outputs[i] = chips[n]->outputs[i] + p;
		chips[n]->location = chips[n]->location + p;
		break;
	}
}

void Board::mouse(int state, Point apm) {
	int c = findChip(apm), w = findWire(apm);
	Point p;
	switch(state) {
	case 0:	// none click
		for(WORD i = 0; i < chips.size(); i++) if(chips[i]->selected < 0) chips[i]->selected = ~chips[i]->selected;
		for(WORD i = 0; i < wires.size(); i++) if(wires[i]->selected < 0) wires[i]->selected = ~wires[i]->selected;
		if(w >= 0) {
			wires[w]->selected = ~wires[w]->selected;
		}else if(c >= 0) {
			chips[c]->selected = ~chips[c]->selected;
		}
		break;
	case 1:	// click without shift
		if(w >= 0) {
			if(~wires[w]->selected) nSelectedWire = 0;
			else nSelectedWire = 1;
			wires[w]->selected = !(~wires[w]->selected);
			nSelectedChip = 0;
		}else if(c >= 0) {
			if(~chips[c]->selected) nSelectedChip = 0;
			else nSelectedChip = 1;
			chips[c]->selected = !(~chips[c]->selected);
			nSelectedWire = 0;
		}else {
			nSelectedWire = 0;
			nSelectedChip = 0;
			aPins.clear();
			aPins.push_back(Point(-1, -1));
			aPins.push_back(Point(-1, -1));
			aChips.clear();
			aChips.push_back(-1);
			aChips.push_back(-1);
		}
		for(WORD i = 0; i < chips.size(); i++) if(i != c) chips[i]->selected = 0;
		for(WORD i = 0; i < wires.size(); i++) if(i != w) wires[i]->selected = 0;
		break;
	case 2:	// click with shift
		if(w >= 0) {
			if(~wires[w]->selected) nSelectedWire--;
			else nSelectedWire++;
			wires[w]->selected = !(~wires[w]->selected);
		}else if(c >= 0) {
			if(~chips[c]->selected) nSelectedChip--;
			else nSelectedChip++;
			chips[c]->selected = !(~chips[c]->selected);
		}
		break;
	case 3:	// right
		//if(c < 0) {
			p = A2L(apm + Point(ppp / 2, -ppp / 2));
			if(selectedPin.X < 0) {
				selectedPin = p;
			}else {
				newWire(selectedPin, p);
				selectedPin = Point(-1, -1);
			}
		//}
		break;
	case 4:	// alt
		p = A2L(apm + Point(ppp / 2, -ppp / 2));
		if(c < 0) {	// no chip
			aChips.clear();
			aChips.push_back(-1);
			aChips.push_back(-1);
			aPins.erase(aPins.begin());
			aPins.push_back(p);
		}else {	// chip
			aPins.clear();
			aPins.push_back(Point(-1, -1));
			aPins.push_back(Point(-1, -1));
			aChips.erase(aChips.begin());
			aChips.push_back(c);
		}
		break;
	}
}

void Board::newChip(Kind kind, Point pm) {
	if(pm.X >= 0 && pm.Y >= 0 && pm.X + Chip::sizes[kind].X < size.X && pm.Y + Chip::sizes[kind].Y < size.Y) {
		chips.push_back(new Chip(kind, pm));
	}
}

void Board::newWire(Point pb, Point pe) {
	if(pb.X >= 0 && pb.Y >= 0 && pb.X < size.X && pb.Y < size.Y) {
		if(pe.X >= 0 && pe.Y >= 0 && pe.X < size.X && pe.Y < size.Y) {
			if(pb != pe) wires.push_back(new Wire(pb, pe));
		}
	}
}

void Board::delSelected() {
	aPins.clear();
	aPins.push_back(Point(-1, -1));
	aPins.push_back(Point(-1, -1));
	aChips.clear();
	aChips.push_back(-1);
	aChips.push_back(-1);
	for(std::vector<Chip*>::iterator iter = chips.begin(); iter != chips.end();) {
		if((*iter)->selected == 1 || ~(*iter)->selected == 1) {
			delete (*iter);
			iter = chips.erase(iter);
			nSelectedChip--;
		}else {
			iter++;
		}
	}
	for(std::vector<Wire*>::iterator iter = wires.begin(); iter != wires.end();) {
		if((*iter)->selected == 1 || ~(*iter)->selected == 1) {
			delete (*iter);
			iter = wires.erase(iter);
			nSelectedWire--;
		}else {
			iter++;
		}
	}
}

std::string Board::toString() {
	std::string str;
	char buf[64];
	sprintf_s(buf, "%d %d\n", size.X, size.Y); str += buf;
	sprintf_s(buf, "%d\n", chips.size()); str += buf;
	for(WORD i = 0; i < chips.size(); i++) {
		sprintf_s(buf, "%d %d %d\n", chips[i]->kind, chips[i]->location.X, chips[i]->location.Y);
		str += buf;
	}
	sprintf_s(buf, "%d\n", wires.size()); str += buf;
	for(WORD i = 0; i < wires.size(); i++) {
		sprintf_s(buf, "%d %d %d %d\n", wires[i]->input.X, wires[i]->input.Y, wires[i]->output.X, wires[i]->output.Y);
		str += buf;
	}
	sprintf_s(buf, "%d\n", buses.size()); str += buf;
	for(WORD i = 0; i < buses.size(); i++) {
		sprintf_s(buf, "%d\n", buses[i]);
		str += buf;
	}
	return str;
}
