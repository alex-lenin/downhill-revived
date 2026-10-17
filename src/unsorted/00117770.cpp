/* UNIT 0x00117770 0x001177C8 */
struct Vec {
    float x, y, z;

    void Set(float a, float b, float c)
    {
        x = a;
        y = b;
        z = c;
    }
};

struct Obj {
    int x0;
    int x4;
    float x8;
    char padC[0xA];
    unsigned char x16;
    char pad17;
    short x18;
    char pad1A[6];
    Vec x20;
    char pad2C[0xA];
    unsigned char x36;
    char pad37;
    short x38;
    char pad3A[0x1004C - 0x3A];
    int x1004C;
    int x10050;
    int x10054;
    int x10058;

    void func_00117770();
};

void Obj::func_00117770()
{
    x1004C = 2;
    x10058 = 1;
    x0 = 0;
    x4 = 0;
    x8 = 100.0f;
    x16 = 1;
    x18 = 1;
    x20.Set(100.0f, 100.0f, 100.0f);
    x36 = 1;
    x38 = 0;
}
