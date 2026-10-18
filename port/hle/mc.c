/* The memory card: the IOP's mcserv RPC server (0x80000400), answering the game's own libmc
   (recompiled MIPS, 0x00245E78-0x00246B80) from a folder per card beside dhd.exe:
   saves\card1 is the card in slot 1 (port 0), saves\card2, if that folder exists, the card in
   slot 2. Each card file is a host file of the same name: saves\card1\BASCUS-97177DHD\... .

   The library's calls and their send/receive layouts, read from its asm (Sony libmc 2.x, the
   XMCSERV function numbers of ps2sdk's libmc.c; finding 050):
     0xFE init         reply [0] result, [1] mcserv version (>= 0x20A), [2] mcman version (>= 0x20E)
     0x01 get info     +4 port, +8 slot, +0x1C result block: [0] type, [1] free, [0x24] format
     0x02 open         +0 port, +4 slot, +8 flags, +0x14 name (flags 0x40: make a directory)
     0x03 close        +0 fd
     0x05 read         +0 fd, +0xC size, +0x18 buffer, +0x1C alignment block (sizes at [0], [1])
     0x06 write        +0 fd, +0xC size from +0x18 buffer, after the first +0x14 bytes at +0x20
     0x0D get dir      +0 port, +4 slot, +8 continue, +0xC most entries, +0x10 table, +0x14 name
     0x10 format       +4 port, +8 slot
     0x15 slot max     +4 port
   The reply's word 0 is the result, which sceMcSync hands to the game. Pointers in the send
   block are the game's (host) pointers. Errors are mcserv's: -4 no such entry, -2 unformatted,
   -10 no card. DHD_MEMCARD=0 (dhd.ini memory_card = 0) leaves the server unwritten, as before:
   the game then sees no card (the race tools run that way). */
#include "hle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

enum { MC_NOENTRY = -4, MC_NOCARD = -10, MC_TOTAL_KB = 8000 };
enum { O_READ = 1, O_WRITE = 2, O_CREATE = 0x200, O_TRUNCATE = 0x400, O_DIRECTORY = 0x40 };

/* Online with everything unlocked (online_menu.c): the game saves as usual, but nothing reaches
   the card, which keeps the player's own progress. */
int hle_mc_hold;

static int mc_enabled(void)
{
    static int on = -1;
    if (on < 0) {
        const char *v = getenv("DHD_MEMCARD");
        on = !v || atoi(v) != 0;
    }
    return on;
}

/* saves\cardN beside dhd.exe (DHD_MEMCARD_DIR overrides saves). */
static int card_root(int port, char *out, size_t size)
{
    const char *named = getenv("DHD_MEMCARD_DIR");
    char base[MAX_PATH];
    if (named) snprintf(base, sizeof base, "%s", named);
    else {
        DWORD n = GetModuleFileNameA(NULL, base, sizeof base);
        while (n && base[n - 1] != '\\' && base[n - 1] != '/') n--;
        snprintf(base + n, sizeof base - n, "saves");
    }
    return snprintf(out, size, "%s\\card%d", base, port + 1) < (int)size;
}

static int is_dir(const char *path)
{
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}

/* Slot 1's card is always in; slot 2's only once its folder exists. */
static int card_present(int port, int slot)
{
    char root[MAX_PATH];
    if (slot != 0 || port < 0 || port > 1 || !card_root(port, root, sizeof root)) return 0;
    if (port == 0) return 1;
    return is_dir(root);
}

static void make_root(int port)
{
    char root[MAX_PATH];
    if (!card_root(port, root, sizeof root)) return;
    char *sep = strrchr(root, '\\');
    if (sep) { *sep = 0; CreateDirectoryA(root, NULL); *sep = '\\'; }
    CreateDirectoryA(root, NULL);
}

/* A card path ("/DIR/FILE" or "DIR/FILE"; the game never changes directory) as a host path.
   "." and ".." are refused, as are names a FAT/NTFS folder can't hold. */
static int host_path(int port, const char *name, char *out, size_t size)
{
    if (!card_root(port, out, size)) return 0;
    size_t n = strlen(out);
    for (const char *p = name; *p;) {
        while (*p == '/' || *p == '\\') p++;
        if (!*p) break;
        const char *end = p;
        while (*end && *end != '/' && *end != '\\') end++;
        size_t len = (size_t)(end - p);
        if ((len == 1 && p[0] == '.') || (len == 2 && p[0] == '.' && p[1] == '.') || len > 31) return 0;
        for (const char *c = p; c < end; c++)
            if (strchr(":*?\"<>|", *c) || (unsigned char)*c < 32) return 0;
        if (n + 1 + len + 1 > size) return 0;
        out[n++] = '\\';
        memcpy(out + n, p, len);
        n += len;
        out[n] = 0;
        p = end;
    }
    return 1;
}

/* Free space in KB: 8,000 for an empty card, less one KB per directory entry and each file's
   size rounded up to 1 KB clusters (what the PS2 browser would roughly show). */
