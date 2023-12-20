#include <windows.h>
#include "Resource.h"
#include "RGZ_OS.h"
#include <tchar.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;                              // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                // текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];          // имя класса главного окна

//для операций сравнивания и т.п.
const DWORD BUTTON_ID1 = 1;
const DWORD BUTTON_ID2 = 2;
const DWORD BUTTON_ID3 = 3;
const DWORD BUTTON_ID4 = 4;
const DWORD TIMER_ID1 = 5;

HWND  StKassa1;                               //состояние кассы
HWND  StKassa2;
HWND  StKassa3;
HWND  PeopleTxt;                              
HWND  PeopleObsl;

Kassa1  KS;                                   //кассы 
Kassa2 KS2;
Kassa3 KS3;
int people0 = 0;                              //количество людей
int n1 = 0, n2 = 0, n3 = 0;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
// Прототип функции обработки сообщений с пользовательским названием
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RGZ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    { return FALSE; }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RGZ));
    MSG msg;

    // Цикл основного сообщения, обновляем окно
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RGZ));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RGZ);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 550, 300, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    { return FALSE; }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    //создание таймера
    SetTimer(hWnd, TIMER_ID1, 100, (TIMERPROC)NULL);     
    return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        //если что-то для начала создать то:
    case WM_CREATE:

        CreateWindow(L"BUTTON", L"Открыть 1 кассу", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
            370, 100, 150, 30, hWnd, (HMENU)BUTTON_ID1, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        CreateWindow(L"BUTTON", L"Открыть 2 кассу", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
            370, 150, 150, 30, hWnd, (HMENU)BUTTON_ID2, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        CreateWindow(L"BUTTON", L"Открыть 3 кассу", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            370, 200, 150, 30, hWnd, (HMENU)BUTTON_ID3, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        CreateWindow(L"BUTTON", L"Добавить человека", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            370, 10, 150, 40, hWnd, (HMENU)BUTTON_ID4, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        PeopleTxt = CreateWindow(L"STATIC", L"Количество людей в очереди: ", WS_VISIBLE | WS_CHILD,
            10, 10, 250, 30, hWnd, NULL, NULL, NULL);

        PeopleObsl = CreateWindow(L"STATIC", L"Обслуживаются в данный момент: ", WS_VISIBLE | WS_CHILD,
            10, 50, 250, 30, hWnd, NULL, NULL, NULL);

        StKassa1 = CreateWindow(L"STATIC", L"Состояние кассы 1: ", WS_VISIBLE | WS_CHILD, 
            10, 100, 350, 30, hWnd, NULL, NULL, NULL );

        StKassa2 = CreateWindow(L"STATIC", L"Состояние кассы 2: ", WS_VISIBLE | WS_CHILD, 
            10, 150, 350, 30, hWnd, NULL, NULL, NULL);

        StKassa3 = CreateWindow(L"STATIC", L"Состояние кассы 3: ", WS_VISIBLE | WS_CHILD,
            10, 200, 350, 30, hWnd, NULL, NULL, NULL);

    case WM_COMMAND:
    {
        //кнопка открыть/закрыть 1
        if (LOWORD(wParam) == BUTTON_ID1) {
            if (KS.status1() == false) {
                KS.start1();
                SetWindowText((HWND)lParam, L"Закрыть 1 кассу");
            }
            else {
                SetWindowText((HWND)lParam, L"Открыть 1 кассу");
                KS.stop1();
            }
        }
        //кнопка открыть/закрыть 2
        if (LOWORD(wParam) == BUTTON_ID2) {
            if (KS2.status2() == false) {
                KS2.start2();
                SetWindowText((HWND)lParam, L"Закрыть 2 кассу");
            }
            else {
                SetWindowText((HWND)lParam, L"Открыть 2 кассу");
                KS2.stop2();
            }
        }
        //кнопка открыть/закрыть 3
        if (LOWORD(wParam) == BUTTON_ID3) {
            if (KS3.status3() == false) {
                KS3.start3();
                SetWindowText((HWND)lParam, L"Закрыть 3 кассу");
            }
            else {
                SetWindowText((HWND)lParam, L"Открыть 3 кассу");
                KS3.stop3();
            }
        }

        //кнопка добавления людей
        if (LOWORD(wParam) == BUTTON_ID4)
        {
            people0++;
        }
    }
    break;
    case WM_TIMER:
        switch (wParam) {
        case TIMER_ID1:
            //обновление данных
            wchar_t buff[256];
            swprintf(buff, 256, L"Обслуживаются в данный момент:  %d", n1 + n2 + n3);
            SetWindowText(PeopleObsl, buff);
            swprintf(buff, 256, L"Количество людей в очереди:  %d", people0);
            SetWindowText(PeopleTxt, buff);
            // 1 касса
            if (KS.status1() == false)
            {
                swprintf(buff, 256, L"Состояние кассы: закрыто");
                n1 = 0;
            }
            else if (KS.open1()) {
                swprintf(buff, 256, L"Состояние кассы: свободно");
                n1 = 0;
            }
            else {
                swprintf(buff, 256, L"Состояние кассы: занято, осталось %d сек", KS.get_time1());
                n1 = 1;
            }
            SetWindowText(StKassa1, buff);
            // 2 касса
            if (KS2.status2() == false)
            {
                swprintf(buff, 256, L"Состояние кассы: закрыто");
                n2 = 0;
            }
            else if (KS2.open2()) {
                swprintf(buff, 256, L"Состояние кассы: свободно");
                n2 = 0;
            }
            else {
                swprintf(buff, 256, L"Состояние кассы: занято, осталось %d сек", KS2.get_time2());
                n2 = 1;
            }
            SetWindowText(StKassa2, buff);
            // 3 касса
            if (KS3.status3() == false)
            {
                swprintf(buff, 256, L"Состояние кассы: закрыто");
                n3 = 0;
            }
            else if (KS3.open3()) {
                swprintf(buff, 256, L"Состояние кассы: свободно");
                n3 = 0;
            }
            else {
                swprintf(buff, 256, L"Состояние кассы: занято, осталось %d сек", KS3.get_time3());
                n3 = 1;
            }
            SetWindowText(StKassa3, buff);
            return 0;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
