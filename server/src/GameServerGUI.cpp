#include "GameServerGUI.h"
#include "Utils/Console.h"
#include <iostream>
#include <cassert>
#include "GameServerGUI.h"

using namespace Console;

void GameServerGUI::RegisterWindowClass() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GameServerUI";
    RegisterClass(&wc);
}

void GameServerGUI::CreateAppWindow() {
    hwnd = CreateWindowExA(0, "GameServerUI", "Gestion du Serveur", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, hInstance, this);

    if (!hwnd) {
        Out << TextColors::FgCyan << ">>> Error on window creation.\n";
        return;
    }

    // Création des éléments UI
    CreateWindowA("BUTTON", "Create party", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        20, 50, 150, 30, hwnd, (HMENU)ID_BUTTON_CREATE, NULL, NULL);
    CreateWindowA("BUTTON", "View party infos", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        20, 100, 150, 30, hwnd, (HMENU)ID_BUTTON_VIEW, NULL, NULL);
    CreateWindowA("BUTTON", "Delete party", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        20, 150, 150, 30, hwnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL);
    hwndListBox = CreateWindowA("LISTBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
        200, 50, 250, 150, hwnd, (HMENU)ID_LISTBOX, NULL, NULL);
    hwndPlayerList = CreateWindowA("LISTBOX", "", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
        20, 220, 430, 200, hwnd, (HMENU)ID_PLAYERLIST, NULL, NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    SetTimer(hwnd, 1, 1000, NULL);
}

void GameServerGUI::MessageLoop() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void GameServerGUI::UpdateListBox()
{
    SendMessageA(hwndListBox, LB_RESETCONTENT, 0, 0);
    /*if (GameManager* manager = GameManager::GetInstance())
    {
        const GameManager::PartyList& partyList = manager->GetPartyList();
        if (partyList.empty()) return;

        for (const Party& p : partyList)
        {
            char buffer[50];
            sprintf_s(buffer, "Party %d | 2/4 players", p.GetID());
            SendMessageA(hwndListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
        }
    }*/
}

void GameServerGUI::OnListBoxSelection() {
    int selIndex = SendMessageA(hwndListBox, LB_GETCURSEL, 0, 0);
    if (selIndex != LB_ERR) {
        int textLength = SendMessageA(hwndListBox, LB_GETTEXTLEN, selIndex, 0);
        if (textLength > 0)
        {
            char* gameName = new char[textLength + 1];
            ZeroMemory(gameName, textLength + 1);

            SendMessageA(hwndListBox, LB_GETTEXT, selIndex, (LPARAM)gameName);
            int partyID = ExtractPartyID(std::string(gameName));

            if (partyID <= 0) {
                MessageBoxA(hwnd, "Party ID is undefined", "Nooooooooooooo", MB_OK | MB_ICONERROR);
                return;
            }

            m_iSelectedPartyId = partyID;
            delete[] gameName;
        }
    }
}

void GameServerGUI::UpdatePlayerList()
{
    SendMessageA(hwndPlayerList, LB_RESETCONTENT, 0, 0);
    /*if (GameManager* manager = GameManager::GetInstance())
    {
        const GameManager::PlayerList& playerList = manager->GetPlayerList();
        if (playerList.empty()) return;

        for (const Player& p : playerList)
        {
            char buffer[256];
            std::tm bt{};
            char time[64];
            localtime_s(&bt, &p.connectionStart);
            strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", &bt);
            sprintf_s(buffer, "%s | %s:%d | %s", p.username.c_str(), p.ip.c_str(), p.port, time);
            SendMessageA(hwndPlayerList, LB_ADDSTRING, 0, (LPARAM)buffer);
        }
    }*/
}

int GameServerGUI::ExtractPartyID(const std::string& partyString)
{
    size_t pos = partyString.find("Party ");
    if (pos != std::string::npos) {
        pos += 6;
        size_t endPos = partyString.find(" |", pos);
        std::string idStr = partyString.substr(pos, endPos - pos);
        return std::stoi(idStr);
    }
    return -1;
}

LRESULT GameServerGUI::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BUTTON_CREATE:
        {
            /*if (GameManager* manager = GameManager::GetInstance())
            {
                int ID = manager->CreateParty();
                if (ID > 0)
                {
                    std::string log = "New party created : ";
                    std::cout << log << ID << std::endl;
                    UpdateListBox();
                }
                else
                {
                    std::cout << "Max party reached (20)." << std::endl;
                }
            }*/
        }
        break;
        case ID_BUTTON_VIEW:
            MessageBoxA(hwnd, "Affichage des infos de partie...", "Info", MB_OK);
            break;
        case ID_BUTTON_DELETE:
        {
            /*if (GameManager* manager = GameManager::GetInstance())
            {
                int result = MessageBoxA(hwnd, "Are you sure to delete this party?", "Are you ok?", MB_OKCANCEL);
                if (result == IDOK)
                {
                    if (manager->RemovePartyByID(m_iSelectedPartyId) > 0)
                    {
                        std::string log = "Party deleted : ";
                        std::cout << log << m_iSelectedPartyId << std::endl;
                        m_iSelectedPartyId = 0;
                        UpdateListBox();
                    }
                    else
                        MessageBoxA(hwnd, "Select party before deletion.", "No no no..", MB_OK);
                }
            }*/
        }
        break;
        case ID_LISTBOX:
        {
            if (HIWORD(wParam) == LBN_SELCHANGE)
                OnListBoxSelection();
        }
        break;
        }
        break;


    case WM_TIMER:
    {
        /*if (GameManager::GetInstance()->IsPlayerListUpdated())
            UpdatePlayerList();
        if (GameManager::GetInstance()->IsPartyListUpdated())
            UpdateListBox();*/
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
