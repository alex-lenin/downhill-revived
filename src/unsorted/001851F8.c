/* UNIT 0x001851F8 0x00185278 */
struct Names { char pad[8]; char entries[100][32]; int count; };
extern int strcmp(const char *, const char *);
int func_001851F8(struct Names *names, const char *name) {
    int i;
    for (i = 0; i < names->count; i++) {
        if (strcmp(name, names->entries[i]) == 0) return i;
    }
    return -1;
}
