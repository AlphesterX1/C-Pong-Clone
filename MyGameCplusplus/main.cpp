#include "utils.cpp"
#include <windows.h>
#include "Color.cpp"


using namespace std;

global_variable bool running = true;

struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

#include "renderer.cpp"
#include "platdormCommon.cpp"
#include "game.cpp"

LRESULT CALLBACK windowCallBack(HWND hwnd, UINT uMsg, WPARAM wPARAM, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(u32);

			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

		}break;
		default: {
			result = DefWindowProc(hwnd, uMsg, wPARAM, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd) {

	//Window Class
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = "Game Window Class";
	windowClass.lpfnWndProc = windowCallBack;

	//Register Class
	RegisterClass(&windowClass);

	//Create Window
	HWND window = CreateWindow(windowClass.lpszClassName, "First Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	Input input = {};

	float DeltaTime = 0.016666f;
	LARGE_INTEGER frameBegin_time;
	QueryPerformanceCounter(&frameBegin_time);

	float performanceFrequency; {
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFrequency = (float)perf.QuadPart;
	}

	while (running) {
		//Input
		MSG message;

		for (int i = 0; i < KeyCount; i++) {
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			
			switch (message.message) {
				case WM_KEYUP:
				case WM_KEYDOWN: {
					u32 vk_code = (u32)message.wParam;
					bool isDown = ((message.lParam & (1 << 31)) == 0);

#define processKey(b, vk)\
case vk: {\
input.buttons[b].changed = isDown != input.buttons[b].isDown;\
input.buttons[b].isDown = isDown;\
}break;

					switch (vk_code) {
						processKey(Down, VK_UP);
						processKey(Up, VK_DOWN);
						processKey(Space, VK_SPACE);
						processKey(W, 'W');
						processKey(S, 'S');
						processKey(Enter,VK_RETURN)
					}
				} break;
				default: {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
			
		}

		//Simulate
		simulateGame(&input , DeltaTime);
			
		//Render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);


		LARGE_INTEGER frameEnd_time;
		QueryPerformanceCounter(&frameEnd_time);
		DeltaTime = (float)(frameEnd_time.QuadPart - frameBegin_time.QuadPart) / performanceFrequency;
		frameBegin_time = frameEnd_time;
	}
}