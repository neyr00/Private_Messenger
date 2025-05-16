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
    globalIP = getGlobalIP();
    localIP = getLocalIP();
    HFONT hFont = CreateMyFont(20);
    hIP = CreateWindowEx(
        0,
        L"EDIT",
        L"IP",
        WS_CHILD | WS_VISIBLE | ES_READONLY,
        5, 10, 50, 20,
        hwnd,
        (HMENU)IDC_IP,
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
        (HMENU)IDC_EDIT_IP,
        hInst,
        NULL
    );
    SetWindowLongPtr(hEditIP, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtr(hEditIP, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc));
    SendMessage(hEditIP, EM_LIMITTEXT, 15, 0);

    hName = CreateWindowEx(
        0,
        L"EDIT",
        L"NAME",
        WS_CHILD | WS_VISIBLE | ES_READONLY,
        5, 40, 50, 20,
        hwnd,
        (HMENU)IDC_NAME,
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
        (HMENU)IDC_EDIT_NAME,
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
        (HMENU)IDC_BUTTON_CONNECT,
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
        (HMENU)IDC_BUTTON_SERVER,
        hInst,
        NULL
    );
    SendMessage(hButtonServer, WM_SETFONT, (WPARAM)hFont, TRUE);
    hGlobalIPLabel = CreateWindowEx(
        0,
        L"EDIT",
        L"Your GLOBAL IP",
        WS_CHILD | WS_VISIBLE | ES_READONLY,
        5, 70, 130, 20,
        hwnd,
        (HMENU)IDC_GLOBAL_IP_LABEL,
        hInst,
        NULL
    );
    SendMessage(hGlobalIPLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    hLocalIPLabel = CreateWindowEx(
        0,
        L"EDIT",
        L"Your LOCAL IP",
        WS_CHILD | WS_VISIBLE | ES_READONLY,
        5, 100, 130, 20,
        hwnd,
        (HMENU)IDC_LOCAL_IP_LABEL,
        hInst,
        NULL
    );
    SendMessage(hLocalIPLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    hGlobalIP = CreateWindowEx(
        0,
        L"EDIT",
        globalIP.c_str(),
        WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
        140, 70, 140, 25,
        hwnd,
        (HMENU)IDC_GLOBAL_IP,
        hInst,
        NULL
    );
    SendMessage(hGlobalIP, WM_SETFONT, (WPARAM)hFont, TRUE);
    hLocalIP = CreateWindowEx(
        0,
        L"EDIT",
        localIP.c_str(),
        WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
        140, 100, 140, 25,
        hwnd,
        (HMENU)IDC_LOCAL_IP,
        hInst,
        NULL
    );
    SendMessage(hLocalIP, WM_SETFONT, (WPARAM)hFont, TRUE);
    hGlobalIPCopy = CreateWindowEx(
        0,
        L"BUTTON",
        L"Copy",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        285, 70, 50, 25,
        hwnd,
        (HMENU)IDC_GLOBAL_IP_COPY,
        hInst,
        NULL
    );
    SendMessage(hGlobalIPCopy, WM_SETFONT, (WPARAM)hFont, TRUE);
    hLocalIPCopy = CreateWindowEx(
        0,
        L"BUTTON",
        L"Copy",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        285, 100, 50, 25,
        hwnd,
        (HMENU)IDC_LOCAL_IP_COPY,
        hInst,
        NULL
    );
    SendMessage(hLocalIPCopy, WM_SETFONT, (WPARAM)hFont, TRUE);
}
void ShowChatInterface() {
    if (hIP) { DestroyWindow(hIP); hIP = NULL; }
    if (hEditIP) { DestroyWindow(hEditIP); hEditIP = NULL; }
    if (hName) { DestroyWindow(hName); hName = NULL; }
    if (hEditName) { DestroyWindow(hEditName); hEditName = NULL; }
    if (hButtonConnect) { DestroyWindow(hButtonConnect); hButtonConnect = NULL; }
    if (hButtonServer) { DestroyWindow(hButtonServer); hButtonServer = NULL; }
    if (hLocalIPLabel) { DestroyWindow(hLocalIPLabel); hLocalIPLabel = NULL; }
    if (hLocalIP) { DestroyWindow(hLocalIP); hLocalIP = NULL; }
    if (hLocalIPCopy) { DestroyWindow(hLocalIPCopy); hLocalIPCopy = NULL; }
    if (hGlobalIPLabel) { DestroyWindow(hGlobalIPLabel); hGlobalIPLabel = NULL; }
    if (hGlobalIP) { DestroyWindow(hGlobalIP); hGlobalIP = NULL; }
    if (hGlobalIPCopy) { DestroyWindow(hGlobalIPCopy); hGlobalIPCopy = NULL; }

    HFONT hFont = CreateMyFont(20);
    hEditChat = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"RichEdit50W",
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
        10, 50, width - 20, height - 100,
        hwnd,
        (HMENU)IDC_EDIT_CHAT,
        hInst,
        NULL
    );
    SendMessage(hEditChat, WM_SETFONT, (WPARAM)hFont, TRUE);
    hEditMessage = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        10, height - 40, width - 110, 30,
        hwnd,
        (HMENU)IDC_EDIT_MESSAGE,
        hInst,
        NULL
    );
    SendMessage(hEditMessage, EM_LIMITTEXT, n_his, 0);
    SendMessage(hEditMessage, WM_SETFONT, (WPARAM)hFont, TRUE);
    SetWindowLongPtr(hEditMessage, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtr(hEditMessage, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc));

    hButtonSend = CreateWindowEx(
        0,
        L"BUTTON",
        L"Send",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        width - 90, height - 40, 80, 30,
        hwnd,
        (HMENU)IDC_BUTTON_SEND,
        hInst,
        NULL
    );
    hButtonDisconnect = CreateWindowEx(
        0,
        L"BUTTON",
        L"X",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        width - 40, 10, 30, 30,
        hwnd,
        (HMENU)IDC_BUTTON_DISCONNECT,
        hInst,
        NULL
    );
    hHisName = CreateWindowEx(
        0,
        L"EDIT",
        (L"Chat with: " + hisName).c_str(),
        WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER,
        10, 10, width - 50, 30,
        hwnd,
        (HMENU)IDC_HISNAME,
        hInst,
        NULL
    );
    SendMessage(hHisName, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void StartWaiting() {
    dotCount = 0;
    waiting = true;
    
    EnableWindow(hEditIP, false);
    EnableWindow(hEditName, false);
    EnableWindow(hButtonConnect, false);
    EnableWindow(hLocalIPCopy, false);
    EnableWindow(hGlobalIPCopy, false);
    if (isServerWait)
        SetWindowText(hButtonServer, L"Stop server");
    else 
        EnableWindow(hButtonServer, false);
    hWait = CreateWindowEx(
        0,
        L"EDIT",
        waitText.c_str(),
        WS_CHILD | WS_VISIBLE | ES_READONLY,
        width/2 - 100, height/2, 200, 20,
        hwnd,
        (HMENU)IDC_WAIT,
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
    EnableWindow(hLocalIPCopy, true);
    EnableWindow(hGlobalIPCopy,true);
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

void ResizeControls() {
    if (hEditIP)
        MoveWindow(hEditIP, 65, 10, width - 285, 20, TRUE);
    if (hEditName)
        MoveWindow(hEditName, 65, 40, width - 285, 20, TRUE);
    if (hButtonConnect)
        MoveWindow(hButtonConnect, width - 200, 10, 80, 50, TRUE);
    if (hButtonServer)
        MoveWindow(hButtonServer, width - 110, 10, 100, 50, TRUE);

    if (hEditMessage)
        AdjustMessageControlHeight();
    if (hButtonSend)
        MoveWindow(hButtonSend, width - 90, height - 40, 80, 30, TRUE);
    if (hWait)
        MoveWindow(hWait, width / 2 - 100, height / 2, 200, 20, TRUE);
    if (hHisName)
        MoveWindow(hHisName, 10, 10, width - 50, 30, TRUE);
    if (hButtonDisconnect)
        MoveWindow(hButtonDisconnect, width - 40, 10, 30, 30, TRUE);
}
void AdjustMessageControlHeight() {
    HDC hdc = GetDC(hEditMessage);
    HFONT hOldFont = (HFONT)SelectObject(hdc, (HFONT)SendMessage(hEditMessage, WM_GETFONT, 0, 0));

    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    int lineHeight = tm.tmHeight;

    int lineCount = (int)SendMessage(hEditMessage, EM_GETLINECOUNT, 0, 0);

    SelectObject(hdc, hOldFont);
    ReleaseDC(hEditMessage, hdc);

    int newHeight = min(max(lineHeight * lineCount + 8, 30), height / 3);

    SetMessageHeight(newHeight);
}
void SetMessageHeight(int newHeight) {
    MoveWindow(hEditMessage, 10, height - newHeight - 10, width - 110, newHeight, TRUE);
    MoveWindow(hEditChat, 10, 50, width - 20, height - newHeight - 70, TRUE);

    SendMessage(hEditMessage, WM_VSCROLL, SB_BOTTOM, 0);
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_BUTTON_CONNECT) {
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
        else if (LOWORD(wParam) == IDC_GLOBAL_IP_COPY) {
            if (OpenClipboard(hwnd)) {
                EmptyClipboard();
                wstring globalIP = getGlobalIP();
                HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (globalIP.length() + 1) * sizeof(wchar_t));
                if (hMem) {
                    wchar_t* data = (wchar_t*)GlobalLock(hMem);
                    wcscpy_s(data, globalIP.length() + 1, globalIP.c_str());
                    GlobalUnlock(hMem);
                    SetClipboardData(CF_UNICODETEXT, hMem);
                }
                CloseClipboard();
            }
        }
        else if (LOWORD(wParam) == IDC_LOCAL_IP_COPY) {
            if (OpenClipboard(hwnd)) {
                EmptyClipboard();
                wstring localIP = getLocalIP();
                HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (localIP.length() + 1) * sizeof(wchar_t));
                if (hMem) {
                    wchar_t* data = (wchar_t*)GlobalLock(hMem);
                    wcscpy_s(data, localIP.length() + 1, localIP.c_str());
                    GlobalUnlock(hMem);
                    SetClipboardData(CF_UNICODETEXT, hMem);
                }
                CloseClipboard();
            }
        }
        else if (LOWORD(wParam) == IDC_BUTTON_SERVER) {
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
        else if (LOWORD(wParam) == IDC_BUTTON_SEND) {
            int len = GetWindowTextLength(hEditMessage) + 1;
            wchar_t* buf = new wchar_t[len];
            GetWindowText(hEditMessage, buf, len);
            wstring message(buf);
            delete[] buf;
            if (!message.empty()) {
                SetWindowText(hEditMessage, L"");
                sendMessages(message);
                addToChat(myName, message, true);
            }
            SetMessageHeight(30);
            SendMessage(hEditChat, WM_VSCROLL, SB_BOTTOM, 0);
        }
        else if (LOWORD(wParam) == IDC_BUTTON_DISCONNECT) {
            reset();
        }
        if (HIWORD(wParam) == EN_UPDATE && (HWND)lParam == hEditMessage) {
            AdjustMessageControlHeight();
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
        ResizeControls();
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
        mmi->ptMinTrackSize.y = 200; 
        return 0;
    }
    case WM_CTLCOLORSTATIC: {
        if ((HWND)lParam == hIP || (HWND)lParam == hName || 
            (HWND)lParam == hGlobalIPLabel || (HWND)lParam == hLocalIPLabel) {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkColor(hdcStatic, RGB(255, 255, 255));
            return (LRESULT)GetStockObject(WHITE_BRUSH);
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
                SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDC_BUTTON_CONNECT, BN_CLICKED), (LPARAM)hButtonConnect);
            else if (GetFocus() == hEditName)
                if (GetWindowTextLength(hEditIP))
                    SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDC_BUTTON_CONNECT, BN_CLICKED), (LPARAM)hButtonConnect);
                else
                    SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDC_BUTTON_SERVER, BN_CLICKED), (LPARAM)hButtonServer);
            else if (GetFocus() == hEditMessage)
                SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDC_BUTTON_SEND, BN_CLICKED), (LPARAM)hButtonSend);
        break;
    }
    case WM_CHAR: {
        if (wParam == VK_RETURN) {
            return 0; 
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
        MessageBox(hwnd, L"WSAStartup failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        MessageBox(hwnd, L"Socket creation failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        MessageBox(hwnd, L"Bind failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    if (listen(server_fd, 1) == SOCKET_ERROR) {
        MessageBox(hwnd, L"Listen failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (sock == INVALID_SOCKET) {
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
        MessageBox(hwnd, L"WSAStartup failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        MessageBox(hwnd, L"Socket creation failed", L"Error", MB_OK);
        stopConnection();
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (InetPton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
        MessageBox(hwnd, L"Invalid address", L"Error", MB_OK);
        stopConnection();
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        MessageBox(hwnd, L"Connection failed", L"Error", MB_OK);
        stopConnection();
        return;
    }
    succesConnection = true;
    PostMessage(hwnd, WM_STOP_WAIT, 0, 0);
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
void receiveName() {
    uint byteSize = 0;
    if (recv(sock, reinterpret_cast<char*>(&byteSize), sizeof(byteSize), 0) <= 0) {
        reset();
        return;
    }
    uint count = byteSize / sizeof(uint);
    vector<uint> data(count);
    if (recv(sock, reinterpret_cast<char*>(data.data()), byteSize, 0) <= 0) {
        reset();
        return;
    }
    hisName = decrypt(data);
}

void receiveMessages() {
    uint byteSize = 0;
    while (connected) {
        if (recv(sock, reinterpret_cast<char*>(&byteSize), sizeof(byteSize), 0) == 0) {
            PostMessage(hwnd, WM_DISCONNECT, 0, 0);
            MessageBox(hwnd, (hisName + L" disconnected").c_str(), L"Info", MB_OK | MB_APPLMODAL);
            return;
        }

        uint count = byteSize / sizeof(uint);
        vector<uint> data(count);
        if (recv(sock, reinterpret_cast<char*>(data.data()), byteSize, 0) == 0) {
            PostMessage(hwnd, WM_DISCONNECT, 0, 0);
            MessageBox(hwnd, (hisName + L" disconnected").c_str(), L"Info", MB_OK | MB_APPLMODAL);
            return;
        }
        wstring message = decrypt(data);
        if (!message.size()) {
            PostMessage(hwnd, WM_DISCONNECT, 0, 0);
            MessageBox(hwnd, (hisName + L" disconnected").c_str(), L"Info", MB_OK | MB_APPLMODAL);
            return;
        }
        addToChat(hisName, message, false);
    }
}
void sendMessages(const wstring& message) {
    if (!message.empty() && sock != INVALID_SOCKET) {
        vector<uint> encrypt_message = encrypt(message);
        uint byteSize = encrypt_message.size() * sizeof(uint);
        send(sock, reinterpret_cast<const char*>(&byteSize), sizeof(byteSize), 0);
        send(sock, reinterpret_cast<const char*>(encrypt_message.data()), byteSize, 0);
    }
}
void addToChat(const wstring& sender, const wstring& message, bool isOut) {
    // Получаем текущее время
    SYSTEMTIME time;
    GetLocalTime(&time);
    wchar_t timeStr[64];
    swprintf_s(timeStr, L"[%02d:%02d]", time.wHour, time.wMinute);

    // Для входящих сообщений
    if (!isOut) {
        // Установка выравнивания по левому краю
        PARAFORMAT2 pf;
        ZeroMemory(&pf, sizeof(pf));
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_ALIGNMENT;
        pf.wAlignment = PFA_LEFT;
        SendMessage(hEditChat, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

        // Если это первое сообщение или сменился отправитель
        if (lastWasOutgoing || firstMes) {
            firstMes = false;
            // Форматирование для имени отправителя (жирный)
            CHARFORMAT2 cf;
            ZeroMemory(&cf, sizeof(cf));
            cf.cbSize = sizeof(cf);
            cf.dwMask = CFM_BOLD;
            cf.dwEffects = CFE_BOLD;
            SendMessage(hEditChat, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

            // Вставка имени отправителя
            wstring senderLine = sender + L":\r\n";
            SendMessage(hEditChat, EM_REPLACESEL, FALSE, (LPARAM)senderLine.c_str());
        }
        
        // Форматирование времени (серый)
        CHARFORMAT2 cfTime;
        ZeroMemory(&cfTime, sizeof(cfTime));
        cfTime.cbSize = sizeof(cfTime);
        cfTime.dwMask = CFM_COLOR;
        cfTime.crTextColor = RGB(150, 150, 150);
        SendMessage(hEditChat, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfTime);

        // Вставка времени
        wstring timeWithSpace = wstring(timeStr) + L" ";
        SendMessage(hEditChat, EM_REPLACESEL, FALSE, (LPARAM)timeWithSpace.c_str());

        // Форматирование текста сообщения (черный)
        CHARFORMAT2 cfMsg;
        ZeroMemory(&cfMsg, sizeof(cfMsg));
        cfMsg.cbSize = sizeof(cfMsg);
        cfMsg.dwMask = CFM_COLOR;
        cfMsg.crTextColor = RGB(0, 0, 0);
        SendMessage(hEditChat, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfMsg);

        // Вставка текста сообщения
        SendMessage(hEditChat, EM_REPLACESEL, FALSE, (LPARAM)message.c_str());
    }
    // Для исходящих сообщений
    else {
        // Установка выравнивания по правому краю
        PARAFORMAT2 pf;
        ZeroMemory(&pf, sizeof(pf));
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_ALIGNMENT;
        pf.wAlignment = PFA_RIGHT;
        SendMessage(hEditChat, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

        // Форматирование текста сообщения (черный)
        CHARFORMAT2 cfMsg;
        ZeroMemory(&cfMsg, sizeof(cfMsg));
        cfMsg.cbSize = sizeof(cfMsg);
        cfMsg.dwMask = CFM_COLOR;
        cfMsg.crTextColor = RGB(0, 0, 0);
        SendMessage(hEditChat, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfMsg);

        // Вставка текста сообщения
        SendMessage(hEditChat, EM_REPLACESEL, FALSE, (LPARAM)message.c_str());

        // Форматирование времени (серый)
        CHARFORMAT2 cfTime;
        ZeroMemory(&cfTime, sizeof(cfTime));
        cfTime.cbSize = sizeof(cfTime);
        cfTime.dwMask = CFM_COLOR;
        cfTime.crTextColor = RGB(150, 150, 150);
        SendMessage(hEditChat, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfTime);

        // Вставка времени
        wstring timeWithSpace = L" " + wstring(timeStr);
        SendMessage(hEditChat, EM_REPLACESEL, FALSE, (LPARAM)timeWithSpace.c_str());
    }

    // Перевод строки после сообщения
    SendMessage(hEditChat, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");

    // Обновляем информацию о последнем отправителе
    lastWasOutgoing = isOut;

    SendMessage(hEditChat, EM_SCROLL, SB_BOTTOM, 0);
}

void connect() {
    connected = true;

    rsa(e, d, n);
    send(sock, reinterpret_cast<const char*>(&e), sizeof(e), 0);
    recv(sock, reinterpret_cast<char*>(&e_his), sizeof(e_his), 0);

    send(sock, reinterpret_cast<const char*>(&n), sizeof(n), 0);
    recv(sock, reinterpret_cast<char*>(&n_his), sizeof(n_his), 0);

    sendMessages(myName);
    receiveName();

    thread(receiveMessages).detach();
    ShowChatInterface();
}
void stopConnection() {
    connected = false;
    
    if (sock != INVALID_SOCKET) {
        shutdown(sock, SD_BOTH);
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    if (server_fd != INVALID_SOCKET) {
        closesocket(server_fd);
        server_fd = INVALID_SOCKET;
    }
    
    WSACleanup();
    
    serverStarted = false;
    succesConnection = false;
    isServerWait = false;
    lastWasOutgoing = false;
    firstMes = true;
    
    hisName.clear();
    
    e = 0;
    d = 0;
    n = 0;
    e_his = 0;
    n_his = 0;
    
    if (waiting)
        PostMessage(hwnd, WM_STOP_WAIT, 0, 0);
}
void reset() {
    stopConnection();

    if (hEditChat) { DestroyWindow(hEditChat); hEditChat = NULL; }
    if (hEditMessage) { DestroyWindow(hEditMessage); hEditMessage = NULL; }
    if (hButtonSend) { DestroyWindow(hButtonSend); hButtonSend = NULL; }
    if (hButtonDisconnect) { DestroyWindow(hButtonDisconnect); hButtonDisconnect = NULL; }
    if (hHisName) { DestroyWindow(hHisName); hHisName = NULL; }
    if (hIP) { DestroyWindow(hIP); hIP = NULL; }
    if (hEditIP) { DestroyWindow(hEditIP); hEditIP = NULL; }
    if (hName) { DestroyWindow(hName); hName = NULL; }
    if (hEditName) { DestroyWindow(hEditName); hEditName = NULL; }
    if (hButtonConnect) { DestroyWindow(hButtonConnect); hButtonConnect = NULL; }
    if (hButtonServer) { DestroyWindow(hButtonServer); hButtonServer = NULL; }

    ShowEnterScreen();
}

wstring getLocalIP() {
    ULONG bufferLength = 0;
    if (GetAdaptersAddresses(AF_INET, 0, NULL, NULL, &bufferLength) == ERROR_BUFFER_OVERFLOW) {
        vector<BYTE> buffer(bufferLength);
        IP_ADAPTER_ADDRESSES* pAddresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.data());

        if (GetAdaptersAddresses(AF_INET, 0, NULL, pAddresses, &bufferLength) == ERROR_SUCCESS) {
            for (IP_ADAPTER_ADDRESSES* adapter = pAddresses; adapter != NULL; adapter = adapter->Next) {
                if (adapter->OperStatus == IfOperStatusUp && adapter->IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
                    for (IP_ADAPTER_UNICAST_ADDRESS* address = adapter->FirstUnicastAddress;
                        address != NULL;
                        address = address->Next) {

                        if (address->Address.lpSockaddr->sa_family == AF_INET) {
                            SOCKADDR_IN* ipv4 = reinterpret_cast<SOCKADDR_IN*>(address->Address.lpSockaddr);
                            wchar_t ipStr[16];
                            InetNtop(AF_INET, &(ipv4->sin_addr), ipStr, 16);
                            wstring ip(ipStr);
                            if (ip.find(L"192.168") == 0 ||
                                ip.find(L"10.") == 0 ||
                                ip.find(L"172.16") == 0) {
                                return ip;
                            }
                        }
                    }
                }
            }
        }
    }
    return L"Not found";
}
wstring getGlobalIP() {
    HINTERNET hInternet = InternetOpen(L"IP_RETRIEVER",
        INTERNET_OPEN_TYPE_DIRECT,
        NULL,
        NULL,
        0);
    if (!hInternet) return L"Error";

    HINTERNET hConnect = InternetOpenUrl(hInternet,
        L"https://api.ipify.org",
        NULL,
        0,
        INTERNET_FLAG_RELOAD,
        0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return L"Error";
    }

    char buffer[16] = { 0 };
    DWORD bytesRead = 0;
    if (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        wstring result(buffer, buffer + bytesRead);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return result;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return L"Not found";
}

//RSA
void rsa(uint& e, uint& d, uint& n) {
    uint p = generate_prime(30);
    uint q, f;
    do {
        q = generate_prime(30);
        e = generate_prime(30);
        f = (p - 1) * (q - 1);
    } while (p == q || gcd(e, f) != 1);

    n = p * q;
    d = mod_inverse(e, f);
}
uint gcd(uint a, uint b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}
uint mod_inverse(uint a, uint m) {
    long long x = 1, y = 0, x1 = 0, y1 = 1;
    long long orig = m;

    while (m) {
        long long q = a / m;
        tie(x, x1) = make_tuple(x1, x - q * x1);
        tie(y, y1) = make_tuple(y1, y - q * y1);
        tie(a, m) = make_tuple(m, a % m);
    }

    return x < 0 ? x + orig : x;
}
uint mod_mul(uint a, uint b, uint mod) {
    uint result = 0;
    a %= mod;
    while (b) {
        if (b & 1)
            result = (result + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
}
uint mod_pow(uint base, uint exp, uint mod) {
    uint result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}
bool is_prime(uint n, int k) {
    if (n <= 3) return n > 1;
    if (n % 2 == 0) return false;

    uint d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint> dis(2, n - 2);

    for (int i = 0; i < k; i++) {
        uint a = dis(gen);
        uint x = mod_pow(a, d, n);

        if (x == 1 || x == n - 1) continue;

        for (int j = 0; j < s - 1 && x != n - 1; j++) {
            x = mod_pow(x, 2, n);
            if (x == 1) return false;
        }

        if (x != n - 1) return false;
    }
    return true;
}
uint generate_prime(int bits) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint> dis(1ULL << (bits - 1), (1ULL << bits) - 1);

    uint candidate;
    do {
        candidate = dis(gen) | 1;
    } while (!is_prime(candidate));

    return candidate;
}

vector<uint> encrypt(const wstring& message) {
    vector<uint> encrypted;
    size_t block_chars = static_cast<size_t>(log2(n_his) / (CHAR_BIT * sizeof(wchar_t))) - 1;
    if (block_chars == 0) block_chars = 1;

    for (size_t i = 0; i < message.size(); i += block_chars) {
        uint block_value = 0;
        size_t chars_in_block = 0;
        
        for (size_t j = 0; j < block_chars && i + j < message.size(); j++) {
            block_value = (block_value << (CHAR_BIT * sizeof(wchar_t))) | static_cast<uint>(message[i + j]);
            chars_in_block++;
        }
        
        block_value = (block_value << 8) | static_cast<uint>(chars_in_block);
        encrypted.push_back(mod_pow(block_value, e_his, n_his));
    }
    return encrypted;
}
wstring decrypt(const vector<uint>& encrypted) {
    wstring decrypted;
    size_t block_chars = static_cast<size_t>(log2(n) / (CHAR_BIT * sizeof(wchar_t))) - 1;
    if (block_chars == 0) block_chars = 1;

    for (uint cipher : encrypted) {
        uint block_value = mod_pow(cipher, d, n);
        
        uint chars_in_block = block_value & 0xFF;
        block_value >>= 8;
        
        wstring block;
        for (size_t j = 0; j < chars_in_block; j++) {
            wchar_t c = static_cast<wchar_t>(block_value & ((1ULL << (CHAR_BIT * sizeof(wchar_t))) - 1));
            block.insert(0, 1, c);
            block_value >>= (CHAR_BIT * sizeof(wchar_t));
        }
        
        decrypted += block;
    }
    return decrypted;
}