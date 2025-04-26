#ifndef HEADER_H
#define HEADER_H
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
#define IDC_MAIN_EDIT_NAME 107
#define IDC_MAIN_WAIT 108
#define WM_START_WAIT WM_USER + 1
#define WM_STOP_WAIT WM_USER + 2
#define WM_DISCONNECT WM_USER + 3

HWND hEditIP;
HWND hButtonConnect;
HWND hButtonServer;
HWND hEditMessage;
HWND hButtonSend;
HWND hEditChat;
HWND hEditName;
HWND hWait;
HWND hwnd;

//LOGIC FUNC
void startServer();
void stopConnection();
void startClient(const wstring& serverIP);
void receiveMessages();
void sendMessages(const wstring& message);
void appendMessageToChat(const wstring& message);
void sendName();
void receiveName();
wstring myName;
wstring hisName;
void connect();
void reset();
void setName();

//VAR
bool waiting = false;
bool serverStarted = false;
bool connected = false;
UINT_PTR timerId = 0;
wstring waitText = L"Waiting for connections";
int dotCount = 0;
HINSTANCE hInst;
int cmd;
SOCKET sock = INVALID_SOCKET;
SOCKET server_fd = INVALID_SOCKET;
int width = 520;
int height = 300;


//INTERFACE
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WaitingWindowProc(HWND, UINT, WPARAM, LPARAM);
void CreateInterface(HINSTANCE hInstance, int nCmdShow);
void ShowEnterScreen();
void ShowChatInterface();
void ResizeControls(HWND hwnd, int width, int height);
void StartWaiting();
void StopWaiting();

#endif