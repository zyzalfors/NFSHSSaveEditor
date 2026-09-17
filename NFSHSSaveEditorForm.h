#include "NFSHSSaveEditor.h"
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>

#define WND_CLASS_NAME "NFSHSSaveEditor"
#define WND_TITLE "NFSHS Save Editor"
#define WND_WIDTH 350
#define WND_HEIGHT 390
#define MAX_BUF_SIZE 21

#define TOOLBAR 1000
#define OPEN 1001
#define WRITE 1002
#define FORMAT 1003
#define SAVE 1004
#define GAME_SERIAL 1005
#define PLAYER_LANGUAGE 1006
#define PLAYER_NAME 1007
#define MONEY 1008
#define OWNED_CAR_SLOT 1009
#define OWNED_CAR_MODEL 1010
#define OWNED_CAR_UPGRADE 1011
#define OWNED_CAR_COLOR 1012
#define ALL_CARS 1013
#define ALL_TRACKS 1014
#define GOLD_TROPHIES 1015

char path[MAX_PATH];
NFSHSSaveEditor editor = {0};

void InitGui(HWND hWnd) {
    SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLongPtr(hWnd, GWL_STYLE) & ~((LONG_PTR) WS_MAXIMIZEBOX));

    HWND h = CreateWindowA(TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST, 0, 0, 0, 0, hWnd, (HMENU) TOOLBAR, NULL, NULL);
    SendMessage(h, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    TBBUTTON buttons[] = {{I_IMAGENONE, OPEN, TBSTATE_ENABLED, BTNS_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR) "Open"},
                          {I_IMAGENONE, WRITE, 0, BTNS_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR) "Write"}};
    SendMessage(h, TB_ADDBUTTONS, ARRAYSIZE(buttons), (LPARAM) buttons);
    SendMessage(h, TB_AUTOSIZE, 0, 0);
    SendMessage(h, CCM_SETBKCOLOR, 0, RGB(255, 255, 255));

    CreateWindowA("STATIC", "Format:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 30, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 30, 150, 20, hWnd, (HMENU) FORMAT, NULL, NULL);
    SendMessage(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);

    CreateWindowA("STATIC", "Save:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 60, 50, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, 60, 150, 200, hWnd, (HMENU) SAVE, NULL, NULL);
    EnableWindow(h, FALSE);

    CreateWindowA("STATIC", "Serial:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 90, 50, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 90, 150, 20, hWnd, (HMENU) GAME_SERIAL, NULL, NULL);
    SendMessage(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);

    CreateWindowA("STATIC", "Name:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 120, 50, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 120, 150, 20, hWnd, (HMENU) PLAYER_NAME, NULL, NULL);
    SendMessage(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);

    CreateWindowA("STATIC", "Language:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 150, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, 150, 150, 200, hWnd, (HMENU) PLAYER_LANGUAGE, NULL, NULL);
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_languages); i++)
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) NFSHSSaveEditor_languages[i]);
    EnableWindow(h, FALSE);

    CreateWindowA("STATIC", "Money:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 180, 50, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 180, 150, 20, hWnd, (HMENU) MONEY, NULL, NULL);
    EnableWindow(h, FALSE);

    char buf[MAX_BUF_SIZE];
    CreateWindowA("STATIC", "Car slot:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 210, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, 210, 150, 200, hWnd, (HMENU) OWNED_CAR_SLOT, NULL, NULL);
    for(size_t i = 0; i < OWNED_CAR_SLOT_COUNT; i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);

    CreateWindowA("STATIC", "Model:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 240, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, 240, 150, 200, hWnd, (HMENU) OWNED_CAR_MODEL, NULL, NULL);
    SendMessage(h, CB_ADDSTRING, 0, (LPARAM) "None");
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_models); i++)
       if(NFSHSSaveEditor_models[i]) SendMessage(h, CB_ADDSTRING, 0, (LPARAM) NFSHSSaveEditor_models[i]);
    EnableWindow(h, FALSE);

    CreateWindowA("STATIC", "Upgrade:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 270, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, 270, 150, 200, hWnd, (HMENU) OWNED_CAR_UPGRADE, NULL, NULL);
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_upgrades); i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);

    CreateWindowA("STATIC", "Color:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 300, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, 300, 150, 200, hWnd, (HMENU) OWNED_CAR_COLOR, NULL, NULL);
    for(size_t i = 0; i < OWNED_CAR_COLOR_COUNT; i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);

    h = CreateWindowA("BUTTON", "All cars", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 330, 100, 20, hWnd, (HMENU) ALL_CARS, NULL, NULL);
    EnableWindow(h, FALSE);

    h = CreateWindowA("BUTTON", "All tracks", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 330, 100, 20, hWnd, (HMENU) ALL_TRACKS, NULL, NULL);
    EnableWindow(h, FALSE);

    h = CreateWindowA("BUTTON", "Gold trophies", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 330, 100, 20, hWnd, (HMENU) GOLD_TROPHIES, NULL, NULL);
    EnableWindow(h, FALSE);
}

void ClearGui(HWND hWnd) {
    SendMessage(GetDlgItem(hWnd, TOOLBAR), TB_ENABLEBUTTON, (WPARAM) WRITE, MAKELONG(FALSE, 0));

    HWND h = GetDlgItem(hWnd, FORMAT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, SAVE);
    SendMessage(h, CB_RESETCONTENT, 0, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, GAME_SERIAL);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, PLAYER_NAME);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, PLAYER_LANGUAGE);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, MONEY);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_SLOT);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_MODEL);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_UPGRADE);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_COLOR);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, ALL_CARS);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, ALL_TRACKS);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, GOLD_TROPHIES);
    EnableWindow(h, FALSE);
}

