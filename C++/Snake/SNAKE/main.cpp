#undef UNICODE
#include <windows.h>
#include <stdio.h>
#include <vector>
#include "resource.h"
#include <ctime>

using namespace std;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

LPCSTR szClassName = "WinAPI";
LPCSTR szTitle =     "Snake";
HINSTANCE hInst;

enum Direction{LEFT, UP, DOWN, RIGHT};

Direction opposite(Direction dir)
{
	return Direction(3 - dir);
}

const int CELLSIZE = 20;
const int BODYSIZE = 10, HEADSIZE = BODYSIZE + 2; 
const int STEP = CELLSIZE;
const COLORREF BODYCOLOR = RGB(9*16+12,11*16+9,4*16+1), HEADCOLOR = RGB(11*16+9,9*16+9,4*16+1);
const int DEFFOODSIZE = BODYSIZE;
const COLORREF DEFFOODCOLOR = RGB(9*16+9,6*16+6,12*16+12);

pair<int, int> getNewCrd(pair<int, int> crd, Direction dir, int step)
{
	switch (dir)
	{
	case LEFT:
		crd.first -= step;
		break;
	case UP:
		crd.second -= step;
		break;
	case RIGHT:
		crd.first += step;
		break;
	case DOWN:
		crd.second += step;
		break;
	}
	return crd;
}

class ObjectOnMap
{

protected:

	pair<int, int> crd;
	int size;
	COLORREF color;

	ObjectOnMap()
	{}

	ObjectOnMap(pair<int, int> a, int b, COLORREF c)
	{
		crd = a;
		size = b;
		color = c;
	}

	virtual void draw(HDC hdc) = 0;

public:

	pair<int, int> getCrd()
	{
		return crd;
	}
};

class BodyCell : public ObjectOnMap
{
	Direction dir;

public:

	BodyCell(pair<int, int> crd, int sz, COLORREF cl, Direction d) : ObjectOnMap(crd, sz, cl), dir(d)
	{}

	void moveToDir()
	{
		crd = getNewCrd(crd, dir, STEP);
	}

	void changeDir(Direction newdir)
	{
		if (newdir != opposite(dir))
			dir = newdir;
	}

	Direction getDir()
	{
		return dir;
	}

	void draw(HDC hdc) 
	{
		HBRUSH hBrush = CreateSolidBrush(color);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, crd.first - size, crd.second - size, crd.first + size, crd.second + size);
		DeleteObject(hBrush);
		MoveToEx(hdc, crd.first, crd.second, 0);
		LineTo(hdc, getNewCrd(crd, dir, size/2).first, getNewCrd(crd, dir, size/2).second); 
	}

};

class Food : public ObjectOnMap
{

public:

	Food()
	{}

	Food(pair<int, int> crd, int sz, COLORREF cl) : ObjectOnMap(crd , sz, cl)
	{}

	void draw(HDC hdc) 
	{
		HBRUSH hBrush = CreateSolidBrush(color);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, crd.first - size, crd.second - size, crd.first + size, crd.second + size);
		DeleteObject(hBrush);
	}

	void set(pair<int, int> newcr, int sz, COLORREF cl)
	{
		crd = newcr;
		size = sz;
		color = cl;
	}

};

class Snake 
{
	vector<BodyCell> body;

public:

	Snake()
	{
	}

	void init()
	{
		BodyCell head(make_pair(100, 100), HEADSIZE, HEADCOLOR, DOWN);
		body.push_back(head);
		body.push_back(BodyCell(make_pair(100, 100 - STEP), BODYSIZE, BODYCOLOR, DOWN));
		body.push_back(BodyCell(make_pair(100, 100 - 2*STEP), BODYSIZE, BODYCOLOR, DOWN));
	}
	
	void clear()
	{
		body.clear();
	}

	void addBodyCell()
	{
		BodyCell tail = body[body.size()-1];
		body.push_back(BodyCell(getNewCrd(tail.getCrd(), opposite(tail.getDir()), STEP), BODYSIZE, BODYCOLOR, tail.getDir()));
	}

	void changeDir(Direction newdir)
	{
		body[0].changeDir(newdir);
	}

	void move()
	{
		body[0].moveToDir();
		for (int i = body.size() - 1; i > 0; i--)
		{
			body[i].moveToDir();
			body[i].changeDir(body[i-1].getDir());
		}
	}

