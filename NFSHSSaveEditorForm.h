#define _CRT_SECURE_NO_WARNINGS

#include "NFSHSSaveEditor.h"
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>

#ifdef _MSC_VER
#pragma comment(lib, "Comctl32.lib")
#endif

#define WND_CLASS_NAME "NFSHSSaveEditor"
#define WND_TITLE "NFSHS Save Editor"
#define WND_WIDTH 380
#define WND_HEIGHT 460
#define WND_Y_POS 30
#define MAX_BUF_SIZE 50

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
#define NO_CAR_RESTRICTION_BTN 1017

char path[MAX_PATH] = {0};
NFSHSSaveEditor editor = {0};

void InitGui(HWND hWnd) {
    SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLongPtr(hWnd, GWL_STYLE) & ~((LONG_PTR) WS_MAXIMIZEBOX));

    HWND h = CreateWindowExA(0, TOOLBARCLASSNAMEA, NULL, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST, 0, 0, 0, 0, hWnd, (HMENU) (INT_PTR) TOOLBAR, NULL, NULL);
    SendMessageA(h, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    LRESULT idx = SendMessageA(h, TB_ADDSTRINGA, 0, (LPARAM) "Open\0Write\0");
    TBBUTTON buttons[] = {{I_IMAGENONE, OPEN_BTN, TBSTATE_ENABLED, BTNS_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR) idx},
                          {I_IMAGENONE, WRITE_BTN, 0, BTNS_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR) (idx + 1)}};
    SendMessageA(h, TB_ADDBUTTONS, ARRAYSIZE(buttons), (LPARAM) buttons);
    SendMessageA(h, TB_AUTOSIZE, 0, 0);
    SendMessageA(h, CCM_SETBKCOLOR, 0, RGB(255, 255, 255));

    int y = WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Path:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 250, 20, hWnd, (HMENU) (INT_PTR) PATH_TXT, NULL, NULL);
    SendMessageA(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Format:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 250, 20, hWnd, (HMENU) (INT_PTR) FORMAT_TXT, NULL, NULL);
    SendMessageA(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Save:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_COMBOBOXA, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 250, 200, hWnd, (HMENU) (INT_PTR) SAVE_COMBO, NULL, NULL);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Serial:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 250, 20, hWnd, (HMENU) (INT_PTR) SERIAL_TXT, NULL, NULL);
    SendMessageA(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Name:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 250, 20, hWnd, (HMENU) (INT_PTR) PLAYER_NAME_TXT, NULL, NULL);
    SendMessageA(h, EM_SETREADONLY, TRUE, 0);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Language:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_COMBOBOXA, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 250, 200, hWnd, (HMENU) (INT_PTR) LANGUAGE_COMBO, NULL, NULL);
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_languages); i++)
        SendMessageA(h, CB_ADDSTRING, 0, (LPARAM) NFSHSSaveEditor_languages[i]);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Money:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 50, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_EDITA, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, y, 250, 20, hWnd, (HMENU) (INT_PTR) MONEY_TXT, NULL, NULL);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    char buf[MAX_BUF_SIZE] = {0};

    CreateWindowExA(0, WC_STATICA, "Car slot:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_COMBOBOXA, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 250, 200, hWnd, (HMENU) (INT_PTR) OWNED_CAR_SLOT_COMBO, NULL, NULL);
    for(size_t i = 0; i < OWNED_CAR_SLOT_COUNT; i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessageA(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Model:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_COMBOBOXA, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 250, 200, hWnd, (HMENU) (INT_PTR) OWNED_CAR_MODEL_COMBO, NULL, NULL);
    SendMessageA(h, CB_ADDSTRING, 0, (LPARAM) "None");
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_models); i++)
        SendMessageA(h, CB_ADDSTRING, 0, (LPARAM) NFSHSSaveEditor_models[i]);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Upgrade:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_COMBOBOXA, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 250, 200, hWnd, (HMENU) (INT_PTR) OWNED_CAR_UPGRADE_COMBO, NULL, NULL);
    for(size_t i = 0; i < ARRAY_SIZE(NFSHSSaveEditor_upgrades); i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessageA(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    CreateWindowExA(0, WC_STATICA, "Color:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, y, 80, 20, hWnd, NULL, NULL, NULL);
    h = CreateWindowExA(0, WC_COMBOBOXA, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 100, y, 250, 200, hWnd, (HMENU) (INT_PTR) OWNED_CAR_COLOR_COMBO, NULL, NULL);
    for(size_t i = 0; i < OWNED_CAR_COLOR_COUNT; i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessageA(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    h = CreateWindowExA(0, WC_BUTTONA, "All cars", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, y, 100, 20, hWnd, (HMENU) (INT_PTR) ALL_CARS_BTN, NULL, NULL);
    EnableWindow(h, FALSE);

    h = CreateWindowExA(0, WC_BUTTONA, "All tracks", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 130, y, 100, 20, hWnd, (HMENU) (INT_PTR) ALL_TRACKS_BTN, NULL, NULL);
    EnableWindow(h, FALSE);

    h = CreateWindowExA(0, WC_BUTTONA, "Gold trophies", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 250, y, 100, 20, hWnd, (HMENU) (INT_PTR) GOLD_TROPHIES_BTN, NULL, NULL);
    EnableWindow(h, FALSE);
    y += WND_Y_POS;

    h = CreateWindowExA(0, WC_BUTTONA, "No car restriction", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, y, 150, 20, hWnd, (HMENU) (INT_PTR) NO_CAR_RESTRICTION_BTN, NULL, NULL);
    EnableWindow(h, FALSE);
}

void ResetGui(HWND hWnd) {
    HWND h = GetDlgItem(hWnd, TOOLBAR);
    SendMessageA(h, TB_ENABLEBUTTON, (WPARAM) WRITE_BTN, MAKELONG(FALSE, 0));

    h = GetDlgItem(hWnd, PATH_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, FORMAT_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, SAVE_COMBO);
    SendMessageA(h, CB_RESETCONTENT, 0, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, SERIAL_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, PLAYER_NAME_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, LANGUAGE_COMBO);
    SendMessageA(h, CB_SETCURSEL, -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, MONEY_TXT);
    SetWindowTextA(h, "");
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_SLOT_COMBO);
    SendMessageA(h, CB_SETCURSEL, -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_MODEL_COMBO);
    SendMessageA(h, CB_SETCURSEL, -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_UPGRADE_COMBO);
    SendMessageA(h, CB_SETCURSEL, -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, OWNED_CAR_COLOR_COMBO);
    SendMessageA(h, CB_SETCURSEL, -1, 0);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, ALL_CARS_BTN);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, ALL_TRACKS_BTN);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, GOLD_TROPHIES_BTN);
    EnableWindow(h, FALSE);

    h = GetDlgItem(hWnd, NO_CAR_RESTRICTION_BTN);
    EnableWindow(h, FALSE);
}