void PrintSaveById(HWND hWnd, NFSHSSaveEditor* editor, const BOOL chsave, const BOOL chownedcar, const size_t saveidx, const size_t carslotidx) {
    if(chsave) {
        HWND h = GetDlgItem(hWnd, GAME_SERIAL);
        SetWindowTextA(h, "");
        if(editor->saves[saveidx].serial) {
            EnableWindow(h, TRUE);
            SetWindowTextA(h, editor->saves[saveidx].serial);
        }

        h = GetDlgItem(hWnd, PLAYER_NAME);
        SetWindowTextA(h, "");
        if(editor->saves[saveidx].name) {
            EnableWindow(h, TRUE);
            SetWindowTextA(h, editor->saves[saveidx].name);
        }

        h = GetDlgItem(hWnd, PLAYER_LANGUAGE);
        EnableWindow(h, TRUE);
        SendMessage(h, CB_SETCURSEL, 0, 0);
        if(editor->saves[saveidx].language) {
            LRESULT lanidx = SendMessage(h, CB_FINDSTRINGEXACT, -1, (LPARAM) editor->saves[saveidx].language);
            SendMessage(h, CB_SETCURSEL, (WPARAM) lanidx, 0);
        }

        h = GetDlgItem(hWnd, MONEY);
        EnableWindow(h, TRUE);
        SetDlgItemInt(hWnd, MONEY, editor->saves[saveidx].money, TRUE);
    }

    if(chownedcar) {
        HWND h = GetDlgItem(hWnd, OWNED_CAR_MODEL);
        EnableWindow(h, TRUE);
        SendMessage(h, CB_SETCURSEL, 0, 0);
        if(editor->saves[saveidx].ownedcars[carslotidx].model) {
            LRESULT modidx = SendMessage(h, CB_FINDSTRINGEXACT, -1, (LPARAM) editor->saves[saveidx].ownedcars[carslotidx].model);
            SendMessage(h, CB_SETCURSEL, (WPARAM) modidx, 0);
        }

        h = GetDlgItem(hWnd, OWNED_CAR_UPGRADE);
        EnableWindow(h, TRUE);
        SendMessage(h, CB_SETCURSEL, (WPARAM) editor->saves[saveidx].ownedcars[carslotidx].upgrade, 0);

        h = GetDlgItem(hWnd, OWNED_CAR_COLOR);
        EnableWindow(h, TRUE);
        SendMessage(h, CB_SETCURSEL, (WPARAM) editor->saves[saveidx].ownedcars[carslotidx].color, 0);
    }
}

