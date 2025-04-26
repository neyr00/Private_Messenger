#include <iostream>
#include "header.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    CreateInterface(hInstance, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void CreateInterface(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;
    cmd = nCmdShow;
    const wchar_t CLASS_NAME[] = L"Messenger";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Private Messenger",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 520, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return;
    }

    ShowWindow(hwnd, nCmdShow);

    hEditIP = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, 10, 300, 30,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_IP,
        hInstance,
        NULL
    );

    hButtonConnect = CreateWindowEx(
        0,
        L"BUTTON",
        L"Connect",
        WS_CHILD | WS_VISIBLE,
        320, 10, 80, 30,
        hwnd,
        (HMENU)IDC_MAIN_BUTTON_CONNECT,
        hInstance,
        NULL
    );
    hButtonServer = CreateWindowEx(
        0,
        L"BUTTON",
        L"Start server",
        WS_CHILD | WS_VISIBLE,
        410, 10, 90, 30,
        hwnd,
        (HMENU)IDC_MAIN_BUTTON_SERVER,
        hInstance,
        NULL
    );
    hEditName = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, 50, 300, 30,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_NAME,
        hInstance,
        NULL
    );
}

void ShowChatInterface() {

    DestroyWindow(hEditIP);
    DestroyWindow(hButtonConnect);
    DestroyWindow(hButtonServer);
    DestroyWindow(hEditName);

    hEditChat = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY | ES_MULTILINE | WS_VSCROLL,
        10, 10, 480, 200,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_CHAT,
        GetModuleHandle(NULL),
        NULL
    );

    hEditMessage = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, 220, 390, 30,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_MESSAGE,
        GetModuleHandle(NULL),
        NULL
    );

    hButtonSend = CreateWindowEx(
        0,
        L"BUTTON",
        L"Send",
        WS_CHILD | WS_VISIBLE,
        410, 220, 80, 30,
        hwnd,
        (HMENU)IDC_MAIN_BUTTON_SEND,
        GetModuleHandle(NULL),
        NULL
    );
}

void StartWaiting() {
    dotCount = 0;
    waiting = true;
    
    EnableWindow(hEditIP, FALSE);
    EnableWindow(hEditName, FALSE);
    EnableWindow(hButtonConnect, FALSE);
    SetWindowText(hButtonServer, L"Stop server");
    hWait = CreateWindowEx(
        0,
        L"STATIC",
        waitText.c_str(),
        WS_CHILD | WS_VISIBLE,
        140, 100, 180, 20,
        hwnd,
        (HMENU)IDC_MAIN_WAIT,
        GetModuleHandle(NULL),
        NULL
    );

    timerId = SetTimer(hwnd, 1, 500, NULL);
}

void StopWaiting() {
    waiting = false;
    EnableWindow(hEditIP, true);
    EnableWindow(hEditName, true);
    EnableWindow(hButtonConnect, true);
    SetWindowText(hButtonServer, L"Start server");
    if (timerId != 0) {
        KillTimer(hwnd, timerId);
        timerId = 0;
    }
    if (hWait) {
        DestroyWindow(hWait);
        hWait = NULL;
    }
}

