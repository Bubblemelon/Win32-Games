# Paint on Chalk Board App

Contains additional notes on extending the functionality of this [application](https://github.com/Bubblemelon/Win32-Games#paint-on-chalk-board-app).

## To Draw a Shape using Lines

Use the `draw_line` function from [`PaintApp.cpp`](https://github.com/Bubblemelon/Win32-Games/blob/5df8d5b5c11aa795a0d35761e2ffb16d5ace6f1d/Paint-on-Chalk-Board-App/winproject/viewserver/PainApp.cpp#L349):

```c
// Parameters:
// Device context
// current x, y position (could be mouse coordinates)
// where the line should end at coordinates a, b
// color of the line
void draw_line(HDC DC, int x, int y, int a, int b, COLORREF color)
{
	// creates a logical pen that has the specified style, width (thickness) and color.
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
```
For example to draw a Star that shows up on the window, call the following several `draw_line` functions within `OnPaint` after creating a compatible device context.

![Shows an image of a star shaped with yellow lines](/img/star-lines.PNG)

It is possible to insert these `draw_line` calls after this line in the [`PaintApp.cpp`](https://github.com/Bubblemelon/Win32-Games/blob/5df8d5b5c11aa795a0d35761e2ffb16d5ace6f1d/Paint-on-Chalk-Board-App/winproject/viewserver/PainApp.cpp#L295) code or [after this line](https://github.com/Bubblemelon/Win32-Games/blob/5df8d5b5c11aa795a0d35761e2ffb16d5ace6f1d/Paint-on-Chalk-Board-App/winproject/viewserver/PainApp.cpp#L321) as well.

```C
draw_line(DC, 325, 600, 375, 450, yellowline);
draw_line(DC, 375, 450, 425, 600, yellowline);
draw_line(DC, 425, 600, 300, 500, yellowline);
draw_line(DC, 325, 600, 445, 500, yellowline);
draw_line(DC, 300, 500, 445, 500, yellowline);
```

This will create yellow lines of thickness set to 3 pixels, see [`CreatePen`](https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createpen). If a varied length is preferred, create a width parameter for `draw_line` to pass in the desired width.

For the color of the lines, initialize a `COLORREF` variable within `OnPaint` e.g. `COLORREF yellowline = RGB( 244, 244, 66); //yellow`.

`x`, `y`, `a` and `b` are coordinates on the window.

## Have a Shape Move change the Cursor Coordinates

Have global variables `int dx = 0, dy = 0;` initialized and have [place them within `OnMM`](https://github.com/Bubblemelon/Win32-Games/blob/a7b556fbd4212bbcd47b76c754c5f6c84e81f38d/Paint-on-Chalk-Board-App/winproject/viewserver/PaintApp.cpp#L180), a function called in `WndProc` whenever it receives a `WM_MOUSEMOVE` message like so [`HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMM);`](https://github.com/Bubblemelon/Win32-Games/blob/a7b556fbd4212bbcd47b76c754c5f6c84e81f38d/Paint-on-Chalk-Board-App/winproject/viewserver/PaintApp.cpp#L417).

```c
void OnMM(HWND hwnd, int x, int y, UINT keyFlags)
{
	// records current cursor position
	dx = x;
	dy = y;
	.
	.
	.
}
```

### To Create A Rectangle

Place the four `draw_line` calls in a function for organization:

```c
// draws a quadrilateral
void draw_rectangle(HDC DC, int x, int y, int w, int h, COLORREF linecolor, int lineWidth)
{

		draw_line(DC, x, y, w, y, magentaline, lineWidth);
		draw_line(DC, w, y, w, h, magentaline, lineWidth);  
		draw_line(DC, w, h, x, h, magentaline, lineWidth);
		draw_line(DC, x, h, x, y, magentaline, lineWidth);

}
```

Make a call to `draw_rectangle` in `OnPaint`:
`draw_rectangle(DC, dx, dy, dx+50, dy+50, magentaline, 5);`

This call will create a quadrilateral made up of 4 magenta colored lines of thickness 5 pixels, that has equal height and width i.e. `300` from the cursor.

`COLORREF magentaline = RGB( 255, 0, 255 ); //magenta`

![a shapes that moves with the cursor demo](/img/shape-moves-with-cursor-demo.gif)

This call instead will vary the height and width of the quadrilateral with respect to the cursor position, where a corner of the quadrilateral will be fixed.

`draw_rectangle(DC,50, 50, dx, dy, magentaline, 5);`

![a shapes that changes in witdh and height with the cursor demo](/img/shape-changes-height-witdh-with-cursor-demo.gif)


### To Create a Target Mark or Crosshair ➕

These `draw_line` calls creates a cross (a target mark) of 10 pixels for height and width:

```
draw_line(DC, dx - 10, dy, dx + 10, dy, 219, 139, 19, 5);
draw_line(DC, dx, dy - 10, dx, dy + 10, 219, 139, 19, 5);
```

![a cross that moves with the cursor demo](/img/cross-moves-with-cursor-demo.gif)

## Using OnTimer to Blink Shapes

Add the following lines to [OnTimer](https://github.com/Bubblemelon/Win32-Games/blob/931f9ffd5ede3a2853351669a77eba3d088c5159/Paint-on-Chalk-Board-App/winproject/viewserver/PaintApp.cpp#L271) in `PaintApp.cpp`.

```c
void OnTimer(HWND hwnd, UINT id)
{
	static int count = 0;
	count++;

  // showing and hiding (i.e. blinking) every 20 milliseconds is too fast
	if (count >= 10) // called 2 times a second  // 20 miliseconds * 10  = 200 milisec == 2 secs
	{
		count = 0;

		// to switch between showing the crosshair && blinking the star
		if (showcross == 1 || star_blink == 1 ) {
			showcross = 0;
			showstar = 0;
	   }
		else
		{
			showcross = 1;
			showstar = 1;
		}
	}

	// redraws the background every 20 miliseconds
	redr_win_full(hwnd, FALSE);
}
```

`showcross` and `showstar` are global variables (i.e. outside of any function) initialized as such, `int showcross = 1, showstar=1;` These variables can also be Booleans.

Simply wrap around an if statement where drawing the star and crosshair is called within `OnPaint`:

```c
void OnPaint(HWND hwnd)
{
	.
	.
	.
	if (showstar == 1)
			{
				draw_line(DC, 325, 600, 375, 450, 244, 244, 66, 8);
				draw_line(DC, 375, 450, 425, 600, 244, 244, 66, 8);
				draw_line(DC, 425, 600, 300, 500, 244, 244, 66, 8);
				draw_line(DC, 325, 600, 445, 500, 244, 244, 66, 8);
				draw_line(DC, 300, 500, 445, 500, 244, 244, 66, 8);
	}

	if (showcross == 1)
		{
		draw_line(DC, dx - 10, dy, dx + 10, dy, 219, 139, 19, 5);
		draw_line(DC, dx, dy - 10, dx, dy + 10, 219, 139, 19, 5);
	}
	.
	.
	.
}

```
This logic indicates that only draw the star and crosshair when `showstar` and `showcross` are set to 1. These variables are switching between `1` and `0` very `2` seconds within `OnTimer`.

![shows a star and a crosshair blinking every 2 seconds](/img/cross-and-star-blink-demo.gif)

## Render Text on the Window

The following snippet is called within `OnPaint` that will render (display the string) a text at coordinates (10, 10).

```c
/*
 * Prints the coordinates of the Cross
 */
	// int to string
	crossPos = "x = " + to_string(dx) + " y= " + to_string(dy);
	// put string in buffer char array
	strcpy(buffer, crossPos.c_str());
	// Output text
	TextOut(DC, 10, 10, buffer, strlen(buffer));
```

Since `crossPos` changes with respect to cursor coordinates, it is a global variable i.e. `string crossPos = "";`. If the display string does not need to change, it is possible to have it within `OnPaint` like so, `crossPos = "hello!";`

![shows cursor coordinates as text in the window](/img/cursor-coordinates-text-demo.gif)

Reference:  
[Win32 Text Output Example](http://www.cplusplus.com/forum/windows/31112/)