void PrintSave(HWND hWnd, NFSHSSaveEditor* editor, const char* path) {
    NFSHSSaveEditor_init(editor, path);

    if(editor->savecount == 0) {
        NFSHSSaveEditor_clear(editor);
        MessageBoxA(hWnd, "No NFSHS save found.", "Error", MB_OK | MB_ICONERROR);
        ClearGui(hWnd);
        return;
    }

    SendMessage(GetDlgItem(hWnd, TOOLBAR), TB_ENABLEBUTTON, (WPARAM) WRITE, MAKELONG(TRUE, 0));

    HWND h = GetDlgItem(hWnd, FORMAT);
    SetWindowTextA(h, "");
    if(editor->format) {
        EnableWindow(h, TRUE);
        SetWindowTextA(h, editor->format);
    }

    char savebuf[MAX_BUF_SIZE];
    h = GetDlgItem(hWnd, SAVE);
    EnableWindow(h, TRUE);
    SendMessage(h, CB_RESETCONTENT, 0, 0);
    for(size_t i = 0; i < editor->savecount; i++) {
        snprintf(savebuf, sizeof(savebuf), "%d", (int) i);
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) savebuf);
    }
    SendMessage(h, CB_SETCURSEL, 0, 0);

    h = GetDlgItem(hWnd, OWNED_CAR_SLOT);
    EnableWindow(h, TRUE);
    SendMessage(h, CB_SETCURSEL, 0, 0);

    h = GetDlgItem(hWnd, ALL_CARS);
    EnableWindow(h, TRUE);

    h = GetDlgItem(hWnd, ALL_TRACKS);
    EnableWindow(h, TRUE);

    h = GetDlgItem(hWnd, GOLD_TROPHIES);
    EnableWindow(h, TRUE);

    PrintSaveById(hWnd, editor, TRUE, TRUE, 0, 0);
}

void OpenSave(HWND hWnd, NFSHSSaveEditor* editor) {
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.nMaxFile = sizeof(path);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetOpenFileNameA(&ofn)) PrintSave(hWnd, editor, path);
}

void WriteSave(HWND hWnd, NFSHSSaveEditor* editor) {
    NFSHSSaveEditor_update(editor);
    MessageBoxA(hWnd, "Save(s) written.", "Info", MB_OK | MB_ICONINFORMATION);
}