void UpdateGui(HWND hWnd, NFSHSSaveEditor* editor, const BOOL upsavedata, const BOOL upcardata, const size_t saveidx, const size_t carslotidx) {
    if(upsavedata) {
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
        SendMessageA(h, CB_SETCURSEL, 0, 0);
        if(editor->saves[saveidx].language) {
            LRESULT lanidx = SendMessageA(h, CB_FINDSTRINGEXACT, -1, (LPARAM) editor->saves[saveidx].language);
            SendMessageA(h, CB_SETCURSEL, (WPARAM) lanidx, 0);
        }

        h = GetDlgItem(hWnd, MONEY_TXT);
        EnableWindow(h, TRUE);
        SetDlgItemInt(hWnd, MONEY_TXT, editor->saves[saveidx].money, TRUE);
    }

    if(upcardata) {
        HWND h = GetDlgItem(hWnd, OWNED_CAR_MODEL_COMBO);
        EnableWindow(h, TRUE);
        SendMessageA(h, CB_SETCURSEL, 0, 0);
        if(editor->saves[saveidx].ownedcars[carslotidx].model) {
            LRESULT modidx = SendMessageA(h, CB_FINDSTRINGEXACT, -1, (LPARAM) editor->saves[saveidx].ownedcars[carslotidx].model);
            SendMessageA(h, CB_SETCURSEL, (WPARAM) modidx, 0);
        }

        h = GetDlgItem(hWnd, OWNED_CAR_UPGRADE_COMBO);
        EnableWindow(h, TRUE);
        SendMessageA(h, CB_SETCURSEL, (WPARAM) editor->saves[saveidx].ownedcars[carslotidx].upgrade, 0);

        h = GetDlgItem(hWnd, OWNED_CAR_COLOR_COMBO);
        EnableWindow(h, TRUE);
        SendMessageA(h, CB_SETCURSEL, (WPARAM) editor->saves[saveidx].ownedcars[carslotidx].color, 0);
    }
}

void PrintSave(HWND hWnd, NFSHSSaveEditor* editor, const char* path) {
    NFSHSSaveEditor_init(editor, (const char*) path);

    if(editor->savecount == 0) {
        NFSHSSaveEditor_clear(editor);
        MessageBoxA(hWnd, "No NFSHS save found", "Error", MB_OK | MB_ICONERROR);
        ResetGui(hWnd);
        return;
    }

    HWND h = GetDlgItem(hWnd, TOOLBAR);
    SendMessageA(h, TB_ENABLEBUTTON, (WPARAM) WRITE_BTN, MAKELONG(TRUE, 0));

    h = GetDlgItem(hWnd, PATH_TXT);
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

    char buf[MAX_BUF_SIZE] = {0};

    h = GetDlgItem(hWnd, SAVE_COMBO);
    EnableWindow(h, TRUE);
    SendMessageA(h, CB_RESETCONTENT, 0, 0);
    for(size_t i = 0; i < editor->savecount; i++) {
        snprintf(buf, sizeof(buf), "%d", (int) i);
        SendMessageA(h, CB_ADDSTRING, 0, (LPARAM) buf);
    }
    SendMessageA(h, CB_SETCURSEL, 0, 0);

    h = GetDlgItem(hWnd, OWNED_CAR_SLOT_COMBO);
    EnableWindow(h, TRUE);
    SendMessageA(h, CB_SETCURSEL, 0, 0);

    h = GetDlgItem(hWnd, ALL_CARS_BTN);
    EnableWindow(h, TRUE);

    h = GetDlgItem(hWnd, ALL_TRACKS_BTN);
    EnableWindow(h, TRUE);

    h = GetDlgItem(hWnd, GOLD_TROPHIES_BTN);
    EnableWindow(h, TRUE);

    h = GetDlgItem(hWnd, NO_CAR_RESTRICTION_BTN);
    EnableWindow(h, TRUE);

    UpdateGui(hWnd, editor, TRUE, TRUE, 0, 0);
}

