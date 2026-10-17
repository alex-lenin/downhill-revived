/* UNIT 0x0023D8D8 0x0023DD58 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Matrix { float m[4][4]; } Matrix __attribute__((aligned(16)));
typedef struct View {
    float x0;
    unsigned short width;
    unsigned short height;
    char pad8[8];
    float x10;
    float x14;
    char pad18[0x18];
    Matrix x30;
    Matrix x70;
    float xB0;
} View;
typedef struct Entry { void *camera; View *view; int mode; char padC[8]; } Entry;

extern Entry D_0075A9D0[];
extern Matrix D_0075AA70[], D_0075ABF0[], D_0075AD70[];
extern Matrix D_0075B370;
extern float D_0029E414, D_0029E418;
extern void func_0023DD58(Vec *out, Vec *origin, Vec *a, Vec *b);
extern float func_002484F8(float);
extern void func_00222170(Matrix *, Matrix *, Matrix *, float, float, float, float, float, float, float, float, float, float, float, float);
extern void func_00221D58(Matrix *, Matrix *);
extern float func_0021E430(void);

/* 64-byte matrix copy, as libvu0's sceVu0CopyMatrix */
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
                      : "=m"((dst).m[0]), "=m"((dst).m[1]), "=m"((dst).m[2]), "=m"((dst).m[3]) \
                      : "r"(src) : "$8", "$9", "$10", "$11")

void func_0023D8D8(int idx, float dist, float arg2) {
    Vec origin = { 0.0f, 0.0f, 0.0f, 0.0f };
    Vec a = { 0.0f, 0.0f, 200.0f, 0.0f };
    Vec b;
    Matrix tmp;
    Entry *entry = &D_0075A9D0[idx];
    View *view = entry->view;
    float y;
    float angle;
    Matrix *proj;

    view->x0 = dist;
    y = dist - 15.0f;
    if (entry->mode == 10) dist = 1024.0f;

    view->x30.m[0][0] = 0.0f;
    view->x30.m[0][1] = 1.0f;
    view->x30.m[0][2] = 0.0f;
    view->x30.m[0][3] = 0.0f;

    b.x = view->width >> 1;
    b.y = y;
    b.z = 0.0f;
    func_0023DD58((Vec *)view->x30.m[1], &origin, &a, &b);

    b.x = -(view->width >> 1);
    b.y = y;
    b.z = 0.0f;
    func_0023DD58((Vec *)view->x30.m[2], &origin, &b, &a);

    a.x = -(view->width >> 1);
    a.y = 0.0f;
    a.z = 0.0f;
    b.x = 0.0f;
    b.y = y;
    b.z = view->height;
    func_0023DD58((Vec *)view->x70.m[0], &origin, &a, &b);

    a.x = -(view->width >> 1);
    a.y = 0.0f;
    a.z = 0.0f;
    b.x = 0.0f;
    b.y = y;
    b.z = -view->height;
    func_0023DD58((Vec *)view->x70.m[1], &origin, &b, &a);

    view->x70.m[2][0] = 0.0f;
    view->x70.m[2][1] = -1.0f;
    view->x70.m[2][2] = 0.0f;
    view->x70.m[2][3] = 0.0f;

    if (entry->mode != 10) {
        angle = func_002484F8((float)(view->width >> 1) / dist) * 57.29578f * 2.0f;
        D_0029E418 = angle;
        view->xB0 = angle;
        D_0029E414 = dist;
    } else {
        angle = 0.0f; /* dead store, but it shapes the generated code */
    }

    proj = &D_0075AA70[idx];
    func_00222170(proj, &D_0075ABF0[idx], &D_0075AD70[idx], view->x0, 1.0f, arg2, view->x10, view->x14, 0.0f, 16777215.0f, 1.0f,
                  16384.0f, view->width >> 1, view->height + 16, 1023.5f);

    COPY_MATRIX(tmp, &view->x30);
    func_00221D58(&view->x30, &tmp);
    COPY_MATRIX(tmp, &view->x70);
    func_00221D58(&view->x70, &tmp);

    if (entry->mode == 10) {
        Matrix *m = &D_0075B370;
        m->m[0][0] = func_0021E430() * 0.015625f;
        m->m[0][1] = 0.0f;
        m->m[0][2] = 0.0f;
        m->m[0][3] = 0.5f;
        m->m[1][0] = 0.0f;
        m->m[1][1] = func_0021E430() * 0.015625f;
        m->m[1][2] = 0.0f;
        m->m[1][3] = 0.5f;
        m->m[2][0] = 0.0f;
        m->m[2][1] = 0.0f;
        m->m[2][2] = 0.0f;
        m->m[2][3] = 1.0f;
        m->m[3][0] = 0.0f;
        m->m[3][1] = 0.0f;
        m->m[3][2] = 0.0f;
        m->m[3][3] = 0.0f;

        proj->m[0][0] = func_0021E430();
        proj->m[0][1] = 0.0f;
        proj->m[0][2] = 0.0f;
        proj->m[0][3] = view->x10;
        proj->m[1][0] = 0.0f;
        proj->m[1][1] = func_0021E430();
        proj->m[1][2] = 0.0f;
        proj->m[1][3] = view->x14;
        proj->m[2][0] = 0.0f;
        proj->m[2][1] = 0.0f;
        proj->m[2][2] = -1024.0623779296875f;
        proj->m[2][3] = 16778240.0f;
        proj->m[3][0] = 0.0f;
        proj->m[3][1] = 0.0f;
        proj->m[3][2] = 0.0f;
        proj->m[3][3] = 1.0f;
    }
}
