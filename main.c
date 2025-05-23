#include <windows.h>

#define STARTBUTTON 1
#define INPUTNUMBERFIELD 2
#define CHECKBOX_FIXED 3
#define CHECKBOX_DYNAMIC 4
#define DYNAMIC_X 5
#define DYNAMIC_Y 6
HHOOK _k_hook;
//start/stop
//fixed position or pick location
HWND startbutton;
HWND inputNumbersField;
HWND checkboxFixed;
HWND checkboxDynamic;
HWND labelCheckbox;
HWND dynamicX;
HWND dynamicY;
HWND mainwindow;
volatile int working = 0;
void addMenus(HWND hwnd);
void checkboxFunctioning(int controlIdfixed, int controlIddynamic, HWND hwnd);
DWORD WINAPI autoClick(LPVOID lpParam);

LRESULT __stdcall k_Callback(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
    //a key was pressed
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
    {
        switch (key->vkCode)
        {
        
        case 'W':
            working = !working;
            if (working)
            {
                SetWindowText(startbutton, L"STOP");
                CreateThread(NULL, 0, autoClick, mainwindow, 0, NULL);
            }
            else
            {
                SetWindowText(startbutton, L"START");
            }
            break;
           
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg)
    {

    case WM_CHAR:
        switch (wParam)
        {
        case 119:
       
            break;
        }
        break;


    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;

    }
    break;
    case WM_CREATE:
    {
        addMenus(hwnd);
        return 0;
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    break;
        case WM_COMMAND:
        {
        int id = LOWORD(wParam);//id of the controlled 'window'
        int code = HIWORD(wParam); //action
        switch (id)
        {

        case STARTBUTTON: 
        {
            working = !working;
            if (working)
            {
                SetWindowText(startbutton, L"STOP");
                CreateThread(NULL, 0, autoClick, hwnd, 0, NULL);
               // autoClick(hwnd);
                //start clicking based on value from textbox and on the location of the mouse
            }
            else
            {
                SetWindowText(startbutton, L"START");
            }

        }
        break;
        case INPUTNUMBERFIELD:
        {

        }
        break;
        case CHECKBOX_FIXED:
            checkboxFunctioning(CHECKBOX_FIXED, CHECKBOX_DYNAMIC, hwnd);
            break;

        case CHECKBOX_DYNAMIC:
            checkboxFunctioning(CHECKBOX_DYNAMIC, CHECKBOX_FIXED, hwnd);
            break;
        case DYNAMIC_X:
        {

        }
        break;
        case DYNAMIC_Y:
        {

        }
        break;

        }


         }


    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
        break;
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Autoclicker";
    WNDCLASS wc = { 0 }; //main structure for the windows
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc); //register window to operating system


    HWND hwnd = CreateWindowEx(
        0, //styles
        CLASS_NAME, //class, (the mywindowclass)
        L"MyAutoclicker", //text
        WS_OVERLAPPEDWINDOW, //window style
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, //size, position
        NULL, NULL, hInstance, NULL //parent window,Menu,instance handle,additional data
    );

    if (!hwnd) return 0; //unsuccesful
    mainwindow = hwnd;
    ShowWindow(hwnd, nCmdShow);


    ///////////////////////////////////////////////////////initialization done

    SetWindowPos(hwnd, 0, 700, 300, 600, 400, NULL);

    UpdateWindow(hwnd);
    _k_hook = SetWindowsHookEx(WH_KEYBOARD_LL, k_Callback, NULL, 0);
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);



    }
    if (_k_hook)
        UnhookWindowsHookEx(_k_hook);
        
    return 0;
}



void addMenus(HWND hwnd)
{
    startbutton = CreateWindow( //on press working=!working
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Start/Stop",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        50,         // x position 
        200,         // y position 
        125,        // Button width
        45,        // Button height
        hwnd,     // Parent window
        (HMENU)STARTBUTTON,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

    inputNumbersField = CreateWindowEx(WS_EX_PALETTEWINDOW, TEXT("EDIT"), TEXT(""),
        WS_CHILD | WS_VISIBLE  | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | ES_NUMBER,
        50, 150,        120, 20, hwnd, (HMENU)INPUTNUMBERFIELD, NULL, NULL);

   labelCheckbox = CreateWindowEx(
        0,                             
        TEXT("STATIC"),                 
        TEXT("Enter the period in milliseconds:"),   
        WS_CHILD | WS_VISIBLE,       
        50, 80, 120, 45,               // x, y, width, height
        hwnd, NULL, NULL,  NULL );

   checkboxFixed= CreateWindowEx(
       0,                              
       TEXT("BUTTON"),               
       TEXT("Fixed position"),        
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
       250, 200,     133, 25,             
       hwnd, (HMENU)CHECKBOX_FIXED , NULL,NULL
   );

   checkboxDynamic= CreateWindowEx(
       0,
       TEXT("BUTTON"),
       TEXT("Dynamic position"),
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
       250, 150, 133, 25,
       hwnd, (HMENU)CHECKBOX_DYNAMIC, NULL, NULL
   );
   SendMessage(checkboxDynamic, BM_SETCHECK, BST_CHECKED, 0);
        
   dynamicX= CreateWindowEx(WS_EX_PALETTEWINDOW, TEXT("EDIT"), TEXT("x"),
       WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | ES_NUMBER,
       250, 250,   40, 20, hwnd, (HMENU)DYNAMIC_X, NULL, NULL);

   dynamicY= CreateWindowEx(WS_EX_PALETTEWINDOW, TEXT("EDIT"), TEXT("y"),
       WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | ES_NUMBER,
       300, 250, 40, 20, hwnd, (HMENU)DYNAMIC_Y, NULL, NULL);



}


void checkboxFunctioning(int clickedId, int otherId, HWND hwnd)
{
    HWND hClicked = GetDlgItem(hwnd, clickedId);
    HWND hOther = GetDlgItem(hwnd, otherId);

    LRESULT clickedState = SendMessage(hClicked, BM_GETCHECK, 0, 0);

    if (clickedState == BST_CHECKED) {
        
        SendMessage(hOther, BM_SETCHECK, BST_UNCHECKED, 0);
    }
    else {
        
        SendMessage(hClicked, BM_SETCHECK, BST_CHECKED, 0);
    }
}
DWORD WINAPI autoClick( LPVOID lpParam)
{
    HWND hwnd = (HWND)lpParam;
    char buffer[20], bufferx[10], buffery[10];
    GetWindowTextA(inputNumbersField, buffer, 20);
    int period = atoi(buffer);
    if (period <= 200) period = 1000;
        
    GetWindowTextA(dynamicX, bufferx, 10);
    GetWindowTextA(dynamicY, buffery, 10);
    int xloc = atoi(bufferx);
    int yloc = atoi(buffery);
    if (xloc <= 0)
        xloc = 500;
    if (yloc <= 0)
        yloc = 500;
    

    INPUT inputs[2] = {};
       
     
       
            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;


            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            Sleep(500);
            while (working)
            {
                LRESULT fixed = SendMessage(checkboxFixed, BM_GETCHECK, 0, 0);
                if (fixed == BST_CHECKED)
                {
                    SetCursorPos(xloc, yloc);
                }
                SendInput(2, inputs, sizeof(INPUT));
                Sleep(period);
            }
            
        

            return 0;
}