void OpenSave(HWND hWnd, NFSHSSaveEditor* editor) {
    OPENFILENAMEA ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.nMaxFile = sizeof(path);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetOpenFileNameA(&ofn))
        PrintSave(hWnd, editor, (const char*) path);
}

void WriteSave(HWND hWnd, NFSHSSaveEditor* editor) {
    const char* p = strrchr(path, '\\');

    char newpath[MAX_PATH] = {0};
    strcpy(newpath, p && p[1] ? p + 1 : path);

    OPENFILENAMEA ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = newpath;
    ofn.nMaxFile = sizeof(newpath);
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    if(!GetSaveFileNameA(&ofn)) return;

    if(NFSHSSaveEditor_update(editor, (const char*) newpath))
        MessageBoxA(hWnd, "Save(s) written", "Info", MB_OK | MB_ICONINFORMATION);
    else
        MessageBoxA(hWnd, "Unable to write save(s) correctly", "Error", MB_OK | MB_ICONERROR);
}

void UpdateSave(HWND hWnd, WPARAM wParam, NFSHSSaveEditor* editor) {
    switch(LOWORD(wParam)) {
        case LANGUAGE_COMBO: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            LRESULT lanidx = SendDlgItemMessageA(hWnd, LANGUAGE_COMBO, CB_GETCURSEL, 0, 0);

            char buf[MAX_BUF_SIZE] = {0};
            SendDlgItemMessageA(hWnd, LANGUAGE_COMBO, CB_GETLBTEXT, (WPARAM) lanidx, (LPARAM) buf);
            NFSHSSaveEditor_updatelang(editor, (size_t) saveidx, (const char*) buf);
        }
        break;

        case MONEY_TXT: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);

            char buf[MAX_BUF_SIZE] = {0};
            GetDlgItemTextA(hWnd, MONEY_TXT, buf, sizeof(buf));
            char* end = NULL;
            int32_t money = strtol(buf, &end, 10);

            if(end != buf && *end == '\0')
                NFSHSSaveEditor_updatemoney(editor, (size_t) saveidx, money);
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

            char buf[MAX_BUF_SIZE] = {0};
            SendDlgItemMessageA(hWnd, OWNED_CAR_MODEL_COMBO, CB_GETLBTEXT, (WPARAM) modidx, (LPARAM) buf);
            NFSHSSaveEditor_updateownedcar(editor, (size_t) saveidx, (size_t) carslotidx, (const char*) buf, (uint8_t) upgrade, (uint8_t) color);
        }
        break;

        case ALL_CARS_BTN: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_unlockcars(editor, (size_t) saveidx);
        }
        break;

        case ALL_TRACKS_BTN: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_unlocktracks(editor, (size_t) saveidx);
        }
        break;

        case GOLD_TROPHIES_BTN: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_setgoldtrophies(editor, (size_t) saveidx);
        }
        break;

        case NO_CAR_RESTRICTION_BTN: {
            LRESULT saveidx = SendDlgItemMessageA(hWnd, SAVE_COMBO, CB_GETCURSEL, 0, 0);
            NFSHSSaveEditor_setnocarrestriction(editor, (size_t) saveidx);
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
                UpdateGui(hWnd, editor, TRUE, TRUE, (size_t) saveidx, (size_t) carslotidx);
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
                UpdateGui(hWnd, editor, FALSE, TRUE, (size_t) saveidx, (size_t) carslotidx);
            }
            break;

        case ALL_CARS_BTN:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "All cars unlocked", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case ALL_TRACKS_BTN:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "All tracks unlocked", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case GOLD_TROPHIES_BTN:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "Gold trophies unlocked", "Info", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case NO_CAR_RESTRICTION_BTN:
            if(HIWORD(wParam) == BN_CLICKED) {
                UpdateSave(hWnd, wParam, editor);
                MessageBoxA(hWnd, "No car restriction unlocked", "Info", MB_OK | MB_ICONINFORMATION);
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
            res = DefWindowProcA(hWnd, msg, wParam, lParam);
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
    return CreateWindowExA(0, WND_CLASS_NAME, WND_TITLE, WS_VISIBLE | (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL);
}