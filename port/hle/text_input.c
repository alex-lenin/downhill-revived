#include "text_input.h"
#include <windows.h>
#include <string.h>

static SRWLOCK lock = SRWLOCK_INIT;
static int active, count;
static unsigned characters[256];
static unsigned char blocked[256]; /* held typing keys must be released before pad use */

void text_input_begin(void)
{
    AcquireSRWLockExclusive(&lock);
    active = 1;
    count = 0;
    ReleaseSRWLockExclusive(&lock);
}
void text_input_end(void)
{
    if (text_input_active())
        for (unsigned vk = 0; vk < 256; vk++)
            if (vk != VK_LEFT && vk != VK_RIGHT && vk != VK_UP && vk != VK_DOWN &&
                (GetAsyncKeyState(vk) & 0x8000)) blocked[vk] = 1;
    AcquireSRWLockExclusive(&lock);
    active = count = 0;
    ReleaseSRWLockExclusive(&lock);
}
int text_input_active(void)
{
    AcquireSRWLockShared(&lock);
    int result = active;
    ReleaseSRWLockShared(&lock);
    return result;
}
void text_input_clear(void)
{
    AcquireSRWLockExclusive(&lock);
    count = 0;
    ReleaseSRWLockExclusive(&lock);
}
void text_input_character(unsigned c)
{
    AcquireSRWLockExclusive(&lock);
    if (active && count < 256) characters[count++] = c;
    ReleaseSRWLockExclusive(&lock);
}
int text_input_key_allowed(unsigned vk)
{
    if (vk >= 256) return 0;
    /* Arrows still operate the original pad picker. All printable keys belong to the field. */
    int typing = vk != VK_LEFT && vk != VK_RIGHT && vk != VK_UP && vk != VK_DOWN;
    if (!(GetAsyncKeyState(vk) & 0x8000)) blocked[vk] = 0;
    else if (typing && text_input_active()) blocked[vk] = 1;
    return !blocked[vk] && !(typing && text_input_active());
}
static void append(char *buffer, int capacity, const char *allowed, unsigned c)
{
    if (c >= 'a' && c <= 'z') c -= 'a' - 'A';
    size_t n = strlen(buffer);
    if (c && c < 128 && strchr(allowed, (int)c) && n + 1 < (size_t)capacity)
        buffer[n] = (char)c, buffer[n + 1] = 0;
}
int text_input_edit(char *buffer, int capacity, const char *allowed)
{
    unsigned input[256];
    AcquireSRWLockExclusive(&lock);
    int n = count;
    memcpy(input, characters, n * sizeof *input);
    count = 0;
    ReleaseSRWLockExclusive(&lock);
    for (int i = 0; i < n; i++) {
        unsigned c = input[i];
        if (c == 13 || c == 10) return 1;
        if (c == 27) return -1;
        if (c == 8) {
            size_t length = strlen(buffer);
            if (length) buffer[length - 1] = 0;
        } else if (c == 22) { /* Ctrl+V; bounded Unicode clipboard, same field validation */
            if (OpenClipboard(NULL)) {
                HANDLE data = GetClipboardData(CF_UNICODETEXT);
                const WCHAR *s = data ? GlobalLock(data) : NULL;
                SIZE_T limit = data ? GlobalSize(data) / sizeof(WCHAR) : 0;
                if (s) {
                    for (SIZE_T k = 0; k < limit && k < 4096 && s[k]; k++)
                        append(buffer, capacity, allowed, s[k]);
                    GlobalUnlock(data);
                }
                CloseClipboard();
            }
        } else append(buffer, capacity, allowed, c);
    }
    return 0;
}
