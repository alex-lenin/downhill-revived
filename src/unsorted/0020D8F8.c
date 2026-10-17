/* UNIT 0x0020D8F8 0x0020D930 */
#include "disc_path.h"

extern int func_0020D930(void *buffer, char *path);

/* Reads the disc file <dir>\<name>.<ext> into buffer; returns its size. */
int func_0020D8F8(void *buffer, char *dir, char *name, char *ext) {
    char path[64];
    func_0020DC28((char *)(path), (const char *)(dir), (const char *)(name), (const char *)(ext));
    return func_0020D930(buffer, path);
}
