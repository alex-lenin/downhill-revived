/* UNIT 0x00117658 0x00117770 */
struct Item {
    char pad0[0x14];
    unsigned short flags;
};

struct SaveFile {
    int magic;
    int count;
    char items[0x10800];
};

class Obj {
public:
    char pad0[0x1004C];
    int x1004C;
    int x10050;
    int x10054;
    int x10058;
    char pad1005C[0xC];
    unsigned char x10068;

    virtual void Changed();
    void func_00117770();
    void func_00117658(char *path);
};

extern "C" int func_0020D930(SaveFile *buf, char *path);
extern "C" void func_00254D08(char *fmt, int got, int want);
extern "C" Item *func_001177F8(Obj *obj, int index);
extern "C" void *memcpy(void *dst, const void *src, unsigned int n);
extern char D_00288970[];

void Obj::func_00117658(char *path)
{
    SaveFile buf;
    int i;
    Item *item;
    int flags;

    if (x10068 == 0) {
        return;
    }
    i = func_0020D930(&buf, path);
    x10058 = 0;
    if (i > 0) {
        if (buf.magic == 0x9212001) {
            goto load;
        }
        func_00254D08(D_00288970, buf.magic, 0x9212001);
    }
    func_00117770();
    Changed();
    return;
load:
    x1004C = buf.count;
    memcpy(this, buf.items, buf.count << 5);
    for (i = 0; i < 0x800; i++) {
        item = func_001177F8(this, i);
        flags = item->flags;
        item->flags = flags & ~1;
    }
    Changed();
}
