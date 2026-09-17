#include "NFSHSSaveEditor.h"
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>

#define WND_CLASS_NAME "NFSHSSaveEditor"
#define WND_TITLE "NFSHS Save Editor"
#define WND_WIDTH 350
#define WND_HEIGHT 420
#define WND_Y_POS 30
#define MAX_BUF_SIZE 21

#define TOOLBAR 1000
#define OPEN_BTN 1001
#define WRITE_BTN 1002
#define PATH_TXT 1003
#define FORMAT_TXT 1004
#define SAVE_COMBO 1005
#define SERIAL_TXT 1006
#define LANGUAGE_COMBO 1007
#define PLAYER_NAME_TXT 1008
#define MONEY_TXT 1009
#define OWNED_CAR_SLOT_COMBO 1010
#define OWNED_CAR_MODEL_COMBO 1011
#define OWNED_CAR_UPGRADE_COMBO 1012
#define OWNED_CAR_COLOR_COMBO 1013
#define ALL_CARS_BTN 1014
#define ALL_TRACKS_BTN 1015
#define GOLD_TROPHIES_BTN 1016

char path[MAX_PATH] = {0};
NFSHSSaveEditor editor = {0};

void InitGui(HWND hWnd) {
    SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLongPtr(hWnd, GWL_STYLE) & ~((LONG_PTR) WS_MAXIMIZEBOX));

    HWND h = CreateWindowA(TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST, 0, 0, 0, 0, hWnd, (HMENU) TOOLBAR, NULL, NULL);
    SendMessage(h, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    TBBUTTON buttons[] = {{I_IMAGENONE, OPEN_BTN, TBSTATE_ENABLED, BTNS_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR) "Open"},
                          {I_IMAGENONE, WRITE_BTN, 0, BTNS_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR) "Write"}};
    SendMessage(h, TB_ADDBUTTONS, ARRAYSIZE(buttons), (LPARAM) buttons);
    SendMessage(h, TB_AUTOSIZE, 0, 0);
    SendMessage(h, CCM_SETBKCOLOR, 0, RGB(255, 255, 255));

    size_t y = WND_Y_POS;

    CreateWindowA("STATIC", "Path:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 150, 20, hWnd, (HMENU) PATH_TXT, NULL, NULL);
    SendMessage(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Format:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 150, 20, hWnd, (HMENU) FORMAT_TXT, NULL, NULL);
    SendMessage(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Save:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 150, 200, hWnd, (HMENU) SAVE_COMBO, NULL, NULL);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Serial:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 150, 20, hWnd, (HMENU) SERIAL_TXT, NULL, NULL);
    SendMessage(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Name:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 150, 20, hWnd, (HMENU) PLAYER_NAME_TXT, NULL, NULL);
    SendMessage(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Language:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 150, 200, hWnd, (HMENU) LANGUAGE_COMBO, NULL, NULL);
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_languages); i++)
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) NFSHSSaveEditor_languages[i]);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Money:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 150, 20, hWnd, (HMENU) MONEY_TXT, NULL, NULL);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    char buf[MAX_BUF_SIZE];
    CreateWindowA("STATIC", "Car slot:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 150, 200, hWnd, (HMENU) OWNED_CAR_SLOT_COMBO, NULL, NULL);
    for(size_t i = 0; i < OWNED_CAR_SLOT_COUNT; i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Model:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 150, 200, hWnd, (HMENU) OWNED_CAR_MODEL_COMBO, NULL, NULL);
    SendMessage(h, CB_ADDSTRING, 0, (LPARAM) "None");
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_models); i++)
       if(NFSHSSaveEditor_models[i]) SendMessage(h, CB_ADDSTRING, 0, (LPARAM) NFSHSSaveEditor_models[i]);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Upgrade:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 150, 200, hWnd, (HMENU) OWNED_CAR_UPGRADE_COMBO, NULL, NULL);
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_upgrades); i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowA("STATIC", "Color:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, (HMENU) 0, NULL, NULL);
    h = CreateWindowA(WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 150, 200, hWnd, (HMENU) OWNED_CAR_COLOR_COMBO, NULL, NULL);
    for(size_t i = 0; i < OWNED_CAR_COLOR_COUNT; i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    h = CreateWindowA("BUTTON", "All cars", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, y, 100, 20, hWnd, (HMENU) ALL_CARS_BTN, NULL, NULL);
    EnableWindow(h, FALSE);

    h = CreateWindowA("BUTTON", "All tracks", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, y, 100, 20, hWnd, (HMENU) ALL_TRACKS_BTN, NULL, NULL);
    EnableWindow(h, FALSE);

    h = CreateWindowA("BUTTON", "Gold trophies", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, y, 100, 20, hWnd, (HMENU) GOLD_TROPHIES_BTN, NULL, NULL);
    EnableWindow(h, FALSE);
}

void ClearGui(HWND hWnd) {
    SendMessage(GetDlgItem(hWnd, TOOLBAR), TB_ENABLEBUTTON, (WPARAM) WRITE_BTN, MAKELONG(FALSE, 0));

    HWND h = GetDlgItem(hWnd, PATH_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, FORMAT_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, SAVE_COMBO);
    SendMessage(h, CB_RESETCONTENT, 0, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, SERIAL_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, PLAYER_NAME_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, LANGUAGE_COMBO);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, MONEY_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_SLOT_COMBO);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_MODEL_COMBO);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_UPGRADE_COMBO);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_COLOR_COMBO);
    SendMessage(h, CB_SETCURSEL, (WPARAM) -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, ALL_CARS_BTN);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, ALL_TRACKS_BTN);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, GOLD_TROPHIES_BTN);
    EnableWindow(h, FALSE);
}

