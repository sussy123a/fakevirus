#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawRandomPattern(HDC hdc, HWND hwnd);  // Pass hwnd to DrawRandomPattern

int main() {
    // Register the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"RandomDrawingClass";  // Use wide character string (L"string")
    RegisterClass(&wc);

    // Create a fullscreen window
    HWND hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, L"RandomDrawingClass", L"Random Drawing and Beeping",
                               WS_POPUP, 0, 0, WIDTH, HEIGHT, NULL, NULL, wc.hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); // Make the window fully transparent
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Draw on the screen for 40 seconds
    DWORD startTime = GetTickCount();
    while (GetTickCount() - startTime < 40000) {
        // Get the device context for drawing
        HDC hdc = GetDC(hwnd);

        // Draw random patterns and generate beeps
        DrawRandomPattern(hdc, hwnd);
        Beep(1000 + rand() % 1000, 100); // Random beep with different frequencies

        // Release the device context
        ReleaseDC(hwnd, hdc);

        // Sleep for a short time to avoid overwhelming the CPU
        Sleep(100);
    }

    // Close the window after 40 seconds
    DestroyWindow(hwnd);
    return 0;
}

// Window procedure to handle messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Function to draw random patterns on the screen
void DrawRandomPattern(HDC hdc, HWND hwnd) {
    // Clear the screen with black
    RECT rect;
    GetClientRect(hwnd, &rect);  // GetClientRect expects HWND as the first parameter
    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW+1));

    // Draw random shapes with random colors
    for (int i = 0; i < 10; i++) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        int size = rand() % 50 + 20;
        int color = RGB(rand() % 256, rand() % 256, rand() % 256);

        // Set the random color
        HBRUSH brush = CreateSolidBrush(color);
        SelectObject(hdc, brush);

        // Randomly choose between drawing a rectangle or ellipse
        if (rand() % 2) {
            Ellipse(hdc, x, y, x + size, y + size);
        } else {
            Rectangle(hdc, x, y, x + size, y + size);
        }

        DeleteObject(brush);
    }
}
