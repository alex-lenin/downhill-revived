/* UNIT 0x00186818 0x00186898 */
struct Names { char pad[8]; char entries[100][32]; int count; };
extern int strcmp(const char *, const char *);
int func_00186818(struct Names *names, const char *name) {
    int i;
    for (i = 0; i < names->count; i++) {
        if (strcmp(names->entries[i], name) == 0) return 1;
    }
    return 0;
}