void PrintSaveById(HWND hWnd, NFSHSSaveEditor* editor, const BOOL chsave, const BOOL chownedcar, const size_t saveidx, const size_t carslotidx) {
    if(chsave) {
        HWND h = GetDlgItem(hWnd, SERIAL_TXT);
        SetWindowTextA(h, "");
        if(editor->saves[saveidx].serial) {
            EnableWindow(h, TRUE);
            SetWindowTextA(h, editor->saves[saveidx].serial);
        }

        h = GetDlgItem(hWnd, PLAYER_NAME_TXT);
        SetWindowTextA(h, "");
        if(editor->saves[saveidx].name) {
            EnableWindow(h, TRUE);
            SetWindowTextA(h, editor->saves[saveidx].name);
        }

        h = GetDlgItem(hWnd, LANGUAGE_COMBO);
        EnableWindow(h, TRUE);
        SendMessage(h, CB_SETCURSEL, 0, 0);
        if(editor->saves[saveidx].language) {
            LRESULT lanidx = SendMessage(h, CB_FINDSTRINGEXACT, -1, (LPARAM) editor->saves[saveidx].language);
            SendMessage(h, CB_SETCURSEL, (WPARAM) lanidx, 0);
        }

        h = GetDlgItem(hWnd, MONEY_TXT);
        EnableWindow(h, TRUE);
        SetDlgItemInt(hWnd, MONEY_TXT, editor->saves[saveidx].money, TRUE);
    }

    if(chownedcar) {
        HWND h = GetDlgItem(hWnd, OWNED_CAR_MODEL_COMBO);
        EnableWindow(h, TRUE);
        SendMessage(h, CB_SETCURSEL, 0, 0);
        if(editor->saves[saveidx].ownedcars[carslotidx].model) {
            LRESULT modidx = SendMessage(h, CB_FINDSTRINGEXACT, -1, (LPARAM) editor->saves[saveidx].ownedcars[carslotidx].model);
            SendMessage(h, CB_SETCURSEL, (WPARAM) modidx, 0);
        }

        h = GetDlgItem(hWnd, OWNED_CAR_UPGRADE_COMBO);
        EnableWindow(h, TRUE);
        SendMessage(h, CB_SETCURSEL, (WPARAM) editor->saves[saveidx].ownedcars[carslotidx].upgrade, 0);

        h = GetDlgItem(hWnd, OWNED_CAR_COLOR_COMBO);
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

    SendMessage(GetDlgItem(hWnd, TOOLBAR), TB_ENABLEBUTTON, (WPARAM) WRITE_BTN, MAKELONG(TRUE, 0));

    HWND h = GetDlgItem(hWnd, PATH_TXT);
    SetWindowTextA(h, "");
    if(editor->path) {
        EnableWindow(h, TRUE);
        SetWindowTextA(h, editor->path);
    }

    h = GetDlgItem(hWnd, FORMAT_TXT);
    SetWindowTextA(h, "");
    if(editor->format) {
        EnableWindow(h, TRUE);
        SetWindowTextA(h, editor->format);
    }

    char savebuf[MAX_BUF_SIZE];
    h = GetDlgItem(hWnd, SAVE_COMBO);
    EnableWindow(h, TRUE);
    SendMessage(h, CB_RESETCONTENT, 0, 0);
    for(size_t i = 0; i < editor->savecount; i++) {
        snprintf(savebuf, sizeof(savebuf), "%d", (int) i);
        SendMessage(h, CB_ADDSTRING, 0, (LPARAM) savebuf);
    }
    SendMessage(h, CB_SETCURSEL, 0, 0);

    h = GetDlgItem(hWnd, OWNED_CAR_SLOT_COMBO);
    EnableWindow(h, TRUE);
    SendMessage(h, CB_SETCURSEL, 0, 0);

    h = GetDlgItem(hWnd, ALL_CARS_BTN);
    EnableWindow(h, TRUE);

    h = GetDlgItem(hWnd, ALL_TRACKS_BTN);
    EnableWindow(h, TRUE);

    h = GetDlgItem(hWnd, GOLD_TROPHIES_BTN);
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
    const char* p = strrchr(path, '\\');

    char newpath[MAX_PATH];
    strcpy(newpath, p && p[1] ? p + 1 : path);

    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = newpath;
    ofn.nMaxFile = sizeof(newpath);
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    if(!GetSaveFileNameA(&ofn)) return;

    if(NFSHSSaveEditor_update(editor, (const char*) newpath))
        MessageBoxA(hWnd, "Save(s) written.", "Info", MB_OK | MB_ICONINFORMATION);
    else
        MessageBoxA(hWnd, "Unable to write save(s) correctly.", "Error", MB_OK | MB_ICONERROR);
}

void UpdateSave(HWND hWnd, WPARAM wParam, NFSHSSaveEditor* editor) {
    switch(LOWORD(wParam)) {
        case LANGUAGE_COMBO: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            LRESULT lanidx = SendDlgItemMessageA(hWnd, LANGUAGE_COMBO, CB_GETCURSEL, 0, 0);

            char lan[MAX_BUF_SIZE];
            SendDlgItemMessageA(hWnd, LANGUAGE_COMBO, CB_GETLBTEXT, (WPARAM) lanidx, (LPARAM) lan);

            NFSHSSaveEditor_updatelang(editor, saveidx, lan);
        }
        break;

        case MONEY_TXT: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);

            char moneybuf[MAX_BUF_SIZE];
            GetDlgItemTextA(hWnd, MONEY_TXT, moneybuf, sizeof(moneybuf));
            char* end = NULL;
            int32_t money = strtol(moneybuf, &end, 10);

            if(end != moneybuf && *end == '\0')
                NFSHSSaveEditor_updatemoney(editor, saveidx, money);
        }
        break;

        case OWNED_CAR_MODEL_COMBO:
        case OWNED_CAR_UPGRADE_COMBO:
        case OWNED_CAR_COLOR_COMBO: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            LRESULT carslotidx = SendDlgItemMessageA(hWnd, OWNED_CAR_SLOT_COMBO, CB_GETCURSEL, 0, 0);
            LRESULT modidx = SendDlgItemMessageA(hWnd, OWNED_CAR_MODEL_COMBO, CB_GETCURSEL, 0, 0);
            LRESULT upgrade = SendDlgItemMessageA(hWnd, OWNED_CAR_UPGRADE_COMBO, CB_GETCURSEL, 0, 0);
            LRESULT color = SendDlgItemMessageA(hWnd, OWNED_CAR_COLOR_COMBO, CB_GETCURSEL, 0, 0);

            char model[MAX_BUF_SIZE];
            SendDlgItemMessageA(hWnd, OWNED_CAR_MODEL_COMBO, CB_GETLBTEXT, (WPARAM) modidx, (LPARAM) model);

            NFSHSSaveEditor_updateownedcar(editor, saveidx, carslotidx, (const char*) model, upgrade, color);
        }
        break;

        case ALL_CARS_BTN: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_unlockcars(editor, saveidx);

        }
        break;

        case ALL_TRACKS_BTN: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_unlocktracks(editor, saveidx);

        }
        break;

        case GOLD_TROPHIES_BTN: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_setgoldtrophies(editor, saveidx);

        }
        break;
    }
}

