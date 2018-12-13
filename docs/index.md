# Win32 API Basics

## Inside WinMain

```c++
//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    WNDCLASSEX wcex;
    HWND hwnd;
    MSG Msg;

    // Register the Window
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = 0;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = g_szClassName;
    wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Window Title",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
```

## Register Class and Create Window

[`WINDCLASSEX`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-tagwndclassexa)

```c++
typedef struct tagWNDCLASSEXA {
  UINT      cbSize;  // structure size
  UINT      style;
  WNDPROC   lpfnWndProc;  // pointer to window procedure
  // extra bytes
  int       cbClsExtra; // class structure
  int       cbWndExtra; // window instance
  HINSTANCE hInstance;  // handle to the instance that contains winproc() for class
  HICON     hIcon;
  HCURSOR   hCursor;
  HBRUSH    hbrBackground;
  LPCSTR    lpszMenuName;  // resource name of class menu
  LPCSTR    lpszClassName; // window class name
  HICON     hIconSm; // small icon
} WNDCLASSEXA, *PWNDCLASSEXA, *NPWNDCLASSEXA, *LPWNDCLASSEXA;
```

The `WINDCLASSEX` structure will contain information about the window's fields i.e. the pointer to the Window Procedure which controls the window, window and class extra allocated memory, small and large icons for the window, the window’s background color, window styles, the window’s cursor, the menu resource name, its own name, the instance’s [handle (to read/write this resource)](https://docs.microsoft.com/en-us/windows/desktop/SysInfo/handles-and-objects) and the size of the class itself.

An instance of `WINDCLASSEX` will be used with [`RegisterClassEx`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-registerclassexa) to register a window and for subsequent calls in `CreateWindow`.

`hInstance` serves as a resource handle for the current instance of the application. It is needed to examine or modify a system resource e.g. when creating the window.

#### Note  
It is useful for creating as many of the same window (with same fields) necessary, by having to only to register a particular `WINDCLASSEX` object once.

Would be better to separate registering and creating the window and place inside a `InitWindow` function, which is called within `WinMain`. See this link!

TODO :  ADD LINK ^^^

### Creating the Window

