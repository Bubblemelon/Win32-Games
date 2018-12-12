#include <windows.h>
#include <windowsx.h>
#include <malloc.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <fstream>
using namespace std;
#include <direct.h>
#include <commdlg.h>
#include <malloc.h>
#include <cmath>
#include <string.h>
#include <tchar.h>
#include <vector>

// definitions
#define TIMER1 1

// global variables
HINSTANCE hInst; //	program instance
HFONT editfont; //	a font to write
HWND hMain = NULL; //	number of windows = handle window = hwnd
static char MainWin[] = "MainWin"; //	class name
int dx = 0, dy = 0; // mouse position

// to store paint brush colors
vector<COLORREF> colors;
int colorId = 0;

class Painted_pixels
{
public:
	int x;
	int y;
	COLORREF color;
};

vector<Painted_pixels> paintedPixels;
bool painting = false;
bool clear = false; // to clear drawings

// function prototypes
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void draw_line(HDC DC, int x, int y, int a, int b, COLORREF color);

// program entry
int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance, //	in case this program is called from within another program
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{

	hInst = hInstance;

	MSG msg = {0};

	//fill description of WNDCLASSEX
	WNDCLASSEX wcex;
	BOOL Result = TRUE;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, NULL);
	//wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);  // cursor style
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // set Background color as white but will be override in OnPaint
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = MainWin;
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

	// register the window
	Result = (RegisterClassEx(&wcex) != 0);

	hMain = CreateWindow(MainWin, "Bubblemelon Paint on Chalk Board App", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, hInst, NULL);
	if (hMain==0)	return 0;

	// Show the window and send a WM_PAINT message to the window
	// procedure.
	// https://docs.microsoft.com/en-us/windows/desktop/winmsg/using-window-classes
	ShowWindow(hMain, nCmdShow);
	UpdateWindow(hMain);


	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

// to update the window whenever the timer expires
void redr_win_full(HWND hwnd, bool erase)
{
	RECT rt;

	// Retrieves the coordinates of a window's client area
	GetClientRect(hwnd, &rt);

	// adds a rectangle to the specified window's update region. The update region represents the portion of the window's client area that must be redrawn
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-invalidaterect
	InvalidateRect(hwnd, &rt, erase);
}

///////////////////////////////////
//		This Function is called every time the Left Mouse Button is down
///////////////////////////////////
void OnLBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
		painting = true;
}

///////////////////////////////////
//		This Function is called every time the Right Mouse Button is down
///////////////////////////////////
void OnRBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	// to change pen colors
	if (colorId > 0)
	{
		colorId--;
	}
	else {
		colorId = colors.size() - 1;
	}

}

///////////////////////////////////
//		This Function is called every time a character key is pressed
///////////////////////////////////
void OnChar(HWND hwnd, UINT ch, int cRepeat)
{

}

///////////////////////////////////
//		This Function is called every time the Left Mouse Button is up
///////////////////////////////////
void OnLBU(HWND hwnd, int x, int y, UINT keyFlags)
{

	painting = false;
	Painted_pixels skipOverElement;
	skipOverElement.x = -1;
	paintedPixels.push_back(skipOverElement);

	// if mouse position is more than 250 on the window
	// then exit
	//if (x > 250)
	//{
	//	PostQuitMessage(0);
	//}

}

///////////////////////////////////
//		This Function is called every time the Right Mouse Button is up
///////////////////////////////////
void OnRBU(HWND hwnd, int x, int y, UINT keyFlags)
{

}

///////////////////////////////////
//		This Function is called every time the Mouse Moves
///////////////////////////////////
void OnMM(HWND hwnd, int x, int y, UINT keyFlags)
{

	dx = x;
	dy = y;

	if (painting) {

		Painted_pixels pixel;

		// store x y mouse coordinates
		pixel.x = x;
		pixel.y = y;

		// store the current color
		pixel.color = colors[colorId];

		paintedPixels.push_back(pixel);
	}

	if ((keyFlags & MK_LBUTTON) == MK_LBUTTON)
	{
	}

	if ((keyFlags & MK_RBUTTON) == MK_RBUTTON)
	{
	}

	OutputDebugStringA("mousing moving \n");
}

///////////////////////////////////
// Invoked once at the beginning of the program
//
// Called when the WndProc() function
// receives the `WM_CREATE` message from the
// message loop, e.g. when the message loop
// sends a `WM_CREATE` message returned by
// UpdateWindow().
//
// The original OnCreate as according to
// the API, is called when the plug-in
// window is first created.
//
// https://docs.microsoft.com/en-us/windows/desktop/wmp/the-oncreate-method
//
// This OnCreate function is overloaded.
///////////////////////////////////
BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
	hMain = hwnd;

	// This timer is specific to a window, i.e. hwnd
	// TIMER 1 is a non-zero identifier
	// timeout value in miliseconds
	// pointer to function that will be called when timeout value elapses
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-settimer
	if (!SetTimer(hwnd, TIMER1, 20, NULL))
	{
		MessageBox(hwnd, "No Timers Available", "Info", MB_OK);
		return FALSE;
	}

	// initializes colors to be placed into vector
	colors.push_back(RGB(0, 0, 0));			//Black
	colors.push_back(RGB(255, 0, 0));		//Red
	colors.push_back(RGB(0, 255, 0));		//Green
	colors.push_back(RGB(0, 0, 255));		//Blue
	colors.push_back(RGB(255, 255, 255));	//White

	colors.push_back(RGB(244, 66, 241));	//pink
	colors.push_back(RGB(244, 167, 66));	//orange
	colors.push_back(RGB(204, 0, 255));	    //purple
	colors.push_back(RGB(0, 255, 255));	    //aqua
	colors.push_back(RGB(255, 250, 5));	    //yellow

	return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HWND hwin;

	switch (id)
	{
	default:
		break;
	}

}