void ProcessCmd(HWND hWnd, WPARAM wParam, NFSHSSaveEditor* editor) {
    switch(LOWORD(wParam)) {
        case OPEN_BTN:
            OpenSave(hWnd, editor);
            break;

        case WRITE_BTN:
            WriteSave(hWnd, editor);
            break;

        case SAVE_COMBO:
            if(HIWORD(wParam) == CBN_SELCHANGE) {
                LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
                LRESULT carslotidx = SendDlgItemMessageA(hWnd, OWNED_CAR_SLOT_COMBO, CB_GETCURSEL, 0, 0);
                PrintSaveById(hWnd, editor, TRUE, TRUE, saveidx, carslotidx);
            }
            break;

        case LANGUAGE_COMBO:
        case OWNED_CAR_MODEL_COMBO:
        case OWNED_CAR_UPGRADE_COMBO:
        case OWNED_CAR_COLOR_COMBO:
            if(HIWORD(wParam) == CBN_SELCHANGE) UpdateSave(hWnd, wParam, editor);
            break;

        case MONEY_TXT:
            if(HIWORD(wParam) == EN_CHANGE) UpdateSave(hWnd, wParam, editor);
            break;

        case OWNED_CAR_SLOT_COMBO:
            if(HIWORD(wParam) == CBN_SELCHANGE) {
                LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
                LRESULT carslotidx = SendDlgItemMessageA(hWnd, OWNED_CAR_SLOT_COMBO, CB_GETCURSEL, 0, 0);
                PrintSaveById(hWnd, editor, FALSE, TRUE, saveidx, carslotidx);
            }
            break;

        case ALL_CARS_BTN:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "All cars unlocked.", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case ALL_TRACKS_BTN:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "All tracks unlocked.", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case GOLD_TROPHIES_BTN:
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