void UpdateSave(HWND hWnd, WPARAM wParam, NFSHSSaveEditor* editor) {
    switch(LOWORD(wParam)) {
        case PLAYER_LANGUAGE: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE, CB_GETCURSEL, 0, 0);
            LRESULT lanidx = SendDlgItemMessageA(hWnd, PLAYER_LANGUAGE, CB_GETCURSEL, 0, 0);

            char lan[MAX_BUF_SIZE];
            SendDlgItemMessageA(hWnd, PLAYER_LANGUAGE, CB_GETLBTEXT, (WPARAM) lanidx, (LPARAM) lan);

            NFSHSSaveEditor_updatelang(editor, saveidx, lan);
        }
        break;

        case MONEY: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE, CB_GETCURSEL, 0, 0);

            char moneybuf[MAX_BUF_SIZE];
            GetDlgItemTextA(hWnd, MONEY, moneybuf, sizeof(moneybuf));
            char* end = NULL;
            int32_t money = strtol(moneybuf, &end, 10);

            if(end != moneybuf && *end == '\0')
                NFSHSSaveEditor_updatemoney(editor, saveidx, money);
        }
        break;

        case OWNED_CAR_MODEL:
        case OWNED_CAR_UPGRADE:
        case OWNED_CAR_COLOR: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE, CB_GETCURSEL, 0, 0);
            LRESULT carslotidx = SendDlgItemMessageA(hWnd, OWNED_CAR_SLOT, CB_GETCURSEL, 0, 0);
            LRESULT modidx = SendDlgItemMessageA(hWnd, OWNED_CAR_MODEL, CB_GETCURSEL, 0, 0);
            LRESULT upgrade = SendDlgItemMessageA(hWnd, OWNED_CAR_UPGRADE, CB_GETCURSEL, 0, 0);
            LRESULT color = SendDlgItemMessageA(hWnd, OWNED_CAR_COLOR, CB_GETCURSEL, 0, 0);

            char model[MAX_BUF_SIZE];
            SendDlgItemMessageA(hWnd, OWNED_CAR_MODEL, CB_GETLBTEXT, (WPARAM) modidx, (LPARAM) model);

            NFSHSSaveEditor_updateownedcar(editor, saveidx, carslotidx, (const char*) model, upgrade, color);
        }
        break;

        case ALL_CARS: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_unlockcars(editor, saveidx);

        }
        break;

        case ALL_TRACKS: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_unlocktracks(editor, saveidx);

        }
        break;

        case GOLD_TROPHIES: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_setgoldtrophies(editor, saveidx);

        }
        break;
    }
}

void ProcessCmd(HWND hWnd, WPARAM wParam, NFSHSSaveEditor* editor) {
    switch(LOWORD(wParam)) {
        case OPEN:
            OpenSave(hWnd, editor);
            break;

        case WRITE:
            WriteSave(hWnd, editor);
            break;

        case SAVE:
            if(HIWORD(wParam) == CBN_SELCHANGE) {
                LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE, CB_GETCURSEL, 0, 0);
                LRESULT carslotidx = SendDlgItemMessageA(hWnd, OWNED_CAR_SLOT, CB_GETCURSEL, 0, 0);
                PrintSaveById(hWnd, editor, TRUE, TRUE, saveidx, carslotidx);
            }
            break;

        case PLAYER_LANGUAGE:
        case OWNED_CAR_MODEL:
        case OWNED_CAR_UPGRADE:
        case OWNED_CAR_COLOR:
            if(HIWORD(wParam) == CBN_SELCHANGE) UpdateSave(hWnd, wParam, editor);
            break;

        case MONEY:
            if(HIWORD(wParam) == EN_CHANGE) UpdateSave(hWnd, wParam, editor);
            break;

        case OWNED_CAR_SLOT:
            if(HIWORD(wParam) == CBN_SELCHANGE) {
                LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE, CB_GETCURSEL, 0, 0);
                LRESULT carslotidx = SendDlgItemMessageA(hWnd, OWNED_CAR_SLOT, CB_GETCURSEL, 0, 0);
                PrintSaveById(hWnd, editor, FALSE, TRUE, saveidx, carslotidx);
            }
            break;

        case ALL_CARS:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "All cars unlocked.", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case ALL_TRACKS:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "All tracks unlocked.", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case GOLD_TROPHIES:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "Gold trophies unlocked.", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT res = 0;

    switch(msg) {
        case WM_CREATE:
            InitGui(hWnd);
            break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            NFSHSSaveEditor_clear(&editor);
            PostQuitMessage(0);
            break;

        case WM_COMMAND:
            ProcessCmd(hWnd, wParam, &editor);
            break;

        default:
            res = DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return res;
}

BOOL RegisterMainClass(HINSTANCE hInstance) {
    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WND_CLASS_NAME;
    wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    return RegisterClassExA(&wc) != 0;
}

HWND CreateMainWindow(HINSTANCE hInstance) {
    return CreateWindowA(WND_CLASS_NAME, WND_TITLE, WS_VISIBLE | (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL);
}