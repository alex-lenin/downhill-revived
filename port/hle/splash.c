/* The start window: from a double-click until the game's window shows, the first dhd.exe checks
   for an update (up to update.c's CHECK_SECONDS), checks the disc image, and starts the second
   dhd.exe (boot.c), which reads the game's data and prepares the graphics. A small window with
   the icon's picture, what is being done and a bar shows meanwhile.
   The first process owns the window, on a thread of its own (its main thread waits on the
   network and on the child); the relaunched one finds it by its class and sends its steps
   (WM_COPYDATA) and the end (WM_CLOSE). A process that wasn't relaunched closes its own. */
#include "hle.h"
#include "splash.h"
#include "../include/version.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

#pragma comment(lib, "gdi32")

#define CLASS "DhdStartWindow"
enum { W = 520, H = 196, ICON = 132, TIMER = 1, STEP_DATA = 0x44485354 /* "DHST" */ };

typedef struct { int percent; char text[120]; } Step;

static HWND window;
static HANDLE ready;
static CRITICAL_SECTION lock;
static Step step = {2, "Starting..."};
static ULONGLONG step_since;  /* when step began (GetTickCount64) */
static double shown;  /* the bar as drawn: it creeps towards the next step, never back */
static HICON icon;

static void paint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(hwnd, &ps);
    HDC mem = CreateCompatibleDC(dc);
    HBITMAP bmp = CreateCompatibleBitmap(dc, W, H), old = SelectObject(mem, bmp);
    /* the icon's dusk: deep purple, the sun's orange for the bar */
    for (int y = 0; y < H; y++) {
        int t = y * 255 / H;
        RECT r = {0, y, W, y + 1};
        HBRUSH b = CreateSolidBrush(RGB(38 - t * 16 / 255, 18 - t * 6 / 255, 64 - t * 26 / 255));
        FillRect(mem, &r, b);
        DeleteObject(b);
    }
    if (icon) DrawIconEx(mem, 24, (H - ICON) / 2 - 6, icon, ICON, ICON, 0, NULL, DI_NORMAL);
    SetBkMode(mem, TRANSPARENT);
    int x = 24 + ICON + 24;
    HFONT title = CreateFontA(-34, 0, 0, 0, FW_HEAVY, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
    HFONT body = CreateFontA(-15, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
    HFONT prev = SelectObject(mem, title);
    SetTextColor(mem, RGB(255, 255, 255));
    TextOutA(mem, x, 34, "DOWNHILL", 8);
    SetTextColor(mem, RGB(255, 150, 40));
    TextOutA(mem, x, 70, "REVIVED", 7);
    SelectObject(mem, body);
    SetTextColor(mem, RGB(170, 150, 200));
    TextOutA(mem, x + 150, 84, "v" DHD_VERSION, (int)strlen("v" DHD_VERSION));
    Step now;
    EnterCriticalSection(&lock);
    now = step;
    LeaveCriticalSection(&lock);
    SetTextColor(mem, RGB(225, 215, 240));
    RECT text = {x, 122, W - 24, 142};
    DrawTextA(mem, now.text, -1, &text, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
    RECT track = {x, 150, W - 24, 156};
    HBRUSH back = CreateSolidBrush(RGB(70, 45, 100)), fill = CreateSolidBrush(RGB(255, 150, 40));
    FillRect(mem, &track, back);
    track.right = track.left + (int)((track.right - track.left) * shown / 100);
    FillRect(mem, &track, fill);
    DeleteObject(back), DeleteObject(fill);
    if (GetTickCount64() - step_since > 4000) {  /* a long step: mostly the first start's shaders */
        SetTextColor(mem, RGB(150, 130, 180));
        RECT hint = {x, 164, W - 24, 182};
        DrawTextA(mem, "The first start takes a little longer.", -1, &hint, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);
    }
    SelectObject(mem, prev);
    DeleteObject(title), DeleteObject(body);
    BitBlt(dc, 0, 0, W, H, mem, 0, 0, SRCCOPY);
    SelectObject(mem, old);
    DeleteObject(bmp);
    DeleteDC(mem);
    EndPaint(hwnd, &ps);
}

static LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_PAINT:
        paint(hwnd);
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_TIMER: {
        /* towards the step's mark, slower as it gets near: never still, never past the next */
        EnterCriticalSection(&lock);
        int target = step.percent;
        LeaveCriticalSection(&lock);
        double goal = target + 12 > 99 ? 99 : target + 12;
        if (shown < target) shown += (target - shown) * 0.25 + 0.5;
        else shown += (goal - shown) * 0.01;
        if (shown > 100) shown = 100;
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    case WM_COPYDATA: {
        const COPYDATASTRUCT *cd = (const COPYDATASTRUCT *)lp;
        if (cd->dwData == STEP_DATA && cd->cbData == sizeof(Step)) {
            EnterCriticalSection(&lock);
            step = *(const Step *)cd->lpData;
            step_since = GetTickCount64();
            step.text[sizeof step.text - 1] = 0;
            LeaveCriticalSection(&lock);
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return TRUE;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        window = NULL;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wp, lp);
}

static DWORD WINAPI run(void *arg)
{
    (void)arg;
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = proc;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_APPSTARTING);
    wc.hIcon = LoadIconA(wc.hInstance, MAKEINTRESOURCEA(1));
    wc.lpszClassName = CLASS;
    RegisterClassA(&wc);
    icon = (HICON)LoadImageA(wc.hInstance, MAKEINTRESOURCEA(1), IMAGE_ICON, 256, 256, 0);
    RECT work;
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);
    window = CreateWindowExA(WS_EX_APPWINDOW, CLASS, "Downhill Revived", WS_POPUP,
                             work.left + (work.right - work.left - W) / 2, work.top + (work.bottom - work.top - H) / 2,
                             W, H, NULL, NULL, wc.hInstance, NULL);
    SetEvent(ready);
    if (!window) return 0;
    HRGN round = CreateRoundRectRgn(0, 0, W + 1, H + 1, 14, 14);
    SetWindowRgn(window, round, FALSE);
    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);
    SetTimer(window, TIMER, 33, NULL);
    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 0;
}

