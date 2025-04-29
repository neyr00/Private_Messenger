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
                addToChat(myName + L": " + message);
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
    int byteSize = 0;
    while (true) {
        if (recv(sock, reinterpret_cast<char*>(&byteSize), sizeof(byteSize), 0) == 0) {
            reset();
            break;
        }

        int count = byteSize / sizeof(ll);
        vector<ll> result(count);
        if (recv(sock, reinterpret_cast<char*>(result.data()), byteSize, 0) == 0) {
            reset();
            break;
        }
        wstring res = decrypt(result);
        if (res == L"/disconnect") {
            PostMessage(hwnd, WM_DISCONNECT, 0, 0);
            break;
        }
        addToChat(hisName + L": " + res);
    }
}

void sendMessages(const wstring& message) {
    if (!message.empty() && sock != INVALID_SOCKET) {
        auto encrypt_message = encrypt(message);
        int byteSize = static_cast<int>(encrypt_message.size() * sizeof(ll));
        send(sock, reinterpret_cast<const char*>(&byteSize), sizeof(byteSize), 0);
        send(sock, reinterpret_cast<const char*>(encrypt_message.data()), byteSize, 0);
    }
}

void addToChat(const wstring& message) {
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
    int byteSize = 0;
    if (recv(sock, reinterpret_cast<char*>(&byteSize), sizeof(byteSize), 0) <= 0) {
        reset();
        return;
    }
    int count = byteSize / sizeof(ll);
    vector<ll> result(count);
    if (recv(sock, reinterpret_cast<char*>(result.data()), byteSize, 0) <= 0) {
        reset();
        return;
    }
    hisName = decrypt(result);
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
    addToChat(L"| Connected to " + hisName + L" |");
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

// Функция для вычисления НОД по алгоритму Евклида
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

    vector<ll> primes = {
        1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063,
        1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217,
        1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373,
        1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
        1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657,
        1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811,
        1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
        1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129,
        2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287,
        2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423,
        2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617,
        2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741,
        2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903,
        2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079,
        3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257,
        3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413,
        3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571,
        3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643, 3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727,
        3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907,
        3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989, 4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057,
        4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139, 4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231,
        4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297, 4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409,
        4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493, 4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583,
        4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657, 4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751,
        4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 4861, 4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937,
        4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003, 5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087,
        5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179, 5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279,
        5281, 5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443,
        5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521, 5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639,
        5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693, 5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791,
        5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857, 5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939,
        5953, 5981, 5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133,
        6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301,
        6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473,
        6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673,
        6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761, 6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833,
        6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997,
        7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103, 7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207,
        7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411,
        7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499, 7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561,
        7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723,
        7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829, 7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919
    };

    uniform_int_distribution<> dis(0, primes.size() - 1);
    ll p, q;
    ll f;

    do {
        p = primes[dis(gen)];
        q = primes[dis(gen)];
        f = (p - 1) * (q - 1);
    } while (p == q || gcd(e, f) != 1);

    n = p * q;
    d = mod_inverse(e, f);
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