/* UNIT 0x0023C498 0x0023C6B8 */
struct Frame { int width, height, x8; };
struct Queue { char *x0, *x4; char pad8[8]; int x10; };
extern struct Queue D_00663B40;
extern char D_00675480[];
extern char D_002993C0[];
extern void func_00224058(void);
extern void func_002243E8(char *);
extern int func_0023C288(struct Frame *);
extern void func_0023E030(struct Queue *);
extern void *func_0023E0A8(struct Queue *);
extern int func_0024D168(struct Frame *, void *, int);
extern int func_0024D1C0(struct Frame *);
extern void func_0024D1E0(struct Frame *);
extern void func_0020B9E8(void *, void *, void *, int, int, int, int, int, int, int);
int func_0023C498(struct Frame *frame) {
    int result = 1;
    void *data;
    int width, height, x, y, halfHeight, i, sourceOffset, destOffset;
    while (!func_0024D1C0(frame)) {
        if (func_0023C288(frame) == 1) { result = -1; break; }
        while (!(data = func_0023E0A8(&D_00663B40))) func_00224058();
        if (func_0024D168(frame, data, 0x460) < 0) func_002243E8(D_002993C0);
        if (!frame->x8) {
            width = frame->width;
            height = frame->height;
            x = (640 - width) >> 1;
            halfHeight = height >> 1;
            y = (448 - height) >> 2;
            for (i = 0; i < D_00663B40.x10; i++) {
                func_0020B9E8(D_00675480, D_00663B40.x4 + i * 0x85740 + 0x40, D_00663B40.x0 + i * 0x118000, 0, x, y, width, halfHeight, width, height);
                func_0020B9E8(D_00675480, D_00663B40.x4 + i * 0x85740 + 0x1AB40, D_00663B40.x0 + i * 0x118000, 1, x, y, width, halfHeight, width, height);
            }
        }
        func_0023E030(&D_00663B40);
        func_00224058();
    }
    func_0024D1E0(frame);
    return result;
}
