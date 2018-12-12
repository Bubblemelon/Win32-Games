# Paint on Chalk Board App

Contains additional notes on extending functionality of this application.

## To Draw a Shape using Lines

Using the `draw_line` function from [`PaintApp.cpp`](https://github.com/Bubblemelon/Win32-Games/blob/5df8d5b5c11aa795a0d35761e2ffb16d5ace6f1d/Paint-on-Chalk-Board-App/winproject/viewserver/PainApp.cpp#L349):

```c
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
```
For example to draw a Star that shows up on the window, call the following lines within the `OnPaint` function after creating a compatible device context.

It is possible to insert these `draw_line` calls after this line in the code on [`PaintApp.cpp`](https://github.com/Bubblemelon/Win32-Games/blob/5df8d5b5c11aa795a0d35761e2ffb16d5ace6f1d/Paint-on-Chalk-Board-App/winproject/viewserver/PainApp.cpp#L295) or [after this line](https://github.com/Bubblemelon/Win32-Games/blob/5df8d5b5c11aa795a0d35761e2ffb16d5ace6f1d/Paint-on-Chalk-Board-App/winproject/viewserver/PainApp.cpp#L321) as well.

```C
draw_line(DC, 325, 600, 375, 450, 244, 244, 66, 8);
draw_line(DC, 375, 450, 425, 600, 244, 244, 66, 8);
draw_line(DC, 425, 600, 300, 500, 244, 244, 66, 8);
draw_line(DC, 325, 600, 445, 500, 244, 244, 66, 8);
draw_line(DC, 300, 500, 445, 500, 244, 244, 66, 8);
```
Each dr
