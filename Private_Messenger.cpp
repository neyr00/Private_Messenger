#include <iostream>
#include "header.h"

HFONT CreateMyFont(int size) {
    return CreateFont(
        size,                        // Высота шрифта
        0,                           // Ширина шрифта
        0,                           // Угол наклона
        0,                           // Ориентация базиса
        FW_NORMAL,                   // Толщина шрифта
        FALSE,                       // Курсив
        FALSE,                       // Подчеркивание
        FALSE,                       // Перечеркивание
        DEFAULT_CHARSET,             // Набор символов
        OUT_DEFAULT_PRECIS,          // Точность вывода
        CLIP_DEFAULT_PRECIS,         // Точность отсечения
        DEFAULT_QUALITY,             // Качество вывода
        DEFAULT_PITCH | FF_SWISS,    // Семейство и шаг
        L"Roboto"                    // Имя шрифта
    );
}

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
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return;
    }

    ShowWindow(hwnd, nCmdShow);
    ShowEnterScreen();
}

void ShowEnterScreen() {
    HFONT hFont = CreateMyFont(20);
    hIP = CreateWindowEx(
        0,
        L"STATIC",
        L"IP",
        WS_CHILD | WS_VISIBLE,
        10, 10, 25, 20,
        hwnd,
        (HMENU)IDC_MAIN_IP,
        hInst,
        NULL
    );
    SendMessage(hIP, WM_SETFONT, (WPARAM)hFont, TRUE);
    hEditIP = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        65, 10, width - 285, 20,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_IP,
        hInst,
        NULL
    );
    SetWindowLongPtr(hEditIP, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtr(hEditIP, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc));
    SendMessage(hEditIP, EM_LIMITTEXT, 15, 0);

    hName = CreateWindowEx(
        0,
        L"STATIC",
        L"NAME",
        WS_CHILD | WS_VISIBLE,
        10, 40, 50, 20,
        hwnd,
        (HMENU)IDC_MAIN_NAME,
        hInst,
        NULL
    );
    SendMessage(hName, WM_SETFONT, (WPARAM)hFont, TRUE);
    hEditName = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        65, 40, width - 285, 20,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_NAME,
        hInst,
        NULL
    );
    SetWindowLongPtr(hEditName, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtr(hEditName, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc));
    SendMessage(hEditName, EM_LIMITTEXT, 15, 0);

    hButtonConnect = CreateWindowEx(
        0,
        L"BUTTON",
        L"Connect",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        width - 200, 10, 80, 50,
        hwnd,
        (HMENU)IDC_MAIN_BUTTON_CONNECT,
        hInst,
        NULL
    );
    SendMessage(hButtonConnect, WM_SETFONT, (WPARAM)hFont, TRUE);
    hButtonServer = CreateWindowEx(
        0,
        L"BUTTON",
        L"Start server",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        width - 110, 10, 100, 50,
        hwnd,
        (HMENU)IDC_MAIN_BUTTON_SERVER,
        hInst,
        NULL
    );
    SendMessage(hButtonServer, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void ShowChatInterface() {

    DestroyWindow(hIP);
    DestroyWindow(hEditIP);
    DestroyWindow(hName);
    DestroyWindow(hEditName);
    DestroyWindow(hButtonConnect);
    DestroyWindow(hButtonServer);

    hEditChat = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_READONLY | ES_MULTILINE | WS_VSCROLL,
        10, 10, width - 20, height - 60,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_CHAT,
        hInst,
        NULL
    );

    hEditMessage = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, height - 40, width - 110, 30,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_MESSAGE,
        hInst,
        NULL
    );
    SetWindowLongPtr(hEditMessage, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtr(hEditMessage, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc));

    hButtonSend = CreateWindowEx(
        0,
        L"BUTTON",
        L"Send",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        width - 90, height - 40, 80, 30,
        hwnd,
        (HMENU)IDC_MAIN_BUTTON_SEND,
        hInst,
        NULL
    );
}

void StartWaiting() {
    dotCount = 0;
    waiting = true;
    
    EnableWindow(hEditIP, false);
    EnableWindow(hEditName, false);
    EnableWindow(hButtonConnect, false);
    if (isServerWait)
        SetWindowText(hButtonServer, L"Stop server");
    else 
        EnableWindow(hButtonServer, false);
    hWait = CreateWindowEx(
        0,
        L"STATIC",
        waitText.c_str(),
        WS_CHILD | WS_VISIBLE,
        width/2 - 100, height/2, 200, 20,
        hwnd,
        (HMENU)IDC_MAIN_WAIT,
        hInst,
        NULL
    );
    SendMessage(hWait, WM_SETFONT, (WPARAM)CreateMyFont(20), TRUE);

    timerId = SetTimer(hwnd, 1, 500, NULL);
}