static int wanted(void) { return !getenv("DHD_HEADLESS") && !getenv("DHD_NO_SPLASH"); }

void splash_show(void)
{
    if (window || getenv("DHD_RELAUNCHED") || !wanted()) return;
    InitializeCriticalSection(&lock);
    step_since = GetTickCount64();
    ready = CreateEventA(NULL, TRUE, FALSE, NULL);
    HANDLE t = CreateThread(NULL, 0, run, NULL, 0, NULL);
    if (!t) return;
    CloseHandle(t);
    WaitForSingleObject(ready, 2000);
}

void splash_step(int percent, const char *text)
{
    Step s = {percent, ""};
    snprintf(s.text, sizeof s.text, "%s", text);
    FILETIME created, unused, now;  /* the log: when each step began, from this process's start */
    GetProcessTimes(GetCurrentProcess(), &created, &unused, &unused, &unused);
    GetSystemTimeAsFileTime(&now);
    ULARGE_INTEGER a = {{created.dwLowDateTime, created.dwHighDateTime}}, b = {{now.dwLowDateTime, now.dwHighDateTime}};
    fprintf(stderr, "start: %.2f s: %s\n", (double)(b.QuadPart - a.QuadPart) / 1e7, text);
    HWND own = window;
    if (own) {
        EnterCriticalSection(&lock);
        step = s;
        step_since = GetTickCount64();
        LeaveCriticalSection(&lock);
        InvalidateRect(own, NULL, FALSE);
        return;
    }
    HWND first = getenv("DHD_RELAUNCHED") ? FindWindowA(CLASS, NULL) : NULL;
    if (!first) return;
    COPYDATASTRUCT cd = {STEP_DATA, sizeof s, &s};
    DWORD_PTR result;
    SendMessageTimeoutA(first, WM_COPYDATA, 0, (LPARAM)&cd, SMTO_ABORTIFHUNG, 500, &result);
}

void splash_close(void)
{
    splash_step(100, "Ready");
    HWND w = window ? window : getenv("DHD_RELAUNCHED") ? FindWindowA(CLASS, NULL) : NULL;
    if (w) PostMessageA(w, WM_CLOSE, 0, 0);
}
