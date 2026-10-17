/* The IOP's other RPC servers (sif.c): module loading, the IOP heap, file I/O and the game's
   sound driver (finding 020); the controllers are in pad.c, the memory card in mc.c. Those not
   written yet log their calls and reply with zeros (rpc_unknown). */
#include "hle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* IOP memory, for what the game puts there (sound data). Addresses are IOP addresses. */
#define IOP_RAM_SIZE 0x200000
#define IOP_HEAP_START 0x100000
static uint8_t *iop_ram;
static uint32_t iop_heap_top = IOP_HEAP_START;

void *iop_ptr(uint32_t addr)
{
    if (!iop_ram)
        iop_ram = calloc(1, IOP_RAM_SIZE);
    return iop_ram + (addr & (IOP_RAM_SIZE - 1));
}

/* loadfile: function 0 loads an IOP module (path, then its arguments); the reply's first word is
   the module id. The modules are native, so loading only checks the name. */
void rpc_loadfile(int fno, void *send, int ssize, void *recv, int rsize)
{
    if (recv && rsize > 0)
        memset(recv, 0, (size_t)rsize);
    if (fno == 0) {
        hle_log("IOP module %s\n", (const char *)send);
        if (recv && rsize >= 4)
            *(int32_t *)recv = 1;
        return;
    }
    rpc_unknown("loadfile", fno, send, ssize, recv, rsize);
}

/* The IOP heap: 1 allocates (size), 2 frees (address); bump allocation. */
void rpc_iop_heap(int fno, void *send, int ssize, void *recv, int rsize)
{
    uint32_t arg = ssize >= 4 ? *(uint32_t *)send : 0;
    uint32_t result = 0;
    if (fno == 1) {
        uint32_t size = (arg + 63) & ~63u;
        if (iop_heap_top + size <= IOP_RAM_SIZE) {
            result = iop_heap_top;
            iop_heap_top += size;
        }
        if (hle_verbose)
            hle_log("IOP heap: %u bytes at %06X\n", arg, result);
    } else if (fno != 2) {
        rpc_unknown("IOP heap", fno, send, ssize, recv, rsize);
        return;
    }
    if (recv && rsize >= 4)
        *(uint32_t *)recv = result;
}

/* File I/O. The SDK's file library (sceOpen ...) sends each call to the IOP's fileio server;
   here those functions are native. "cdrom0:" files are read from the disc image; writes to the
   standard output and error (the game's printf) go to stderr. */
#define MAX_FILES 32
enum { SEEK_SET_ = 0, SEEK_CUR_ = 1, SEEK_END_ = 2 };

static struct {
    int open;
    uint32_t lsn, size, pos;
} files[MAX_FILES];

int sceOpen(const char *name, int flags) RETAIL(func_0025DA60);
int sceOpen(const char *name, int flags)
{
    kernel_poll();
    const char *path = strchr(name, ':');
    if (!path || _strnicmp(name, "cdrom", 5) != 0) {
        hle_log("sceOpen(%s, %x): only cdrom0: files\n", name, flags);
        return -1;
    }
    int fd;
    for (fd = 3; fd < MAX_FILES && files[fd].open; fd++)
        ;
    if (fd == MAX_FILES || !cdvd_find(path + 1, &files[fd].lsn, &files[fd].size)) {
        hle_log("sceOpen(%s): %s\n", name, fd == MAX_FILES ? "too many files" : "not on the disc");        return -1;
    }
    files[fd].open = 1;
    files[fd].pos = 0;
    if (hle_verbose)
        hle_log("sceOpen(%s) = %d (%u bytes)\n", name, fd, files[fd].size);
    return fd;
}

static int valid(int fd) { return fd >= 3 && fd < MAX_FILES && files[fd].open; }

int sceClose(int fd) RETAIL(func_0025DCE8);
int sceClose(int fd)
{
    if (!valid(fd))
        return -1;
    files[fd].open = 0;
    return 0;
}

