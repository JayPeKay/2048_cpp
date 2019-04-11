#include <windows.h>
#include <time.h>

#include "Window.h"

int main(int argc, char **argv) {
	// pobieranie rozdzielczoœci
	srand(time(NULL));
	const HWND hwndDesktop = GetDesktopWindow();
	RECT desktop;
	GetWindowRect(hwndDesktop, &desktop);

	// marginesy w pixelach
	const int marginRight = 100;
	const int marginLeft = 100;
	const int marginTop = 100;
	const int marginBot = 100;

	const int width = desktop.right - marginRight - marginLeft;
	const int height = desktop.bottom - marginTop - marginBot;

	Window window(width, height);
	return 0;
}