void OnTimer(HWND hwnd, UINT id)
{
	// redraws the background every 20 miliseconds as specied in SetTimer
	redr_win_full(hwnd, FALSE); 
}

///////////////////////////////////
//		This Function is called every time the window has to be painted again
///////////////////////////////////
void OnPaint(HWND hwnd)
	{
	// information can be used to paint the client area of a window owned by that application
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-tagpaintstruct
	PAINTSTRUCT PaintStruct;

  // device context handle
	// begins painting (prepares the specified window for painting) by filling paintstruct information about painting
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-beginpaint
	HDC DC_ = BeginPaint(hwnd, &PaintStruct);

	// bitmap handles
	HBITMAP hbmMem, hbmOld;

	// creates a memory device context (DC) compatible with the specified device
	// https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createcompatibledc
	HDC DC = CreateCompatibleDC(DC_);

	// DEVICE CONTEXT
	// a structure that defines a set of graphic objects and their associated attributes,
	// as well as the graphic modes that affect output.
	// It tell the Graphic Display Interface (GDI) what to do
	// https://docs.microsoft.com/en-us/windows/desktop/gdi/device-contexts

	RECT rc;
	// get the coordinates of client area, see line 110
	GetClientRect(hwnd, &rc);

	// creates a bitmap compatible with the device that is associated with the specified device context
	// https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createcompatiblebitmap
	// to get the width, right(x-coor) minus left(x-coord) or rectangle. positive in right direction.
	// https://docs.microsoft.com/en-us/previous-versions/dd162897(v%3Dvs.85)
	hbmMem = CreateCompatibleBitmap(DC_, rc.right - rc.left, 2000);

	// selects object into device context, see line 318
	hbmOld = (HBITMAP)SelectObject(DC, hbmMem);

	// window background color
	COLORREF bg = RGB(47, 79, 79);
	HBRUSH hbrBkGnd = CreateSolidBrush(bg);
	FillRect(DC, &rc, hbrBkGnd);
	DeleteObject(hbrBkGnd);

	if (clear) {
		paintedPixels.clear();
		clear = false;
	}

	for (int i = 1; i < paintedPixels.size(); ++i)
	{
		 // if mouse x coordinate is -1, skip over an element
		 // see line 160
		if (paintedPixels[i].x == -1) {
			i++;
			continue;
		}

		draw_line(DC, paintedPixels[i - 1].x, paintedPixels[i - 1].y, paintedPixels[i].x, paintedPixels[i].y, paintedPixels[i].color);

	}

//	DOUBLE BUFFERING
	BitBlt(DC_, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, DC, 0, 0, SRCCOPY);
	SelectObject(DC, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(DC);
	EndPaint(hwnd, &PaintStruct);

}

void draw_line(HDC DC,int x, int y, int a, int b, COLORREF color)
{
	// creates a logical pen that has the specified style, width and color.
	// https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createpen
	HPEN pen = CreatePen(PS_SOLID, 3, color);

	// selects object into device context
	// https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-selectobject
	SelectObject(DC, pen);

	// updates current position to specified point
	// https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-movetoex
	MoveToEx(DC, x, y, NULL);

	// draws line from current position up to but not including specified point
	// https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-lineto
	LineTo(DC, a, b);

	// deletes a logical pen to free system resources associated with object
	// https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-deleteobject
	DeleteObject(pen);

}

//*************************************************************************
void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{

	switch (vk)
		{

			case 67: // c
				// to clear drawing
				clear = true;
				break;

			default:
				break;

		}
}

//*************************************************************************
void OnKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	switch (vk)
		{
			default:
				break;
		}
}

//**************************************************************************
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

    /*
	#define HANDLE_MSG(hwnd, message, fn)    \
    case (message): return HANDLE_##message((hwnd), (wParam), (lParam), (fn))
	*/

		HANDLE_MSG(hwnd, WM_CHAR, OnChar); // when a key is pressed and its a character
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLBD); // when pressing the left button
		HANDLE_MSG(hwnd, WM_LBUTTONUP, OnLBU); // when releasing the left button
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMM); // when moving the mouse inside your window
		HANDLE_MSG(hwnd, WM_RBUTTONDOWN, OnRBD);
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate); // called only once when the window is created
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint); // drawing/render
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand); // not used
		HANDLE_MSG(hwnd, WM_KEYDOWN, OnKeyDown); // press a keyboard key
		HANDLE_MSG(hwnd, WM_KEYUP, OnKeyUp); // release a keyboard key
		HANDLE_MSG(hwnd, WM_TIMER, OnTimer); // timer

	case WM_ERASEBKGND:
		return (LRESULT)1;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
