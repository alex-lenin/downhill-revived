/* UNIT 0x00184AB0 0x00184BB0 */
typedef struct Stream {
    char pad0[0x8];
    int x8;
    int buf;
    int size;
} Stream;

typedef struct Request {
    int busy;
    int x4;
    char pad8[0xC - 0x8];
    int xC;
    int x10;
} Request;

extern void func_00184938(Request *, int);
extern int func_002460F8(int, int, int, int);
extern int func_00246258(int);
extern int func_002463A0(int, int, int);
extern int func_002466A0(int, int *, int *);

int func_00184AB0(Stream *stream, Request *req) {
    int cmd;
    int fd;
    int result;

    if (req->busy == 0) {
        func_002460F8(req->x10, req->xC, stream->x8, 1);
        cmd = 2;
        func_002466A0(0, &cmd, &fd);
        if (fd == -1) {
            func_00184938(req, req->x4);
            return 1;
        }
        func_002463A0(fd, stream->buf, stream->size);
        cmd = 5;
        func_002466A0(0, &cmd, &result);
        if (result < stream->size) {
            func_00184938(req, req->x4);
            return 1;
        }
        func_00246258(fd);
        cmd = 3;
        func_002466A0(0, &cmd, &result);
        if (result < 0) {
            func_00184938(req, req->x4);
        }
    }
    return 1;
}
