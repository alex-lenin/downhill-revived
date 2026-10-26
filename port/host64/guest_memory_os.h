/* OS backend for guest_memory.c (shared policy). Each function is small and
   policy-free: which regions are RAM mirrors and what stays committed is
   decided by the shared file. Backends: guest_memory_win32.c (Windows 10
   1803+), guest_memory_posix.c (Linux and other POSIX hosts). */
#ifndef DHD_HOST64_GUEST_MEMORY_OS_H
#define DHD_HOST64_GUEST_MEMORY_OS_H
#include <stdint.h>

/* Reserves 8 GiB of inaccessible address space and readies the 32 MiB RAM
   backing store. Returns the base, or 0 on failure. */
uintptr_t os_reserve(void);
/* Prepares [at, at + size) for mapping (Windows placeholders; no-op on POSIX).
   Returns nonzero on success. */
int os_split(uintptr_t at, uint64_t size);
/* Aliases the RAM backing store at [file_offset, file_offset + size). */
int os_map_ram(uintptr_t at, uint64_t size, uint64_t file_offset);
/* Marks [at, at + size) as a private inaccessible reservation. */
int os_private(uintptr_t at, uint64_t size);
/* Commits [at, at + size) read/write, preserving existing contents. */
int os_commit(uintptr_t at, uint64_t size);
/* Returns [at, at + size) to the reserved state (contents lost). */
void os_decommit(const void *at, uint64_t size);
/* Removes one mapping installed by os_map_ram / os_private. */
int os_unmap_view(void *at, uint64_t size);
int os_release(void *at, uint64_t size);
/* Closes the RAM backing store. */
void os_backing_close(void);
/* Installs the report-and-exit fault reporter; nonzero on success. */
int os_fault_install(void);
void os_fault_remove(void);
/* Logs what (with the OS error) and aborts. */
void os_fail(const char *what);
#endif
