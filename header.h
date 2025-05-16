#ifndef HEADER_H
#define HEADER_H
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <commctrl.h>
#include <random>
#include <tuple>
#include <vector>
#include <cmath>
#include <richedit.h>
#include <iphlpapi.h>
#include <wininet.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "wininet.lib")
HMODULE hRichEditLib = LoadLibrary(TEXT("Msftedit.dll"));

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define PORT 3826
#define IDC_EDIT_IP 101
#define IDC_BUTTON_CONNECT 102
#define IDC_EDIT_MESSAGE 103
#define IDC_BUTTON_SEND 104
#define IDC_EDIT_CHAT 105
#define IDC_BUTTON_SERVER 106
#define IDC_EDIT_NAME 107
#define IDC_WAIT 108
#define IDC_IP 109
#define IDC_NAME 110
#define IDC_HISNAME 111
#define IDC_BUTTON_DISCONNECT 112
#define IDC_LOCAL_IP 113
#define IDC_GLOBAL_IP 114
#define IDC_GLOBAL_IP_LABEL 115
#define IDC_GLOBAL_IP_COPY 116
#define IDC_LOCAL_IP_LABEL 117
#define IDC_LOCAL_IP_COPY 118
#define WM_START_WAIT WM_USER + 1
#define WM_STOP_WAIT WM_USER + 2
#define WM_DISCONNECT WM_USER + 3
using uint = uint64_t;

HWND hEditIP;
HWND hIP;
HWND hButtonConnect;
HWND hButtonServer;
HWND hEditMessage;
HWND hButtonSend;
HWND hEditChat;
HWND hEditName;
HWND hWait;
HWND hName;
HWND hwnd;
HWND hHisName;
HWND hButtonDisconnect;
HWND hGlobalIP;
HWND hLocalIP;
HWND hGlobalIPLabel;
HWND hLocalIPLabel;
HWND hGlobalIPCopy;
HWND hLocalIPCopy;

//LOGIC FUNC
void startServer();
void stopConnection();
void startClient(const wstring& serverIP);
void receiveMessages();
void sendMessages(const wstring& message);
void addToChat(const wstring& sender, const wstring& message, bool isOut);
void receiveName();
void connect();
void reset();
void setName();
wstring getLocalIP();
wstring getGlobalIP();

//RSA
void rsa(uint& e, uint& d, uint& n);
uint gcd(uint a, uint b);
uint mod_inverse(uint a, uint m);
uint mod_mul(uint a, uint b, uint mod);
uint mod_pow(uint base, uint exp, uint mod);
bool is_prime(uint n, int k = 40);
uint generate_prime(int bits);
vector<uint> encrypt(const wstring& message);
wstring decrypt(const vector<uint>& encrypted);
//RSA VAR
uint e = 65537, d, n; // e,n - public; d,n - private
uint e_his, n_his;

//VAR
bool waiting = false;
bool isServerWait;
bool serverStarted = false;
bool connected = false;
bool succesConnection = false;
UINT_PTR timerId = 0;
wstring waitText = L"Waiting for connections";
int dotCount = 0;
HINSTANCE hInst;
int cmd;
SOCKET sock = INVALID_SOCKET;
SOCKET server_fd = INVALID_SOCKET;
int width = 800;
int height = 600;
bool lastWasOutgoing = false;
bool firstMes = true;
wstring myName;
wstring hisName;
wstring localIP;
wstring globalIP;

//INTERFACE
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateInterface(HINSTANCE hInstance, int nCmdShow);
void ShowEnterScreen();
void ShowChatInterface();
void ResizeControls();
void StartWaiting();
void StopWaiting();
HFONT CreateMyFont(int size);
void AdjustMessageControlHeight();
void SetMessageHeight(int newHeight);
#endif