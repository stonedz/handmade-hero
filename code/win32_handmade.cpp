#include <windows.h>
#define internal static
#define global_variable static
#define local_persist static


global_variable bool Running = 1;

internal void
ResizeDIBSection()
{
}

LRESULT CALLBACK
MainWindowCallback(HWND Window,
                   UINT Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
    LRESULT Result = 0;

	switch(Message){
		case WM_SIZE:
		{
            OutputDebugStringA("WM_SIZE\n");
		};
		break;
		case WM_DESTROY:
        {
            ResizeDIBSection();
            Running = false;
		};
		break;
		case WM_CLOSE:
        {
            Running = false;
        };
        break;
		case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        };
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            LONG Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            LONG Width = Paint.rcPaint.right - Paint.rcPaint.left;
            PatBlt(DeviceContext, X, Y, Width, Height, BLACKNESS);
            EndPaint(Window, &Paint);
        }
		default:{
            Result = DefWindowProc(Window, Message, WParam, LParam) ;
        };
        break;

	}

    return(Result);
}

int WINAPI WinMain(HINSTANCE Instance,
                   HINSTANCE hPrevInstance,
                   PSTR lpCmdLine,
                   int intnCmdShow)
{
	WNDCLASS WindowClass = {};
	WindowClass.style = CS_OWNDC;
	WindowClass.lpfnWndProc = MainWindowCallback;
	WindowClass.hInstance = Instance ;
	WindowClass.lpszClassName = "MyTestWindowClass";

    if(RegisterClass(&WindowClass))
    {
        HWND WindowHandle =
            CreateWindowEx(
                           0,
                           WindowClass.lpszClassName,
                           "Test1",
                           WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           0,
                           0,
                           Instance,
                           0);
        if(WindowHandle)
        {
            while(Running)
            {
                MSG Message;
                BOOL Messageresult = GetMessage(&Message, 0,0,0);
                if(Messageresult > 0 )
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            // TODO(Paolo): Logging
        }
    }
    else
    {
        // TODO(Paolo): lOGGING
    }

    return(0);
}