	bool isOnCrd(pair<int, int> crd)
	{
		return body[0].getCrd() == crd;
	}

	void draw(HDC hdc)
	{
		for (int i = 0; i < body.size(); i++)
			body[i].draw(hdc);
	}

	bool isSelfCrossed()
	{
		for (int i = 1; i < body.size(); i++)
			if (body[0].getCrd() == body[i].getCrd())
				return true;
		return false;
	}
};

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	MSG msg;
	hInst = hInstance;
	if (!hPrevInstance) 
	{ 
		if (!InitApplication(hInstance)) 
		return (FALSE);
	}
	if (!InitInstance(hInstance, nCmdShow)) 
		return (FALSE);  
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
    	DispatchMessage(&msg);
	}
	return (msg.wParam);
}

BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASS  wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_ASTERISK);
	wc.hCursor       = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)(WHITE_BRUSH);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = szClassName;
	return RegisterClass(&wc);
} 

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hWnd = CreateWindow(
    szClassName,         
    szTitle,             
    WS_OVERLAPPEDWINDOW, 
    CW_USEDEFAULT,      
    CW_USEDEFAULT,      
    500,       
    500,       
    NULL,                
    NULL,               
    hInstance,           
    NULL);               

	if (!hWnd)    return (FALSE); 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return (TRUE);
} 
 
pair<int, int> setRandomCellCrd(int maxx, int maxy)
{
	int newx = (rand() % maxx) / CELLSIZE;
	int newy = (rand() % maxy) / CELLSIZE;
	if (newx == 0)
		newx++;
	if (newx == maxx / CELLSIZE)
		newx--;
	if (newy == 0)
		newy++;
	if (newy == maxy / CELLSIZE)
		newy--;
	return make_pair(newx * CELLSIZE, newy * CELLSIZE);
}

INT_PTR CALLBACK aboutDialogFunc(HWND hwndAbout, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwndAbout, 0);
			return FALSE;
		default:
			{};
		}
		break;
	case WM_CLOSE:
		EndDialog(hwndAbout, 0);
		return FALSE;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{  
	static Snake snake;
	static Food food;
	static bool isPlaying;
	static bool isPressed;  
   	HDC hDC;
	PAINTSTRUCT ps;
	RECT wnr;
	switch(message)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case ID_FILE_EXIT:
			DestroyWindow(hwnd);
			break;
		case ID_FILE_START:
		{
			isPlaying = true;
			snake.clear();
			snake.init();
			GetClientRect(hwnd, &wnr);
			food.set(setRandomCellCrd(wnr.right, wnr.bottom), DEFFOODSIZE, DEFFOODCOLOR);
			int nTime = SetTimer(hwnd, 1, 100, NULL);
		}	
			break;
		case ID_HELP_ABOUT:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_ABOUTDIALOG), hwnd, aboutDialogFunc, 0); 
			break;
		}
		break;
	case WM_KEYDOWN:
		if (isPlaying)
		{
			if (!isPressed)
			{
				isPressed = true;
				switch(wParam)
				{
				case VK_LEFT:
					snake.changeDir(LEFT);
					break;
				case VK_UP:
					snake.changeDir(UP);
					break;
				case VK_RIGHT:
					snake.changeDir(RIGHT);
					break;
				case VK_DOWN:
					snake.changeDir(DOWN);
					break;
				}
			}
		}
		break;
	case WM_TIMER:
		if (wParam == 1)
		{
			snake.move();
			if (snake.isSelfCrossed())
			{
				isPlaying = false;
				KillTimer(hwnd, 1);
				MessageBox(hwnd, "Self-crossed", "End of game", MB_OK);
			}
			isPressed = false;
			if (snake.isOnCrd(food.getCrd()))
			{
				snake.addBodyCell();
				GetClientRect(hwnd, &wnr);
				food.set(setRandomCellCrd(wnr.right, wnr.bottom), DEFFOODSIZE, DEFFOODCOLOR);
			}
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
	case WM_CREATE:
		srand(time(0));
		break;
    case WM_PAINT:
	    GetClientRect(hwnd, &wnr);
		hDC = BeginPaint(hwnd, &ps);
		FillRect(hDC, &wnr, WHITE_BRUSH);
		food.draw(hDC);
		snake.draw(hDC);
		EndPaint(hwnd, &ps);
		break;
    case WM_DESTROY:
	    PostQuitMessage(0);
		break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
