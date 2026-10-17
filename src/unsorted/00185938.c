/* UNIT 0x00185938 0x001859B8 */
struct Names { char pad[8]; char entries[100][32]; int count; };
extern int strcmp(const char *, const char *);
int func_00185938(struct Names *names, const char *name) {
    int i;
    for (i = 0; i < names->count; i++) {
        if (strcmp(names->entries[i], name) == 0) return 1;
    }
    return 0;
}