[`CreateWindowEx`](https://msdn.microsoft.com/en-us/library/ms960010.aspx)

Passing `g_szClassName`(the name of the WINDCLASSEX object) into `CreateWindowEx` tells the system to use `g_szClassName`'s associated attributes/fields to create a window (as in, `g_szClassName` points to which window identity to create).

Returns [`HWND`](https://docs.microsoft.com/en-us/windows/desktop/winprog/windows-data-types), a Windows data type that is a handle to a window object.

## Show the Window

```C
ShowWindow(hwnd, nCmdShow);
UpdateWindow(hwnd);
```

After creating the window it is necessary to call the [`ShowWindow`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-showwindow) function. This function specifies the state of a specific window (through the window handler i.e. `hwnd`).

The state is determined by the value of `nCmdShow`. For example, when `nCmdShow` has the integer value `3`, this maximizes the specific window.

The [`UpdateWindow`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-updatewindow) function updates the client area of the specified window by sending a `WM_PAINT` message to the window if the window's update region is not empty. The function sends a `WM_PAINT` message directly to the window procedure of the specified window, bypassing the application queue. If the update region is empty, no message is sent.

## The Message Loop

### [`GetMessage`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-getmessage)

Retrieves the next available [system/application generated message](https://docs.microsoft.com/en-us/windows/desktop/winmsg/about-messages-and-message-queues) (e.g. may contain information about mouse movement, or keyboard input) from the front of the  application's (calling thread's) message queue - FIFO order.

Copies the message to structure of type [`MSG`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-tagmsg).

These messages are placed on the message queue (specific to a thread of a window) by the `PostMessage` function.

Returns a non-zero values unless [WM_QUIT](https://docs.microsoft.com/en-us/windows/desktop/winmsg/wm-quit) is encountered. If an error is occurs, `GetMessage` will return `-1`.

#### Note  
A `BOOL` is defined as a unsigned int. True would equate to `1` while false would be `0`.

### [`PeekMessage`](https://docs.microsoft.com/en-us/windows/desktop/winmsg/using-messages-and-message-queues#examining-a-message-queue)

Similar to [`GetMessage`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-peekmessagea).

Both are used to check for a message on the queue that matches the filter criteria i.e. parameters `wMsgFilterMin` and `wMsgFilterMax` of type `UNIT`.

`GetMessage` does not return until a message matching the filter criteria as specified in the parameters is placed in the queue.

Whereas, [`PeekMessage`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-peekmessagea) returns immediately regardless of whether a message is in the queue.

Reference:  
- [Examining a Message Queue](https://docs.microsoft.com/en-us/windows/desktop/winmsg/using-messages-and-message-queues#examining-a-message-queue)    

#### Note
Notice that whenever `HWND` is present/used, it is to specify a specific window.

### `TranslateMessage`

Does additional processing on the received messages. It is necessary if the thread is to receive character input from keyboard.

Translates virtual-key messages i.e. [`WM_KEYDOWN`](https://msdn.microsoft.com/library/windows/desktop/ms646280) and [`WM_KEYUP`](https://msdn.microsoft.com/library/windows/desktop/ms646281) generated from the system into a character message i.e. [`WM_CHAR`](https://msdn.microsoft.com/library/windows/desktop/ms646276) and _places it back into message queue_.

### [`DispatchMessage`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-dispatchmessage)

Releases/sends a message to the window's (the one that originally sent that message i.e. as specified by `HWND`) window procedure as specified in `MSG`.

Therefore, only one message loop is needed since `DispatchMessage` will always dispatch the message to the appropriate window.

My Message Loop Analogy:

> It is almost like a filtering process involving `GetMessage` the receiver, `TranslateMessage` the mediator, then finally `DispatchMessage` the sender, all within the application layer which sends off the message to the application window layer.  
>
> The Message Loop is like the heart of the application.

### Window Messages (Notifications)

`wParam` and `lParam` are fields of the [MSG](https://msdn.microsoft.com/en-us/library/ms930610.aspx) structure. Which is passed to the window procedure after being dispatched from the message loop.

Both specify additional information about the message. The exact meaning depends on the values of the message member.

For example, `WM_CREATE` is defined as:

```c
#define WM_CREATE                      0x0001
```

The identifier of a message is passed to the window procedure as the [`UINT` parameter](https://goo.gl/kxihon).

Depending on the purpose of the message, some messages will use one of the two parameters (either `wParam` or `lParam`) of `WndProc`, both or none. This applies to `DefWindowProc` as well.

For example, [`WM_CLOSE`](https://docs.microsoft.com/en-us/windows/desktop/winmsg/wm-close) uses neither `wParam` or `lParam`. Hence, it is `0` for both.


```c
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
```

`UINT_PTR` is an unsigned `INT_PTR`

```c
#if defined(_WIN64)
 typedef unsigned __int64 UINT_PTR;
#else
 typedef unsigned int UINT_PTR;
#endif
```

Returns a `LRESULT` type.

```c
typedef LONG_PTR LRESULT;
```

`LONG_PTR` is a signed long type for pointer precision.

```c
#if defined(_WIN64)
 typedef __int64 LONG_PTR;
#else
 typedef long LONG_PTR;
#endif  
```

Reference:  
- [Naming Convention of Types such as LRESULT and What They Mean](https://stackoverflow.com/questions/17851348/what-do-lresult-wparam-and-lparam-mean)   
-  [Windows Data Types](https://docs.microsoft.com/en-us/windows/desktop/WinProg/windows-data-types)  

### [`PostMessage`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-postmessagea)

Places (posts) a message in the message queue associated with the thread that created the specified window.

Returns true (non-zero) if succeeds and without waiting for the thread to process the message.

For example, the same effect as clicking on the close button ❎, would be to call `PostMessage(hwnd, WM_CLOSE, 0, 0)`.

### [`SendMessage`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-sendmessage)

Similar to `PostMessage`, both are used to send messages to the message queue.

However, `PostMessage` only puts the message in the message queue and then immediately returns.

While the `SendMessage`  function calls the window procedure for the specified window and does not return until the window procedure has processed the message.

In other words, `SendMessage` does not send the message to the message queue.

### To Terminate the Message Loop

**Briefly**:  
An application can end its own loop by using the `PostQuitMessage` function, typically in response to the `WM_DESTROY` message in the window procedure of the application's main window.

**In other words:**  
The message loop continues to retrieve messages from the thread's message queue and to dispatch them to the appropriate windows. The message loop ends when the `GetMessage` function removes the `WM_QUIT` message from the message queue.

**Explained**:   
When the window procedure receives a `WM_CLOSE` message this would in turn call the `DestroyWindow` function. This tells the window that the message is to destroy its child windows and then itself.

This triggers a [`WM_DESTROY`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-destroywindow) message to be dispatched to the window procedure. As a result, `PostQuitMessage` will be called which sends a `WM_QUIT` that never makes it to the window procedure as this causes the `GetMessage` while loop to not run, which means that `DispatchMessage` doesn’t get called. The purpose `WM_QUIT` is essentially to stop `GetMessage` or return false.


Reference:
- [Message Loop Microsoft Docs](https://docs.microsoft.com/en-us/windows/desktop/winmsg/about-messages-and-message-queues#message-loop)     
- [Understanding the Message Loop](http://www.winprog.org/tutorial/message_loop.html)    

## The Window Procedure

```c
//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}
```
A function that receives and processes all messages sent to a particular window as specified by `HWND`.

Returns type `LRESULT` which indicates the success or failure of the function.

`CALLBACK` is an identifier that specifies `WndProc` as a _callback_ function. This means that the OS (Windows) will be calling this function outside of the code space of the program. `WndProc` is never explicitly called in the application code.

See this [Callback Functions](https://docs.microsoft.com/en-us/dotnet/framework/interop/callback-functions) for more information, there will be mention of [Dynamic Link Library (DLL)](https://support.microsoft.com/en-us/help/815065/what-is-a-dll) functions.

Every window class has a window procedure, and every window created with that class uses that same window procedure to respond to messages.

[`DefWindowProc`](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-defwindowproca) handles/processes default actions (or actions not handled by `WndProc` are passed to it) and returns a message result.

My WndProc Analogy:

> The Window Procedure is like the brain of the application.

Reference:  
- [Window Procedure Microsoft Docs](https://docs.microsoft.com/en-us/windows/desktop/winmsg/about-messages-and-message-queues#window-procedure)   

## Why Function Names in Win32 end with `A` and `W`

 Functions with `A` use [`ANSI`](https://stackoverflow.com/questions/701882/what-is-ansi-format) strings as input and output while functions with `W` use [Unicode](https://unicodebook.readthedocs.io/definitions.html#character-string) strings.

 Reference:  
- [Unicode and ANSI Functions](https://stackoverflow.com/questions/7424383/what-is-the-difference-between-the-a-and-w-functions-in-the-win32-api)    
- [Stackoverflow Discussion on fucntionsA and functionsW](https://stackoverflow.com/questions/7424383/what-is-the-difference-between-the-a-and-w-functions-in-the-win32-api)  

## References Overall
[A Simple Window](http://www.winprog.org/tutorial/simple_window.html)

[DirectX11 Sample Browser Tutorial 1](https://code.msdn.microsoft.com/Direct3D-Tutorial-Win32-829979ef)