static int used_kb(const char *dir)
{
    char pattern[MAX_PATH];
    WIN32_FIND_DATAA fd;
    int used = 0;
    snprintf(pattern, sizeof pattern, "%s\\*", dir);
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, "..")) continue;
        used += 1;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            char sub[MAX_PATH];
            snprintf(sub, sizeof sub, "%s\\%s", dir, fd.cFileName);
            used += used_kb(sub);
        } else {
            used += (int)((fd.nFileSizeLow + 1023) / 1024);
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return used;
}

/* Open files. */
#define MC_FILES 16
static FILE *files[MC_FILES];

static int mc_open(int port, int slot, int flags, const char *name)
{
    char path[MAX_PATH];
    if (!card_present(port, slot)) return MC_NOCARD;
    if (!host_path(port, name, path, sizeof path)) return MC_NOENTRY;
    make_root(port);
    if (flags & O_DIRECTORY) {
        if (is_dir(path) || GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES) return MC_NOENTRY;
        if (hle_mc_hold) return 0;
        return CreateDirectoryA(path, NULL) ? 0 : MC_NOENTRY;
    }
    if (is_dir(path)) return MC_NOENTRY;
    int exists = GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
    if (!exists && !(flags & O_CREATE)) return MC_NOENTRY;
    const char *mode = !(flags & O_WRITE) ? "rb" : (!exists || (flags & O_TRUNCATE)) ? "w+b" : "r+b";
    int fd;
    for (fd = 0; fd < MC_FILES && files[fd]; fd++)
        ;
    if (fd == MC_FILES) return -7;  /* sceMcResUpLimitHandle */
    files[fd] = fopen(hle_mc_hold && (flags & O_WRITE) ? "NUL" : path, mode);  /* held: the write goes nowhere */
    if (!files[fd]) return MC_NOENTRY;
    if (hle_verbose) hle_log("memory card: open %s (%x) = %d\n", path, flags, fd);
    return fd;
}

static FILE *file(int fd) { return fd >= 0 && fd < MC_FILES ? files[fd] : NULL; }

/* Matches a card name pattern: '*' any run, '?' one character, letters case-sensitive. */
static int match(const char *pattern, const char *name)
{
    if (!*pattern) return !*name;
    if (*pattern == '*') return match(pattern + 1, name) || (*name && match(pattern, name + 1));
    return *name && (*pattern == '?' || *pattern == *name) && match(pattern + 1, name + 1);
}

typedef struct {               /* sceMcTblGetDir, 64 bytes */
    uint8_t created[8], modified[8];  /* sceMcStDateTime: reserved, sec, min, hour, day, month, year */
    uint32_t size;             /* bytes; for a directory, its entries (with . and ..) */
    uint16_t attributes, reserved1;
    uint32_t reserved2, pda;
    char name[32];
} McEntry;

static void stamp(uint8_t out[8], FILETIME ft)
{
    SYSTEMTIME st;
    FILETIME local;
    FileTimeToLocalFileTime(&ft, &local);
    FileTimeToSystemTime(&local, &st);
    out[0] = 0;
    out[1] = (uint8_t)st.wSecond, out[2] = (uint8_t)st.wMinute, out[3] = (uint8_t)st.wHour;
    out[4] = (uint8_t)st.wDay, out[5] = (uint8_t)st.wMonth;
    out[6] = (uint8_t)st.wYear, out[7] = (uint8_t)(st.wYear >> 8);
}

static int count_entries(const char *dir)
{
    char pattern[MAX_PATH];
    WIN32_FIND_DATAA fd;
    int n = 0;
    snprintf(pattern, sizeof pattern, "%s\\*", dir);
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do n++; while (FindNextFileA(h, &fd));  /* . and .. included, as on the card */
    FindClose(h);
    return n;
}

static void fill_entry(McEntry *e, const char *dir, const WIN32_FIND_DATAA *fd)
{
    memset(e, 0, sizeof *e);
    stamp(e->created, fd->ftCreationTime);
    stamp(e->modified, fd->ftLastWriteTime);
    if (fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        char sub[MAX_PATH];
        snprintf(sub, sizeof sub, "%s\\%s", dir, fd->cFileName);
        e->attributes = 0x8427;  /* exists, subdirectory, read/write/execute */
        e->size = !strcmp(fd->cFileName, ".") || !strcmp(fd->cFileName, "..") ? 0 : (uint32_t)count_entries(sub);
    } else {
        e->attributes = 0x8497;  /* exists, closed, file, read/write/execute */
        e->size = fd->nFileSizeLow;
    }
    snprintf(e->name, sizeof e->name, "%s", fd->cFileName);
}

/* get dir: the entries of the name's directory that match its last part. A continued call
   (mode 1) goes on after the entries already returned. */