void StopWaiting() {
    waiting = false;
    EnableWindow(hEditIP, true);
    EnableWindow(hEditName, true);
    EnableWindow(hButtonConnect, true);
    if (isServerWait)
        SetWindowText(hButtonServer, L"Start server");
    else
        EnableWindow(hButtonServer, true);
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
    if (hEditIP)
        MoveWindow(hEditIP, 65, 10, width - 285, 20, TRUE);
    if (hEditName)
        MoveWindow(hEditName, 65, 40, width - 285, 20, TRUE);
    if (hButtonConnect)
        MoveWindow(hButtonConnect, width - 200, 10, 80, 50, TRUE);
    if (hButtonServer)
        MoveWindow(hButtonServer, width - 110, 10, 100, 50, TRUE);

    if (hEditChat)
        MoveWindow(hEditChat, 10, 10, width - 20, height - 60, TRUE);
    if (hEditMessage)
        MoveWindow(hEditMessage, 10, height - 40, width - 110, 30, TRUE);
    if (hButtonSend)
        MoveWindow(hButtonSend, width - 90, height - 40, 80, 30, TRUE);

    if (hWait)
        MoveWindow(hWait, width / 2 - 100, height / 2, 200, 20, TRUE);
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

            setName();

            isServerWait = false;
            StartWaiting();
            thread clientThread(startClient, serverIP);
            clientThread.detach();
        }
        else if (LOWORD(wParam) == IDC_MAIN_BUTTON_SERVER) {
            if (serverStarted) {
                stopConnection();
                StopWaiting();
            }
            else {
                setName();

                isServerWait = true;
                StartWaiting();
                thread serverThread(startServer);
                serverThread.detach();
            }
        }
        else if (LOWORD(wParam) == IDC_MAIN_BUTTON_SEND) {
            int len = GetWindowTextLength(hEditMessage) + 1;
            wchar_t* buf = new wchar_t[len];
            GetWindowText(hEditMessage, buf, len);
            wstring message(buf);
            delete[] buf;
            if (!message.empty()) {
                if (message == L"/disconnect") {
                    PostMessage(hwnd, WM_DISCONNECT, 0, 0);
                    break;
                }
                SetWindowText(hEditMessage, L"");
                sendMessages(message);
                appendMessageToChat(myName + L": " + message);
            }
        }
        break;
    }
    break;
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
        if(succesConnection)
            connect();
        break;
    }
    case WM_DISCONNECT: {            
        reset();
        break;
    }
    case WM_SIZE: {
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        ResizeControls(hwnd, width, height);
        break;
    }
    case WM_DESTROY: {
        if (connected)
            stopConnection();
        PostQuitMessage(0);
        break;
    }
    case WM_KILLFOCUS: {
        if ((HWND)wParam == hEditChat)
            SetFocus(hwnd);
        break;
    }
    case WM_GETMINMAXINFO:    {
        MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        mmi->ptMinTrackSize.x = 440; 
        mmi->ptMinTrackSize.y = 140; 
        return 0;
    }
    case WM_CTLCOLORSTATIC: {
        if ((HWND)lParam == hIP || (HWND)lParam == hName) {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0)); // Установить цвет текста (черный)
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // Установить цвет фона (белый)
            return (LRESULT)GetStockObject(WHITE_BRUSH); // Вернуть белую кисть
        }
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN: {
        if (wParam == VK_RETURN)
            if (GetFocus() == hEditIP)
                SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDC_MAIN_BUTTON_CONNECT, BN_CLICKED), (LPARAM)hButtonConnect);
            else if (GetFocus() == hEditName)
                if (GetWindowTextLength(hEditIP))
                    SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDC_MAIN_BUTTON_CONNECT, BN_CLICKED), (LPARAM)hButtonConnect);
                else
                    SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDC_MAIN_BUTTON_SERVER, BN_CLICKED), (LPARAM)hButtonServer);
            else if (GetFocus() == hEditMessage)
                SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDC_MAIN_BUTTON_SEND, BN_CLICKED), (LPARAM)hButtonSend);
        break;
    }
    case WM_CHAR: {
        if (GetKeyState(VK_CONTROL) & 0x8000) { // Проверка, нажата ли клавиша Ctrl
            if (wParam == 'A') { // Ctrl+A
                SendMessage(hWnd, EM_SETSEL, 0, -1);
                return 0;
            }
            else if (wParam == 'C') { // Ctrl+C
                SendMessage(hWnd, WM_COPY, 0, 0);
                return 0;
            }
            else if (wParam == 'V') { // Ctrl+V
                SendMessage(hWnd, WM_PASTE, 0, 0);
                return 0;
            }
            else if (wParam == 'X') { // Ctrl+X
                SendMessage(hWnd, WM_CUT, 0, 0);
                return 0;
            }
        }
        break;
    }
    }
    return CallWindowProc((WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA), hWnd, uMsg, wParam, lParam);
}

