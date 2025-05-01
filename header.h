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
HMODULE hRichEditLib = LoadLibrary(TEXT("Msftedit.dll"));

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define PORT 3826
#define IDC_MAIN_EDIT_IP 101
#define IDC_MAIN_BUTTON_CONNECT 102
#define IDC_MAIN_EDIT_MESSAGE 103
#define IDC_MAIN_BUTTON_SEND 104
#define IDC_MAIN_EDIT_CHAT 105
#define IDC_MAIN_BUTTON_SERVER 106
#define IDC_MAIN_EDIT_NAME 107
#define IDC_MAIN_WAIT 108
#define IDC_MAIN_IP 109
#define IDC_MAIN_NAME 110
#define WM_START_WAIT WM_USER + 1
#define WM_STOP_WAIT WM_USER + 2
#define WM_DISCONNECT WM_USER + 3
using ll = long long;

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

//LOGIC FUNC
void startServer();
void stopConnection();
void startClient(const wstring& serverIP);
void receiveMessages();
void sendMessages(const wstring& message);
void addToChat(const wstring& sender, const wstring& message, bool isOut);
void receiveName();
wstring myName;
wstring hisName;
void connect();
void reset();
void setName();

//RSA
void rsa(ll& e, ll& d, ll& n);
ll mod_pow(ll base, ll exponent, ll mod);
vector<ll> encrypt(wstring message);
wstring decrypt(const vector<ll>& encrypted);
ll mod_inverse(ll a, ll m);
ll gcd(ll a, ll b);
ll e = 65537, d, n; // e,n - public; d,n - private
ll e_his, n_his;
vector<ll> primes_arr(int a, int b);

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