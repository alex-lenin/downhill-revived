/* UNIT 0x00185D38 0x00185DE0 .sdata=0x29CD6C:0x29CD70 .sbss=0x29DE18:0x29DE1C */
struct Req {
    int x0;
    int x4;
    char pad8[4];
    int xC;
    int x10;
};
extern void func_00246AB0(int, int);
extern int func_002466A0(int, int *, int *);
extern void func_00184938(struct Req *, int);
int D_0029CD6C = 16;
int D_0029DE18 __attribute__((section(".sbss"))) = 0;

int func_00185D38(int *state, struct Req *req) {
    int step;

    if (req->x0 != 0) {
        return 1;
    }
    step = *state;
    switch (step) {
    case 0:
        func_00246AB0(req->x10, req->xC);
        step = *state;
        *state = ++step;
    case 1:
        if (func_002466A0(1, &D_0029CD6C, &D_0029DE18) != 0) {
            if (D_0029DE18 < 0) {
                func_00184938(req, req->x4);
            }
            return 1;
        }
        break;
    }
    return 0;
}
