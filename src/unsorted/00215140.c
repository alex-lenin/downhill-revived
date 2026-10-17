/* UNIT 0x00215140 0x00215448 .rodata=0x298290:0x298330 */
#include "vector.h"
/* NOTES: Permitted VU0 vector-subtraction assembly. The x8 select as a
   ternary on last->flags keeps v0 dead at the post-loop branch, so the
   loop-back branch's delay slot is not annulled (beq, not beql). */
typedef unsigned int u32;
typedef Vector4 Vec;
typedef Matrix4 Matrix;

typedef struct Cmd {
    u32 flags;
    int x4;
    unsigned char x8;
    char pad9[3];
    float xC;
    Matrix x10;
    Vec x50;
} Cmd;

typedef struct CmdList {
    char pad0[0x1E];
    signed char count;
    char pad1F;
    Cmd *cmds[1];
} CmdList;

typedef struct Result {
    Cmd *cmd;
    Matrix *matrix;
    unsigned short x8;
    unsigned short xA;
} Result;

extern Matrix D_005156C0[];



extern void func_00214EB0(Cmd *, Cmd *, Matrix **);

#define VEC_SUB(out, a, b) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" \
                      : "=m"(out) : "r"(a), "r"(b))

int func_00215140(CmdList *list, Result *out)
{
    Vec delta;
    Matrix *xform;
    Cmd *joint;
    Cmd *table;
    Matrix *matrix;
    Cmd **cmds;
    Cmd *last;
    Cmd *cmd;
    u32 flags;
    int v;
    int i, next, stop, x8, xA;

    matrix = D_005156C0;
    stop = 0;
    i = 0;
    x8 = 1;
    func_00221F20(matrix);
    xA = 0;
    joint = 0;
    table = 0;
    while (i < list->count - 1 && !stop) {
        {
            Cmd **cursor = list->cmds;
            u32 address = ~(u32)cursor;
            address = -address;
            address -= 1;
            cmds = (Cmd **)address;
            cursor += i;
            do {
            cmd = *cursor;
        flags = cmd->flags;
        if ((flags & 0x3F) != 25) {
            if (flags & 0xFFFC0000)
                x8 = flags >> 18;
            if (flags & 0x3FF80)
                { xA = flags >> 7; xA &= 0x7FF; }
        }
        } while (0);
        }
        next = ~i;
        next = -next;
        switch (cmd->flags & 0x3F) {
        case 3:
            VEC_SUB(matrix->row[3], &matrix->row[3], &cmd->x10.row[0]);
            next = i + 1;
            break;
        case 4:
            matrix++;
            func_00221A60(matrix, &cmd->x10, matrix - 1);
            VEC_SUB(delta, &matrix[-1].row[3], &cmd->x10.row[3]);
            func_00221888(&cmd->x10, &delta, &matrix->row[3]);
            next = i + 1;
            break;
        case 17:
            func_00214EB0(cmd, joint, &xform);
            if (cmd->xC == 0.0f) {
                next = i + 1;
                stop = 1;
                break;
            }
            matrix++;
            func_00221A60(matrix, xform, matrix - 1);
            VEC_SUB(delta, &matrix[-1].row[3], &xform->row[3]);
            func_00221888(xform, &delta, &matrix->row[3]);
            next = i + 1;
            break;
        case 6: {
            int following = i + 1;
            if (*(u32 *)((u32)cmd + (cmd->x8 << 2) + 12) != *(u32 *)((u32)cmds + (following << 2)))
                stop = 1;
            next = following;
            break;
        }
        case 25:
            joint = cmd;
            next = i + 1;
            break;
        case 39:
            table = cmd;
            next = i + 1;
            break;
        case 31: {
            int following = i + 1;
            if (*(u32 *)((char *)cmd + table->x4 * 12 + 0xC) != *(u32 *)((u32)cmds + (following << 2)))
                stop = 1;
            next = following;
            break;
        }
        case 9:
            next = i + 1;
            stop = 1;
            break;
        default:
            break;
        }
        i = next;
    }
    if (stop) {
        out->cmd = 0;
        return 0;
    }
    last = list->cmds[i];
    out->matrix = matrix;
    out->cmd = last;
    out->x8 = (last->flags & 0xFFFC0000) ? last->flags >> 18 : x8;
    { u32 flags = out->cmd->flags;
      int v;
    if (flags & 0x3FF80)
        { v = flags; v = (u32)v >> 7; v &= 0x7FF; }
    else
        v = xA;
    out->xA = v;
    }
    return 1;
}