void ResizeControls(HWND hwnd, int width, int height) {
    if (hEditChat)
        MoveWindow(hEditChat, 10, 10, width - 20, height - 60, TRUE);
    if (hEditMessage)
        MoveWindow(hEditMessage, 10, height - 40, width - 110, 30, TRUE);
    if (hButtonSend)
        MoveWindow(hButtonSend, width - 90, height - 40, 80, 30, TRUE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_MAIN_BUTTON_CONNECT) {
            int len = GetWindowTextLength(hEditIP) + 1;
            wchar_t* buf = new wchar_t[len];
            GetWindowText(hEditIP, buf, len);
            wstring serverIP(buf);
            delete[] buf;

            len = GetWindowTextLength(hEditName) + 1;
            buf = new wchar_t[len];
            GetWindowText(hEditName, buf, len);
            wstring my_name(buf);
            if (my_name.empty())
                set_myName(L"anonym");
            else
                set_myName(my_name);
            delete[] buf;

            startClient(serverIP);
        }
        else if (LOWORD(wParam) == IDC_MAIN_BUTTON_SERVER) {
            if (serverStarted) {
                //stopServer();
                serverStarted = false;
                StopWaiting();
            }
            else {
                int len = GetWindowTextLength(hEditName) + 1;
                wchar_t* buf = new wchar_t[len];
                GetWindowText(hEditName, buf, len);
                wstring my_name(buf);
                if (my_name.empty())
                    set_myName(L"anonym");
                else
                    set_myName(my_name);
                delete[] buf;

                StartWaiting();
                thread(startServer).detach();
            }
        }
        else if (LOWORD(wParam) == IDC_MAIN_BUTTON_SEND) {
            int len = GetWindowTextLength(hEditMessage) + 1;
            wchar_t* buf = new wchar_t[len];
            GetWindowText(hEditMessage, buf, len);
            wstring message(buf);
            delete[] buf;
            if (!message.empty()) {
                SetWindowText(hEditMessage, L"");
                sendMessages(message);
                appendMessageToChat(get_myName() + L": " + message);
            }
        }
        break;
    }
    case WM_TIMER: {
        if (waiting) {
            dotCount = (dotCount + 1) % 4;
            wstring newText = waitText + wstring(dotCount, L'.');
            SetWindowText(hWait, newText.c_str());
        }
        break;
    }
    case WM_START_WAIT: {
        StartWaiting();
        break;
    }
    case WM_STOP_WAIT: {
        StopWaiting();
        connect();
        break;
    }
    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        ResizeControls(hwnd, width, height);
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    case WM_KILLFOCUS: {
        if ((HWND)wParam == hEditChat)
            SetFocus(hwnd);
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void startServer() {
    serverStarted = true;
    WSADATA wsaData;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        MessageBox(NULL, L"WSAStartup failed", L"Error", MB_OK);
        //stopServer();
        return;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        MessageBox(NULL, L"Socket creation failed", L"Error", MB_OK);
        //stopServer();
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        MessageBox(NULL, L"Bind failed", L"Error", MB_OK);
        //stopServer();
        return;
    }

    if (listen(server_fd, 1) == SOCKET_ERROR) {
        MessageBox(NULL, L"Listen failed", L"Error", MB_OK);
        //stopServer();
        return;
    }

    sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (sock == INVALID_SOCKET) {
        MessageBox(NULL, L"Accept failed", L"Error", MB_OK);
        //stopServer();
        return;
    }
    PostMessage(hwnd, WM_STOP_WAIT, 0, 0);
    ExitThread(0);
}
void stopServer() {
    closesocket(server_fd);
    WSACleanup();
    PostMessage(hwnd, WM_STOP_WAIT, 0, 0);
    serverStarted = false;
}
void startClient(const wstring& serverIP) {
    WSADATA wsaData;
    struct sockaddr_in serv_addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        MessageBox(NULL, L"WSAStartup failed", L"Error", MB_OK);
        return;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        MessageBox(NULL, L"Socket creation failed", L"Error", MB_OK);
        WSACleanup();
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (InetPton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
        MessageBox(NULL, L"Invalid address", L"Error", MB_OK);
        closesocket(sock);
        WSACleanup();
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        MessageBox(NULL, L"Connection failed", L"Error", MB_OK);
        closesocket(sock);
        WSACleanup();
        return;
    }
    connect();
}

void receiveMessages(SOCKET sock) {
    wchar_t buffer[BUFFER_SIZE] = { 0 };
    while (true) {
        int bytesReceived = recv(sock, (char*)buffer, BUFFER_SIZE * sizeof(wchar_t) - sizeof(wchar_t), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            reset();
            break;
        }
        buffer[bytesReceived / sizeof(wchar_t)] = L'\0';
        appendMessageToChat(get_hisName() + L": " + wstring(buffer));
    }
}

void sendMessages(const wstring& message) {
    if (!message.empty() && sock != INVALID_SOCKET)
        send(sock, (char*)message.c_str(), message.size() * sizeof(wchar_t), 0);
}

void appendMessageToChat(const wstring& message) {
    int len = GetWindowTextLength(hEditChat) + 1;
    wchar_t* buf = new wchar_t[len + message.size() + 3]; // +3 for "\r\n" and null terminator
    GetWindowText(hEditChat, buf, len);
    wcscat_s(buf, len + message.size() + 3, L"\r\n");
    wcscat_s(buf, len + message.size() + 3, message.c_str());
    SetWindowText(hEditChat, buf);
    delete[] buf;

    SendMessage(hEditChat, WM_VSCROLL, SB_BOTTOM, 0);
}

void sendName(wstring name) {
    if (!name.empty() && sock != INVALID_SOCKET)
        send(sock, (char*)name.c_str(), name.size() * sizeof(wchar_t), 0);
}

void receiveName(SOCKET sock) {
    wchar_t buffer[BUFFER_SIZE] = { 0 };
    int bytesReceived = recv(sock, (char*)buffer, BUFFER_SIZE * sizeof(wchar_t) - sizeof(wchar_t), 0);
    buffer[bytesReceived / sizeof(wchar_t)] = L'\0';
    set_hisName(wstring(buffer));
}

void set_myName(wstring name) {
    myName = name;
}

wstring get_myName() {
    return myName;
}

void set_hisName(wstring name) {
    hisName = name;
}

wstring get_hisName() {
    return hisName;
}

void connect() {
    sendName(get_myName());
    receiveName(sock);
    thread(receiveMessages, sock).detach();
    ShowChatInterface();
}


void reset() {
    DestroyWindow(hEditChat);
    DestroyWindow(hEditMessage);
    DestroyWindow(hButtonSend);

    CreateInterface(hInst, cmd);

    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    WSACleanup();
}