void startServer() {
    serverStarted = true;
    WSADATA wsaData;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        MessageBox(NULL, L"WSAStartup failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        MessageBox(NULL, L"Socket creation failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        MessageBox(NULL, L"Bind failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    if (listen(server_fd, 1) == SOCKET_ERROR) {
        MessageBox(NULL, L"Listen failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (sock == INVALID_SOCKET) {
        //MessageBox(NULL, L"Accept failed", L"Error", MB_OK);
        stopConnection();
        return;
    }
    succesConnection = true;
    PostMessage(hwnd, WM_STOP_WAIT, 0, 0);
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
        stopConnection();
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (InetPton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
        MessageBox(NULL, L"Invalid address", L"Error", MB_OK);
        stopConnection();
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        MessageBox(NULL, L"Connection failed", L"Error", MB_OK);
        stopConnection();
        return;
    }
    succesConnection = true;
    PostMessage(hwnd, WM_STOP_WAIT, 0, 0);
}

void receiveMessages() {
    int dataSize = 0;
    while (true) {
        if (recv(sock, (char*)&dataSize, sizeof(dataSize), 0) == 0) {
            reset();
            break;
        }

        wchar_t* buffer = new wchar_t[dataSize / sizeof(wchar_t) + 1];
        if (recv(sock, (char*)buffer, dataSize, 0) == 0) {
            reset();
            break;
        }
        buffer[dataSize / sizeof(wchar_t)] = L'\0';

        if (wstring(buffer) == L"/disconnect") {
            delete[] buffer;
            PostMessage(hwnd, WM_DISCONNECT, 0, 0);
            break;
        }
        appendMessageToChat(hisName + L": " + wstring(buffer));
        delete[] buffer;
    }
}

void sendMessages(const wstring& message) {
    if (!message.empty() && sock != INVALID_SOCKET) {
        int messageSize = message.size() * sizeof(wchar_t);
        send(sock, (char*)&messageSize, sizeof(messageSize), 0);
        send(sock, (char*)message.c_str(), messageSize, 0);
    }
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

void receiveName() {
    int dataSize = 0;
    if (recv(sock, (char*)&dataSize, sizeof(dataSize), 0) <= 0) {
        reset();
        return;
    }
    wchar_t* buffer = new wchar_t[dataSize / sizeof(wchar_t) + 1];
    if (recv(sock, (char*)buffer, dataSize, 0) <= 0) {
        delete[] buffer;
        reset();
        return;
    }
    buffer[dataSize / sizeof(wchar_t)] = L'\0';
    hisName = wstring(buffer);
    delete[] buffer;
}

void connect() {
    connected = true;
    sendMessages(myName);
    receiveName();
    thread(receiveMessages).detach();
    ShowChatInterface();
    appendMessageToChat(L"| Connected to " + hisName + L" |");
}

void stopConnection() {
    if(connected)
        sendMessages(L"/disconnect");
    if (sock != INVALID_SOCKET)
        closesocket(sock);
    if (server_fd != INVALID_SOCKET)
        closesocket(server_fd);
    serverStarted = false;
    connected = false;
    succesConnection = false;
    WSACleanup();
    if (waiting)
        PostMessage(hwnd, WM_STOP_WAIT, 0, 0);
}

void reset() {
    stopConnection();

    DestroyWindow(hEditChat);
    DestroyWindow(hEditMessage);
    DestroyWindow(hButtonSend);

    ShowEnterScreen();
}

void setName() {
    int len = GetWindowTextLength(hEditName) + 1;
    wchar_t* buf = new wchar_t[len];
    GetWindowText(hEditName, buf, len);
    wstring my_name(buf);
    if (my_name.empty())
        myName = L"anonym";
    else
        myName = my_name;
    delete[] buf;
}