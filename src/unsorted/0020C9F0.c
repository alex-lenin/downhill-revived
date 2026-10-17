/* UNIT 0x0020C9F0 0x0020CC40 .sdata=0x29DA10:0x29DA18 */
typedef struct {
    unsigned short x, y;
    int unused;
} Point;

typedef struct {
    char pad0[0x6C];
    int x6c;
    char pad70[0x5B];
    char xcb;
    char padcc[0x24];
    Point points[51];
    char pad288[8];
    float x290;
    char pad294[12];
    int x2a0, x2a4, x2a8, x2ac;
    char pad2b0[12];
    int x2bc;
} Obj;

long D_0029DA10 = 0x8163BD970AF4C5E2UL;
extern void func_0023D650(int, int *, int *);
extern void func_0023D618(int, int *, int *);
extern void func_0020C530(Obj *);

void func_0020C9F0(Obj *obj, int handle)
{
    struct {
        float sincos[2];
        char pad8[8];
        int width, height, left, top;
    } locals;

    func_0023D650(handle, &locals.width, &locals.height);
    func_0023D618(handle, &locals.left, &locals.top);
    if (locals.width != obj->x2a0 || locals.height != obj->x2a4 ||
        locals.left != obj->x2a8 || locals.top != obj->x2ac) {
        obj->x2a0 = locals.width;
        obj->x2a4 = locals.height;
        obj->x2a8 = locals.left;
        obj->x2ac = locals.top;
        func_0020C530(obj);
    } else {
        int value = (int)obj->x290;
        obj->x6c = value;
        obj->xcb = value;
        if (obj->x2bc) {
            Point *first = obj->points;
            Point *out;
            int count;
            float sine, cosine, x, y;

            /* VU0 microroutine returns sine and cosine in vf14 and vf15. */
            __asm__ volatile (
                "mfc1 $2,%2\n\t"
                "qmtc2 $2,$vf14\n\t"
                "vcallms 0x978\n\t"
                "nop\n\t"
                "qmfc2.i $2,$vf14\n\t"
                "mtc1 $2,%0\n\t"
                "qmfc2 $2,$vf15\n\t"
                "mtc1 $2,%1"
                : "=&f"(locals.sincos[0]), "=&f"(locals.sincos[1])
                : "f"(0.1256637126f) : "$2");

            x = (float)((locals.width >> 1) < locals.height ?
                        (locals.width >> 1) : locals.height);
            y = 0.0f;
            count = 49;
            out = first;
            do {
                long seed;
                union { int i; float f; } random;
                float rx, ry, newY;
                cosine = locals.sincos[1];
                sine = locals.sincos[0];
                count--;
                seed = D_0029DA10 * 0x41C64E6DL + 0x3039;
                random.i = (seed & 0x7FFFFF) | 0x3F800000;
                D_0029DA10 = seed;
                random.f -= 0.75f;
                rx = y * random.f;
                ry = x * random.f;
                newY = y * cosine + x * sine;
                x = x * cosine - y * sine;
                y = newY;
                out->x = (obj->x2a8 + (int)rx) << 4;
                out->y = (obj->x2ac + (int)(ry * 0.5f)) << 4;
                out++;
            } while (count >= 0);
            first[50].x = first[0].x;
            first[50].y = first[0].y;
        }
    }
}
