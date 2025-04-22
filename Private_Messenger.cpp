#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 3826
#define BUFFER_SIZE 1024
#define IDC_MAIN_EDIT_IP 101
#define IDC_MAIN_BUTTON_CONNECT 102
#define IDC_MAIN_EDIT_MESSAGE 103
#define IDC_MAIN_BUTTON_SEND 104
#define IDC_MAIN_EDIT_CHAT 105
#define IDC_MAIN_BUTTON_SERVER 106

HWND hEditIP;
HWND hButtonConnect;
HWND hButtonServer;
HWND hEditMessage;
HWND hButtonSend;
HWND hEditChat;
SOCKET sock = INVALID_SOCKET;
bool isServer = false;

void startServer();
void startClient(const string& serverIP);
void receiveMessages(SOCKET sock);
void sendMessages(const string& message);
void appendMessageToChat(const string& message);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateInterface(HINSTANCE hInstance, int nCmdShow);
void ShowChatInterface();
void ResizeControls(HWND hwnd, int width, int height);

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
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Chat Application",
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
}

void ShowChatInterface() {
    HWND hwnd = GetParent(hEditIP);

    DestroyWindow(hEditIP);
    DestroyWindow(hButtonConnect);
    DestroyWindow(hButtonServer);

    hEditChat = CreateWindowEx(
        0,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        10, 10, 390, 180,
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
        10, 200, 300, 30,
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
        320, 200, 80, 30,
        hwnd,
        (HMENU)IDC_MAIN_BUTTON_SEND,
        GetModuleHandle(NULL),
        NULL
    );
}

void ResizeControls(HWND hwnd, int width, int height) {
    if (hEditChat) {
        MoveWindow(hEditChat, 10, 10, width - 20, height - 120, TRUE);
    }
    if (hEditMessage) {
        MoveWindow(hEditMessage, 10, height - 100, width - 110, 30, TRUE);
    }
    if (hButtonSend) {
        MoveWindow(hButtonSend, width - 90, height - 100, 80, 30, TRUE);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_MAIN_BUTTON_CONNECT) {
            int len = GetWindowTextLength(hEditIP) + 1;
            wchar_t* buf = new wchar_t[len];
            GetWindowText(hEditIP, buf, len);
            string serverIP(buf, buf + wcslen(buf));
            delete[] buf;

            if (serverIP.empty()) {
                isServer = true;
                thread(startServer).detach();
            }
            else {
                isServer = false;
                thread(startClient, serverIP).detach();
            }

            ShowChatInterface();
        }
        else if (LOWORD(wParam) == IDC_MAIN_BUTTON_SERVER) {
            int len = GetWindowTextLength(hEditIP) + 1;
            wchar_t* buf = new wchar_t[len];
            GetWindowText(hEditIP, buf, len);
            string serverIP(buf, buf + wcslen(buf));
            delete[] buf;


            isServer = true;
            thread(startServer).detach();

            ShowChatInterface();
        }
        else if (LOWORD(wParam) == IDC_MAIN_BUTTON_SEND) {
            int len = GetWindowTextLength(hEditMessage) + 1;
            wchar_t* buf = new wchar_t[len];
            GetWindowText(hEditMessage, buf, len);
            string message(buf, buf + wcslen(buf));
            delete[] buf;
            SetWindowText(hEditMessage, L"");
            sendMessages(message);
            appendMessageToChat("You: " + message);
        }
        break;
    }
    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        ResizeControls(hwnd, width, height);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void startServer() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        MessageBox(NULL, L"WSAStartup failed", L"Error", MB_OK);
        return;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        MessageBox(NULL, L"Socket creation failed", L"Error", MB_OK);
        WSACleanup();
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        MessageBox(NULL, L"Bind failed", L"Error", MB_OK);
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    if (listen(server_fd, 1) == SOCKET_ERROR) {
        MessageBox(NULL, L"Listen failed", L"Error", MB_OK);
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket == INVALID_SOCKET) {
        MessageBox(NULL, L"Accept failed", L"Error", MB_OK);
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    sock = new_socket;
    thread(receiveMessages, new_socket).detach();
}

void startClient(const string& serverIP) {
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
    if (inet_pton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
        MessageBox(NULL, L"Invalid address/ Address not supported", L"Error", MB_OK);
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

    thread(receiveMessages, sock).detach();
}

void receiveMessages(SOCKET sock) {
    char buffer[BUFFER_SIZE] = { 0 };
    while (true) {
        int bytesReceived = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) break;
        buffer[bytesReceived] = '\0';
        appendMessageToChat("Other: " + string(buffer));
    }
}

void sendMessages(const string& message) {
    if (sock != INVALID_SOCKET) {
        send(sock, message.c_str(), message.size(), 0);
    }
}

void appendMessageToChat(const string& message) {
    int len = GetWindowTextLength(hEditChat) + 1;
    wchar_t* buf = new wchar_t[len + message.size() + 3]; // +3 for "\r\n" and null terminator
    GetWindowText(hEditChat, buf, len);
    wcscat_s(buf, len + message.size() + 3, L"\r\n");
    wcscat_s(buf, len + message.size() + 3, wstring(message.begin(), message.end()).c_str());
    SetWindowText(hEditChat, buf);
    delete[] buf;

    SendMessage(hEditChat, WM_VSCROLL, SB_BOTTOM, 0);
}
