#include <Windows.h>
#include<stdio.h>
#include <tchar.h>
#include <tchar.h>
#define MATR_DIM 6
static int matr[MATR_DIM][MATR_DIM];
HWND aa_hwnd[MATR_DIM][MATR_DIM];
LPCTSTR g_szWindowClassName = _T("WindowClass1");
LPCTSTR g_szWindowClassName2 = _T("WindowClass2");
LPCTSTR g_szWindowClassName3 = _T("WindowClass3");
LRESULT CALLBACK WindowProc1(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
    switch (nMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
    default:
        return DefWindowProc(hwnd, nMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WindowProc2(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
    switch (nMsg)
    {
    case WM_MBUTTONUP:
        MessageBox(NULL, _T("WM_MBUTTONUP Отпущена центральная кнопка САП-211!"), _T("Сообщение"), MB_OK);
    default:
        return DefWindowProc(hwnd, nMsg, wParam, lParam);
    }
    return 0;
}

int main() {
    int i, j, n, x = 1;
    TCHAR buff[300] = _T("");
    n = MATR_DIM - 1;
    for (i = 0; i < MATR_DIM; i++) {
        for (j = i + 0; j < MATR_DIM; j += 1) {

            matr[i][n - j] = matr[j][n - i] = x;
            if (x == 0)
                x = 9;
        }
        matr[i][i] = 9;
    }
    for (i = 0; i < MATR_DIM; i++) {
        for (j = 0; j < MATR_DIM; j++) {
            _stprintf_s(buff, _T("%s%3d"), buff, matr[i][j]);

        }
        _stprintf_s(buff, _T("%s\n"), buff);
    }
    MessageBox(NULL, buff, _T("Отладка"), MB_OK);
    return 0;
}

bool RegisterWndClasses() {
    WNDCLASSEX wce_main;
    ZeroMemory(&wce_main, sizeof(WNDCLASSEX));
    wce_main.cbSize = sizeof(WNDCLASSEX);

    wce_main.style = CS_VREDRAW | CS_HREDRAW;
    wce_main.lpfnWndProc = &WindowProc1;
    wce_main.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //wce_main.hbrBackground = (HBRUSH)(rand() % 30);
    wce_main.lpszClassName = g_szWindowClassName;
    if (RegisterClassEx(&wce_main) == NULL)
        return 0;

    wce_main.style = CS_VREDRAW | CS_HREDRAW;
    wce_main.lpfnWndProc = &WindowProc2;
    wce_main.hbrBackground = (HBRUSH)(COLOR_WINDOWTEXT + 10);
    //wce_main.hbrBackground = (HBRUSH)((rand() % 30));
    wce_main.lpszClassName = g_szWindowClassName3;
    if (RegisterClassEx(&wce_main) == NULL)
        return 0;

    wce_main.style = CS_VREDRAW | CS_HREDRAW;
    wce_main.lpfnWndProc = &WindowProc1;
    wce_main.hbrBackground = (HBRUSH)(COLOR_WINDOWTEXT + 6);
    //wce_main.hbrBackground = (HBRUSH)((rand() % 30));
    wce_main.lpszClassName = g_szWindowClassName2;
    if (RegisterClassEx(&wce_main) == NULL)
        return 0;
}

bool CreateWindows(HINSTANCE hApp) {
    int scrw = GetSystemMetrics(SM_CXSCREEN), scrh = GetSystemMetrics(SM_CYSCREEN), wndw, wndh, i, j, x, y, gop = 0;
    LPCTSTR cl; long w;
    TCHAR mc_buff[300] = _T("");
    wndw = scrw / MATR_DIM - gop;
    wndh = scrh / MATR_DIM - gop;
    main();
    for (i = 0; i < MATR_DIM; i++) {
        for (j = 0; j < MATR_DIM; j++) {
            x = j * (wndw + gop);
            y = i * (wndh + gop);
            _stprintf_s(mc_buff, _T("matr[%d][%d]=%d"), i, j, matr[i][j]);
            switch (matr[i][j])
            {
            case 0:
                cl = g_szWindowClassName;
                w = WS_VSCROLL | WS_HSCROLL | WS_SIZEBOX | WS_SYSMENU | WS_CAPTION;;
                break;
            case 1:
                cl = g_szWindowClassName2;
                w = WS_SYSMENU | WS_CAPTION;
                break;
            default:
                cl = g_szWindowClassName3;
                w = WS_SIZEBOX | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
                break;
            }
            aa_hwnd[i][j] = CreateWindow(cl,
                mc_buff,
                w,
                x, y,
                wndw, wndh,
                NULL, NULL, hApp, NULL);
            if (aa_hwnd[i][j] == NULL) {
                MessageBox(NULL, _T("EROR!"), _T("EROR!"), MB_OK);
                return false;
            }
            ShowWindow(aa_hwnd[i][j], SW_SHOW);
            UpdateWindow(aa_hwnd[i][j]);
        }
    }
}
int WINAPI _tWinMain(HINSTANCE hApp, HINSTANCE hPrevApp, LPTSTR szCmdLine, int nCmdShow) {
    MSG msg;
    RegisterWndClasses();
    CreateWindows(hApp);
    while (GetMessage(&msg, NULL, 0, 0) != 0)
        DispatchMessage(&msg);
    UnregisterClass(g_szWindowClassName, hApp);
    UnregisterClass(g_szWindowClassName2, hApp);
    UnregisterClass(g_szWindowClassName3, hApp);
    return msg.wParam;

}