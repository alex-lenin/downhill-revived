/* UNIT 0x001B5D78 0x001B5DC8 */
#include "ui_style.h"
extern char D_004B1F50[];
void func_001B5D78(int enabled, int value) {
    if (value == 65535 && enabled) {
        InitializeUiStyleDiscard((UiStyle *)(void *)(D_004B1F50), 349, 194, -112, -32, 83, 83, 83, 255);
    }
}
