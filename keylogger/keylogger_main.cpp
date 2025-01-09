#include <windows.h>
#include <WinUser.h>

#include "keylogger/keylog.pb.h"

#include <iostream>
#include <fstream>

#include <chrono>
#include <format>

namespace {
std::ofstream                                      logfile;
std::chrono::time_point<std::chrono::system_clock> start_time;
} // namespace

void process_kbd_event(unsigned short makeCode, bool isE0, bool isE1, bool isKeyUp) {
    auto key_timestamp = std::chrono::system_clock::now();
    using std::chrono::duration_cast;
    using std::chrono::hours;
    using std::chrono::milliseconds;
    using std::chrono::minutes;
    using std::chrono::seconds;
    using namespace std::chrono_literals;
    auto ms = duration_cast<milliseconds>(key_timestamp - start_time);
    auto timestamp
        = std::format("{} {} {} {}", duration_cast<hours>(ms), duration_cast<minutes>(ms % 1h),
                      duration_cast<seconds>(ms % 1min), ms % 1s);

    keylog::proto::KeyLog keylog;
    auto*                 evt = keylog.add_keyevents();

    evt->set_state(isKeyUp ? keylog::proto::Released : keylog::proto::Pressed);
    evt->set_scancode(makeCode);
    evt->set_timestamp_ms(ms.count());
    evt->set_ise0(isE0);
    evt->set_ise1(isE1);

    keylog.SerializeToOstream(&logfile);
    logfile.flush();

    std::cout << timestamp << "\t" << (isKeyUp ? '^' : 'v') << ' ' << (isE0 ? "E0 " : "")
              << (isE1 ? "E1 " : "") << std::hex << makeCode << std::dec << std::endl;
}

// to receive events for the rawkeyboard data
LRESULT CALLBACK wndproc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    if (message != WM_INPUT)
        return DefWindowProc(window, message, wparam, lparam);

    char rid_buf[64];
    UINT rid_size = sizeof(rid_buf);

    if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, rid_buf, &rid_size,
                        sizeof(RAWINPUTHEADER))) {
        RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rid_buf);

        if (raw->header.dwType == RIM_TYPEKEYBOARD) {
            RAWKEYBOARD* rk = &raw->data.keyboard;
            process_kbd_event(rk->MakeCode, rk->Flags & RI_KEY_E0, rk->Flags & RI_KEY_E1,
                              rk->Flags & RI_KEY_BREAK);
        }
    }
    return DefWindowProc(window, message, wparam, lparam);
}

int main(int argc, char* argv[]) {
    if (argc < 2)
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <filename>");

    logfile.open(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);
    start_time = std::chrono::system_clock::now();

    // define a window class which is required to receive RAWINPUT events
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = wndproc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = "rawkbd_wndclass";

    // register class
    if (!RegisterClassExA(&wc))
        return -1;

    // create window
    HWND rawkbd_wnd = CreateWindowExA(
        0, wc.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
    if (!rawkbd_wnd)
        return -2;

    // setup raw input device sink
    RAWINPUTDEVICE devs = {0x01 /* generic */, 0x06 /* keyboard */, RIDEV_INPUTSINK, rawkbd_wnd};
    if (RegisterRawInputDevices(&devs, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
        return -3;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // cleanup
    DestroyWindow(rawkbd_wnd);
    UnregisterClass(wc.lpszClassName, GetModuleHandle(NULL));
    return 0;
}
