#include "NFSHSSaveEditorForm.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	InitCommonControls();
    if(!RegisterMainClass(hInstance) || !CreateMainWindow(hInstance)) return 1;

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}