static int mc_getdir(int port, int slot, const char *name, int more, int most, McEntry *table)
{
    static int returned;
    char dir[MAX_PATH], pattern[64], parent[1024];
    if (!card_present(port, slot)) return MC_NOCARD;
    const char *last = strrchr(name, '/');
    snprintf(pattern, sizeof pattern, "%s", last ? last + 1 : name);
    snprintf(parent, sizeof parent, "%.*s", last ? (int)(last - name) : 0, name);
    if (!host_path(port, parent, dir, sizeof dir)) return MC_NOENTRY;
    if (!more) returned = 0;
    char search[MAX_PATH];
    WIN32_FIND_DATAA fd;
    snprintf(search, sizeof search, "%s\\*", dir);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    int at_root = !parent[0] || !strcmp(parent, "/");
    int seen = 0, n = 0;
    do {
        if (at_root && (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))) continue;
        if (!match(pattern, fd.cFileName)) continue;
        if (seen++ < returned) continue;
        if (n >= most) break;
        if (table) fill_entry(&table[n], dir, &fd);
        n++;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    returned += n;
    if (hle_verbose) hle_log("memory card: get dir %s = %d\n", name, n);
    return n;
}

static uint32_t word(const void *send, int ssize, int offset)
{
    return offset + 4 <= ssize ? *(const uint32_t *)((const char *)send + offset) : 0;
}

static void *pointer(const void *send, int ssize, int offset)
{
    return (void *)(uintptr_t)word(send, ssize, offset);
}

void rpc_mc(int fno, void *send, int ssize, void *recv, int rsize)
{
    if (!mc_enabled()) {
        rpc_unknown("memory card", fno, send, ssize, recv, rsize);
        return;
    }
    static int known[2];  /* the card was seen: get info then says 0, the first time -1 (new card) */
    int32_t reply[3] = {0, 0, 0};
    const char *sendc = send;
    switch (fno) {
    case 0xFE:
        reply[1] = 0x20A, reply[2] = 0x20E;
        hle_log("memory card: %s\n", card_present(0, 0) ? "slot 1 is the folder saves\\card1" : "none");
        break;
    case 0x01: {
        int port = (int)word(send, ssize, 4), slot = (int)word(send, ssize, 8);
        uint32_t *info = pointer(send, ssize, 0x1C);
        int present = card_present(port, slot);
        if (info) {
            char root[MAX_PATH];
            int used = present && card_root(port, root, sizeof root) ? used_kb(root) : 0;
            info[0] = present ? 2 : 0;  /* sceMcTypePS2 */
            info[1] = present ? (uint32_t)(used < MC_TOTAL_KB ? MC_TOTAL_KB - used : 0) : 0;
            info[0x24] = present;       /* formatted */
        }
        if (!present) reply[0] = MC_NOCARD;
        else if (port >= 0 && port < 2 && !known[port]) reply[0] = -1, known[port] = 1;
        break;
    }
    case 0x02:
        reply[0] = ssize >= 0x15 ? mc_open((int)word(send, ssize, 0), (int)word(send, ssize, 4),
                                           (int)word(send, ssize, 8), sendc + 0x14) : MC_NOENTRY;
        break;
    case 0x03: {
        int fd = (int)word(send, ssize, 0);
        if (file(fd)) { fclose(files[fd]); files[fd] = NULL; }
        else reply[0] = MC_NOENTRY;
        break;
    }
    case 0x05: {
        FILE *f = file((int)word(send, ssize, 0));
        int size = (int)word(send, ssize, 0xC);
        uint8_t *buffer = pointer(send, ssize, 0x18);
        uint32_t *align = pointer(send, ssize, 0x1C);
        if (align) align[0] = align[1] = 0;  /* the whole read goes straight to the buffer */
        reply[0] = f && buffer && size >= 0 ? (int32_t)fread(buffer, 1, (size_t)size, f) : MC_NOENTRY;
        break;
    }
    case 0x06: {
        FILE *f = file((int)word(send, ssize, 0));
        int size = (int)word(send, ssize, 0xC), head = (int)word(send, ssize, 0x14);
        const uint8_t *buffer = pointer(send, ssize, 0x18);
        if (!f || head < 0 || head > 16 || size < 0 || (size && !buffer)) { reply[0] = MC_NOENTRY; break; }
        size_t done = fwrite(sendc + 0x20, 1, (size_t)head, f);
        if (size) done += fwrite(buffer, 1, (size_t)size, f);
        fflush(f);
        reply[0] = (int32_t)done;
        break;
    }
    case 0x0D:
        reply[0] = ssize >= 0x15 ? mc_getdir((int)word(send, ssize, 0), (int)word(send, ssize, 4), sendc + 0x14,
                                             (int)word(send, ssize, 8), (int)word(send, ssize, 0xC),
                                             pointer(send, ssize, 0x10)) : MC_NOENTRY;
        break;
    case 0x10: {
        int port = (int)word(send, ssize, 4), slot = (int)word(send, ssize, 8);
        if (card_present(port, slot)) make_root(port);
        else reply[0] = MC_NOCARD;
        break;
    }
    case 0x15:
        reply[0] = 1;  /* one slot per port (no multitap for cards) */
        break;
    default:
        rpc_unknown("memory card", fno, send, ssize, recv, rsize);
        return;
    }
    if (hle_verbose) hle_log("memory card: function %d = %d\n", fno, reply[0]);
    if (recv && rsize > 0) {
        memset(recv, 0, (size_t)rsize);
        memcpy(recv, reply, rsize < (int)sizeof reply ? (size_t)rsize : sizeof reply);
    }
}
