#pragma once
#include <Windows.h>
#include <string>

#define ID_BUTTON_CREATE 1
#define ID_BUTTON_VIEW 2
#define ID_BUTTON_DELETE 3
#define ID_LISTBOX 4
#define ID_PLAYERLIST 5

class GameServerGUI
{
public:
    GameServerGUI(HINSTANCE hInstance) : hInstance(hInstance), hwnd(NULL), hwndListBox(NULL), hwndPlayerList(NULL) {}

    void Run() {
        RegisterWindowClass();
        CreateAppWindow();
        MessageLoop();
    }

private:
    HINSTANCE hInstance;
    HWND hwnd;
    HWND hwndListBox;
    HWND hwndPlayerList;
    int m_iSelectedPartyId = 0;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        GameServerGUI* self;
        if (uMsg == WM_NCCREATE) {
            self = static_cast<GameServerGUI*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        }
        else {
            self = reinterpret_cast<GameServerGUI*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (self) {
            return self->HandleMessage(hwnd, uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void RegisterWindowClass();
    void CreateAppWindow();
    void MessageLoop();
    void UpdateListBox();
    void OnListBoxSelection();

    void UpdatePlayerList();

    // Utils
    int ExtractPartyID(const std::string& partyString);

    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};