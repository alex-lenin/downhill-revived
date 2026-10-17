/* UNIT 0x001859B8 0x00185A30 */
struct Request {
    int x0;
    char pad4[0xC - 0x4];
    int xC;
    int x10;
};

struct Task {
    char pad0[0x8];
    int x8;
    char xC;
};

extern int func_00246960(int, int, int, int, int, void *);
extern void func_002466A0(int, int *, int *);

int func_001859B8(struct Task *task, struct Request *req)
{
    char buf[0x40];
    int type;
    int result;

    if (req->x0 != 0) {
        return 1;
    }
    func_00246960(req->x10, req->xC, task->x8, 0, 1, buf);
    type = 0xD;
    func_002466A0(0, &type, &result);
    task->xC = result > 0;
    return 1;
}
