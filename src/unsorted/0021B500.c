/* UNIT 0x0021B500 0x0021B668 */
typedef struct EntryBits {
    unsigned int index : 14;
    unsigned int pad : 6;
    unsigned int bank : 5;
    unsigned int count : 3;
    unsigned int pad2 : 3;
    unsigned int active : 1;
} EntryBits;

typedef struct Entry {
    int x0;
    union {
        EntryBits b;
        unsigned int word;
    } x4;
    int x8;
    int xC;
} Entry;

typedef struct Header {
    unsigned long pad : 48;
    unsigned long flags : 16;
    unsigned short ids[1];
} Header;

typedef struct Elem {
    char ref[0x20];
    long x20[2];
} Elem;

typedef struct Tag {
    union {
        unsigned int word;
        struct {
            unsigned short qwc;
            unsigned short kind;
        } s;
    } x0;
    struct Tag *next;
    union {
        long x8;
        struct {
            Header *hdr;
            Elem *data;
        } p;
    } u;
} Tag;

typedef struct Ctx {
    char pad0[0x8];
    Tag *tag;
    char padC[0x20 - 0xC];
} Ctx;

extern Entry D_00621310[];
extern void func_0025A960(int);
extern void func_0021B668(unsigned int, void *, Ctx *, long *);

void func_0021B500(Tag *tag)
{
    Header *hdr;
    Tag *p;
    Ctx ctx;
    int i;

    func_0025A960(0);
    while (tag->x0.word != 0x70000000) {
        if (tag->x0.s.kind == 0x5000) {
            tag = (Tag *)((char *)tag + tag->x0.s.qwc * 16) + 1;
        } else {
            if (tag->x0.word != 0x20000000) {
                hdr = tag->u.p.hdr;
                {
                Elem *data;
                data = tag->u.p.data;
                p = tag;
                tag->u.x8 = 0;
            for (i = 0; i < ((int)hdr->flags & 7); i++) {
                ctx.tag = &tag[i * 3];
                p += 3;
                func_0021B668(hdr->ids[i], data, &ctx, data->x20);
                data++;
                p += D_00621310[hdr->ids[i]].x4.b.count * 3;
            }
                }
                tag = (Tag *)((char *)p + p->x0.s.qwc * 16) + 1;
            }
        }
        tag = tag->next;
    }
}
