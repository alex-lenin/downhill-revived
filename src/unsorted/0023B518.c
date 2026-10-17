/* UNIT 0x0023B518 0x0023B570 */
typedef struct Stream {
    char pad0[0x14];
    int pos;
    char pad18[0x40 - 0x18];
    int sema;
    char pad44[0x48 - 0x44];
    long total;
} Stream;

extern int func_0025A720(int);
extern int func_0025A700(int);

void func_0023B518(Stream *s, int n)
{
    func_0025A720(s->sema);
    s->pos += n;
    s->total += n;
    func_0025A700(s->sema);
}
