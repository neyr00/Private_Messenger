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
    HFONT hFont = CreateMyFont(20);
    hEditChat = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"RichEdit50W",
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
        10, 10, width - 20, height - 60,
        hwnd,
        (HMENU)IDC_MAIN_EDIT_CHAT,
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
        (HMENU)IDC_MAIN_EDIT_MESSAGE,
        hInst,
        NULL
    );
    SendMessage(hEditMessage, WM_SETFONT, (WPARAM)hFont, TRUE);
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

void ResizeControls() {
    if (hEditIP)
        MoveWindow(hEditIP, 65, 10, width - 285, 20, TRUE);
    if (hEditName)
        MoveWindow(hEditName, 65, 40, width - 285, 20, TRUE);
    if (hButtonConnect)
        MoveWindow(hButtonConnect, width - 200, 10, 80, 50, TRUE);
    if (hButtonServer)
        MoveWindow(hButtonServer, width - 110, 10, 100, 50, TRUE);

    if (hEditChat) {
        MoveWindow(hEditChat, 10, 10, width - 20, height - 60, TRUE);
        SendMessage(hEditChat, WM_VSCROLL, SB_BOTTOM, 0);
    }

    if (hEditMessage)
        AdjustMessageControlHeight();

    if (hButtonSend)
        MoveWindow(hButtonSend, width - 90, height - 40, 80, 30, TRUE);

    if (hWait)
        MoveWindow(hWait, width / 2 - 100, height / 2, 200, 20, TRUE);
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
    MoveWindow(hEditChat, 10, 10, width - 20, height - newHeight - 30, TRUE);

    SendMessage(hEditChat, WM_VSCROLL, SB_BOTTOM, 0);
    SendMessage(hEditMessage, WM_VSCROLL, SB_BOTTOM, 0);
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
                addToChat(myName, message, true);
            }
            SetMessageHeight(30);
            SendMessage(hEditChat, WM_VSCROLL, SB_BOTTOM, 0);
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

void receiveMessages() {
    ll byteSize = 0;
    while (true) {
        if (recv(sock, reinterpret_cast<char*>(&byteSize), sizeof(byteSize), 0) == 0) {
            reset();
            break;
        }

        ll count = byteSize / sizeof(ll);
        vector<ll> data(count);
        if (recv(sock, reinterpret_cast<char*>(data.data()), byteSize, 0) == 0) {
            reset();
            break;
        }
        wstring message = decrypt(data);
        if (message == L"/disconnect") {
            PostMessage(hwnd, WM_DISCONNECT, 0, 0);
            MessageBox(hwnd, (hisName + L" disconnected").c_str(), L"Info", MB_OK | MB_APPLMODAL);
            break;
        }
        addToChat(hisName, message, false);
    }
}

void sendMessages(const wstring& message) {
    if (!message.empty() && sock != INVALID_SOCKET) {
        vector<ll> encrypt_message = encrypt(message);
        ll byteSize = encrypt_message.size() * sizeof(ll);
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

    // Прокрутка вниз
    SendMessage(hEditChat, EM_SCROLL, SB_BOTTOM, 0);
}
void receiveName() {
    ll byteSize = 0;
    if (recv(sock, reinterpret_cast<char*>(&byteSize), sizeof(byteSize), 0) <= 0) {
        reset();
        return;
    }
    ll count = byteSize / sizeof(ll);
    vector<ll> data(count);
    if (recv(sock, reinterpret_cast<char*>(data.data()), byteSize, 0) <= 0) {
        reset();
        return;
    }
    hisName = decrypt(data);
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
    //addToChat(L"| Connected to " + hisName + L" |");
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
    lastWasOutgoing = false;
    firstMes = false;
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


ll gcd(ll a, ll b) {
    while (b != 0) {
        ll temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
ll mod_inverse(ll a, ll m) {
    ll m0 = m, t, q;
    ll x0 = 0, x1 = 1;
    if (m == 1)
        return 0;
    while (a > 1) {
        q = a / m;
        t = m, m = a % m, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
    }
    if (x1 < 0)
        x1 += m0;
    return x1;
}   
ll mod_pow(ll base, ll exponent, ll mod) {
    ll result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exponent = exponent >> 1;
    }
    return result;
}
void rsa(ll& e, ll& d, ll& n) {
    random_device rd;
    mt19937 gen(rd());

    vector<ll> primes = primes_arr(10000, 50000);

    uniform_int_distribution<> dis(0, primes.size() - 1);
    uniform_int_distribution<> e_gen(primes.size()/2, primes.size() - 1);
    ll p, q;
    ll f;
    
    do {
        p = primes[dis(gen)];
        q = primes[dis(gen)];
        e = primes[e_gen(gen)];
        f = (p - 1) * (q - 1);
    } while (p == q || gcd(e, f) != 1);

    n = p * q;
    d = mod_inverse(e, f);
}
vector<ll> primes_arr(int a, int b) {
    vector<ll> primes;
    vector<bool> is_prime(b + 1, true);  
    is_prime[0] = is_prime[1] = false;   

    for (int p = 2; p * p <= b; p++)
        if (is_prime[p])
            for (int i = p * p; i <= b; i += p)
                is_prime[i] = false;
    for (int i = a; i <= b; i++)
        if (is_prime[i])
            primes.push_back(i);
    return primes;
}

vector<ll> encrypt(wstring message) {
    vector<ll> encrypted;
    for (wchar_t c : message) {
        ll m = static_cast<ll>(c);
        ll cipher = mod_pow(m, e_his, n_his);
        encrypted.push_back(cipher);
    }
    return encrypted;
}
wstring decrypt(const vector<ll>& encrypted) {
    wstring decrypted;
    for (ll cipher : encrypted) {
        ll m = mod_pow(cipher, d, n);
        decrypted += static_cast<wchar_t>(m);
    }
    return decrypted;
}