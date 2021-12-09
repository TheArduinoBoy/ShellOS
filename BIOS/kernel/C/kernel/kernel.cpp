#include "graphics/print.h"
#include "IDT/IDT.h"

extern "C" void _start() {
    InitializeIDT();
    printString("Welcome to ShellOS!\r\nuser@ShellOS -> ");
}