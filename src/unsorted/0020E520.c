/* UNIT 0x0020E520 0x0020E7B0 */
typedef struct Toc {
    int sector;
    char pad4[2];
    unsigned short count;
    int size;
    unsigned char flags;
    char name[19];
    struct Toc *items[1];
} Toc;
typedef struct { int sector; int size; char name[16]; char pad[8]; } Out;
extern Toc D_004DA280;
extern char D_002981C8[];
extern int strcmp(const char *a, const char *b);
extern void func_0020D028(char *fmt, ...);
int func_0020E520(Out *out, char *path)
{
    char tmp[32];
    Toc *table;
    char *dst;
    char *rest;
    int found;
    int i;
    int j;
    char *p;
    char *q;
    char c;
    dst = tmp;
    table = &D_004DA280;
    rest = tmp + 16;
    found = 0;
    if (path[0] != '\\')
        return 0;
    p = path + 1;
    if (*p != '\\' && *p != 0) {
        do {
            *dst = *p;
            dst++;
            p++;
        } while (*p != '\\' && *p != 0);
    }
    c = *p;
    *dst = 0;
    p++;
    if (c == '\\')
        rest = p;
    dst = tmp;
    for (i = 0; i < table->count; i++) {
        if (strcmp(table->items[i]->name, dst) == 0) {
            if (!(table->items[i]->flags & 2)) {
                out->sector = table->items[i]->sector;
                out->size = table->items[i]->size;
                q = out->pad;
                for (j = 0; j < 8; j++) {
                    q[j] = 0;
                }
                for (j = 0; j < 16; j++)
                    out->name[j] = table->items[i]->name[j];
                return 1;
            }
            table = table->items[i];
            found = 1;
        }
    }
    if (found) {
        for (i = 0; i < table->count; i++) {
            if (strcmp(table->items[i]->name, rest) == 0) {
                out->sector = table->items[i]->sector;
                out->size = table->items[i]->size;
                q = out->pad;
                for (j = 0; j < 8; j++) {
                    q[j] = 0;
                }
                for (j = 0; j < 16; j++)
                    out->name[j] = table->items[i]->name[j];
                return 1;
            }
        }
    }
    func_0020D028(D_002981C8, path, &D_004DA280);
    return 0;
}
