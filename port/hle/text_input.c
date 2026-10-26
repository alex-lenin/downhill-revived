#include "text_input.h"
#include "hle.h"
#ifdef _WIN32
#include <windows.h>  /* SRWLOCK */
#else
#include "../host/win32_compat.h"
#endif
#if defined(_WIN32) || defined(DHD_HAVE_SDL)
#include <SDL3/SDL.h>  /* the clipboard */
#endif
#include <string.h>

/* The arrow keys, by their Windows virtual-key codes (keyboard.c's codes). */
enum { TEXT_LEFT = 0x25, TEXT_UP = 0x26, TEXT_RIGHT = 0x27, TEXT_DOWN = 0x28 };

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
            if (vk != TEXT_LEFT && vk != TEXT_RIGHT && vk != TEXT_UP && vk != TEXT_DOWN &&
                keyboard_is_down(vk)) blocked[vk] = 1;
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
    int typing = vk != TEXT_LEFT && vk != TEXT_RIGHT && vk != TEXT_UP && vk != TEXT_DOWN;
    if (!keyboard_is_down(vk)) blocked[vk] = 0;
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
        } else if (c == 22) { /* Ctrl+V: the clipboard's text, the field's allowed characters only */
#if defined(_WIN32) || defined(DHD_HAVE_SDL)
            char *text = SDL_GetClipboardText();  /* UTF-8: bytes of 128 and more are not allowed */
            if (text) {
                for (size_t k = 0; text[k] && k < 4096; k++)
                    append(buffer, capacity, allowed, (unsigned char)text[k]);
                SDL_free(text);
            }
#else
            /* No clipboard on this host; the paste is dropped. */
#endif
        } else append(buffer, capacity, allowed, c);
    }
    return 0;
}

/* Guest-address-only module services. */
#include "../host/host_api.h"
#include "../include/guest.h"
void host_service_text_input_begin(void) { text_input_begin(); }
void host_service_text_input_end(void) { text_input_end(); }
int host_service_text_input_edit(uint32_t a0, int a1, uint32_t a2) { return text_input_edit((char *)GUEST(a0), a1, (const char *)GUEST(a2)); }
