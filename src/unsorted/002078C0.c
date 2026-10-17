/* UNIT 0x002078C0 0x002079B0 */
struct Heap {
    int x0;
    int x4;
    int x8;
};

extern char D_0029D9A8[];
extern int func_0020D8F8(char *dst, int arg1, int arg0, char *tag, int arg4);

int func_002078C0(int arg0, int arg1, char **out, struct Heap *heap)
{
    int prev;
    int *link;
    char *buf;
    int len;
    int last;
    int align;

    align = 16;
    prev = heap->x8;
    link = (int *)((heap->x4 + 3) & ~3);
    heap->x8 = (int)link;
    heap->x4 = (int)(link + 1);
    *link = prev;

    buf = (char *)((heap->x4 + 15) & ~15);
    heap->x4 = (int)(buf + 1);
    len = func_0020D8F8(buf, arg1, arg0, D_0029D9A8, 1);
    last = len - 1;
    align = align - len % 16;
    heap->x4 = ((heap->x4 + 0xFF) & ~0xFF) + last;
    *out = buf;
    return len + align % 16;
}