int sceLseek(int fd, int offset, int whence) RETAIL(func_0025DE68);
int sceLseek(int fd, int offset, int whence)
{
    if (!valid(fd))
        return -1;
    int64_t pos = whence == SEEK_SET_ ? 0 : whence == SEEK_CUR_ ? files[fd].pos : files[fd].size;
    pos += offset;
    if (pos < 0)
        return -1;
    files[fd].pos = (uint32_t)pos;
    return (int)pos;
}

int sceRead(int fd, void *buf, int count) RETAIL(func_0025E0A0);
int sceRead(int fd, void *buf, int count)
{
    kernel_poll();
    if (!valid(fd) || count < 0)
        return -1;
    uint32_t pos = files[fd].pos, end = files[fd].size;
    uint32_t n = pos >= end ? 0 : end - pos < (uint32_t)count ? end - pos : (uint32_t)count;
    uint8_t sec[2048];
    for (uint32_t done = 0; done < n;) {  /* sector by sector: reads need not be aligned */
        uint32_t at = pos + done, skip = at % 2048, chunk = 2048 - skip;
        if (chunk > n - done)
            chunk = n - done;
        if (!cdvd_read(files[fd].lsn + at / 2048, 1, sec))
            return -1;
        memcpy((uint8_t *)buf + done, sec + skip, chunk);
        done += chunk;
    }
    files[fd].pos += n;
    return (int)n;
}

int sceWrite(int fd, const void *buf, int count) RETAIL(func_0025E300);
int sceWrite(int fd, const void *buf, int count)
{
    if (fd == 1 || fd == 2) {
        fwrite(buf, 1, (size_t)count, stderr);
        return count;
    }
    hle_log("sceWrite(%d): files are read-only\n", fd);
    return -1;
}

void rpc_fileio(int fno, void *send, int ssize, void *recv, int rsize) { rpc_unknown("fileio", fno, send, ssize, recv, rsize); }
void rpc_mtap(int fno, void *send, int ssize, void *recv, int rsize) { rpc_unknown("multitap", fno, send, ssize, recv, rsize); }
/* Multitap (finding 040; ps2sdk's libmtap: function 1, the port in send word 0, the result in
   reply word 1). With 3-4 pads port 0 has one, so the game maps player i to port 0 slot i. */
static void mtap_reply(void *send, int ssize, void *recv, int rsize, int word, uint32_t value)
{
    if (recv && rsize > 0) memset(recv, 0, (size_t)rsize);
    if (rsize >= 4 * (word + 1)) ((uint32_t *)recv)[word] = value;
    (void)send, (void)ssize;
}
static int mtap_on(void *send, int ssize) { return pads_multitap() && ssize >= 4 && *(int32_t *)send == 0; }
void rpc_mtap_open(int fno, void *send, int ssize, void *recv, int rsize)
{
    (void)fno;
    mtap_reply(send, ssize, recv, rsize, 1, mtap_on(send, ssize));
}
void rpc_mtap_connection(int fno, void *send, int ssize, void *recv, int rsize)
{
    (void)fno;
    mtap_reply(send, ssize, recv, rsize, 1, mtap_on(send, ssize));
}
void rpc_mtap_version(int fno, void *send, int ssize, void *recv, int rsize)
{
    (void)fno;
    mtap_reply(send, ssize, recv, rsize, 0, 0x0300);  /* mtapman 3.0: the version the library checks */
}
/* The game's IOP driver is 989snd (finding 022): its two RPC servers, the
   batched command queue and the bank loader, live in sound.c. The read's
   status word D_004CD380 is written there (func_00202820 waits for word 0 to
   clear; word 4 is the error, func_00202910). */
#include "sound.h"

void rpc_sound(int fno, void *send, int ssize, void *recv, int rsize)
{
    sound_rpc(fno, send, ssize, recv, rsize);
}
void rpc_sound2(int fno, void *send, int ssize, void *recv, int rsize)
{
    sound_rpc2(fno, send, ssize, recv, rsize);
}
