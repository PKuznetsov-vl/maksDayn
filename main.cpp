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
    switch (nMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
        default:
            return DefWindowProc(hwnd, nMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WindowProc2(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
    /*  switch (nMsg)
      {
      case WM_MBUTTONUP:
          MessageBox(NULL, _T("WM_MBUTTONUP Отпущена центральная кнопка САП-211!"), _T("Сообщение"), MB_OK);
      default:*/
    return DefWindowProc(hwnd, nMsg, wParam, lParam);

}

void CreatePreView() {
    int i, j;
    TCHAR buff[300] = _T("");
    for (i = 0; i < MATR_DIM; i++) {
        for (j = 0; j < MATR_DIM; j++) {
            matr[i][j] = 3;

        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < MATR_DIM - i - 1; j++) {
            matr[i][j] = 1;
        }
    }

    for (i = 6; i >= 3; i--) {
        for (j = 0; j < i; j++) {
            matr[i][j] = 2;
        }
    }
    for (i = 1; i < 3; i++) {
        for (j = 0; j < MATR_DIM - (MATR_DIM - i); j++) {
            matr[i][j] = 0;
        }
    }
    for (i = 3; i < 5; i++) {
        for (j = 0; j < MATR_DIM - i - 1; j++) {
            matr[i][j] = 0;
        }
    }

    for (i = 0; i < MATR_DIM; i++) {
        for (j = 0; j < MATR_DIM; j++) {
            _stprintf_s(buff, _T("%s%3d"), buff, matr[i][j]);

        }
        _stprintf_s(buff, _T("%s\n"), buff);
    }


    MessageBox(NULL, buff, _T("Windows structure"), MB_OK);

}

void RegisterWndClasses() {
    WNDCLASSEX wce_main;
    ZeroMemory(&wce_main, sizeof(WNDCLASSEX));
    wce_main.cbSize = sizeof(WNDCLASSEX);

    wce_main.style = CS_VREDRAW | CS_HREDRAW;
    wce_main.lpfnWndProc = &WindowProc1;
    wce_main.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    //wce_main.hbrBackground = (HBRUSH)(rand() % 30);
    wce_main.lpszClassName = g_szWindowClassName;
    if (RegisterClassEx(&wce_main) == NULL)
        throw eeptNone;

    wce_main.style = CS_VREDRAW | CS_HREDRAW;
    wce_main.lpfnWndProc = &WindowProc2;
    wce_main.hbrBackground = (HBRUSH) (COLOR_WINDOWTEXT + 10);
    //wce_main.hbrBackground = (HBRUSH)((rand() % 30));
    wce_main.lpszClassName = g_szWindowClassName3;
    if (RegisterClassEx(&wce_main) == NULL)
        throw eeptNone;

    wce_main.style = CS_VREDRAW | CS_HREDRAW;
    wce_main.lpfnWndProc = &WindowProc1;
    wce_main.hbrBackground = (HBRUSH) (COLOR_WINDOWTEXT + 6);
    //wce_main.hbrBackground = (HBRUSH)((rand() % 30));
    wce_main.lpszClassName = g_szWindowClassName2;
    if (RegisterClassEx(&wce_main) == NULL)
//todo rewrite ex
        throw eeptNone;
}

void CreateWindows(HINSTANCE hApp) {
    CreatePreView();
    int scrw = GetSystemMetrics(SM_CXSCREEN), scrh = GetSystemMetrics(SM_CYSCREEN), wndw, wndh, i, j, x, y, gop = 0;
    LPCTSTR cl;
    long w;
    TCHAR mc_buff[300] = _T("");
    wndw = scrw / MATR_DIM - gop;
    wndh = scrh / MATR_DIM - gop;

    for (i = 0; i < MATR_DIM; i++) {
        for (j = 0; j < MATR_DIM; j++) {
            x = j * (wndw + gop);
            y = i * (wndh + gop);
            _stprintf_s(mc_buff, _T("matr[%d][%d]=%d"), i, j, matr[i][j]);
            switch (matr[i][j]) {
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
                                         nullptr, nullptr, hApp, nullptr);
            if (aa_hwnd[i][j] == nullptr) {
                MessageBox(nullptr, _T("EROR!"), _T("EROR!"), MB_OK);
                //throw exception_code ;
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
    while (GetMessage(&msg, nullptr, 0, 0) != 0)
        DispatchMessage(&msg);
    UnregisterClass(g_szWindowClassName, hApp);
    UnregisterClass(g_szWindowClassName2, hApp);
    UnregisterClass(g_szWindowClassName3, hApp);
    return msg.wParam;

}

int main() {
    CreatePreView();
    return 0;
}