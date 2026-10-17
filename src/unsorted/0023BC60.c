/* UNIT 0x0023BC60 0x0023BCB8 */
typedef struct Stream {
    char pad0[0x14];
    int pos;
    char pad18[0x40 - 0x18];
    int sema;
} Stream;

extern int func_0025A720(int);
extern int func_0025A700(int);

void func_0023BC60(Stream *s)
{
    func_0025A720(s->sema);
    s->pos = (s->pos + 0x7FF) / 0x800 * 0x800;
    func_0025A700(s->sema);
}
