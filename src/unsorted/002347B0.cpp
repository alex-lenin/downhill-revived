/* UNIT 0x002347B0 0x00234BD8 */
struct Item {
    int index;
    int score;
};

struct Rec {
    char pad0[0x5];
    unsigned char flags;
    char pad6[0x8 - 0x6];
    int score;
};

struct Key {
    int key;
    int pad;
};

struct Link {
    int key;
    int offset;
};

struct Obj {
    char pad0[0x1D4];
    Rec *recs;
    char *base;
    char pad1DC[0x5F9 - 0x1DC];
    unsigned char x5F9;
    char pad5FA[0x5FC - 0x5FA];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual unsigned int Random();
};

extern "C" Link *func_00234608(Link *first, int count, int key);

extern "C" void func_002347B0(Obj *self, unsigned int *node, unsigned int n, Key *path,
                              int *count, Item *list, int *worst, int *worstIdx, int depth)
{
    unsigned int i, j, k, start;
    Link *link;
    Key *key;
    int index, max;
    int *items;
    unsigned char flags;

    link = (Link *)(node + 1);
    for (i = 0; i < n && node[0] != 0; i++) {
        key = &path[i];
        if (key->key == -1) {
            for (k = 0; k < node[0] - 1; k++, link++) {
                func_002347B0(self, (unsigned int *)(self->base + link->offset), n - i - 1, &path[i] + 1,
                              count, list, worst, worstIdx, depth + i + 1);
            }
        } else {
            if (link->key == -2 && key->key != -2) {
                func_002347B0(self, (unsigned int *)(self->base + link->offset), n - i - 1, &path[i + 1],
                              count, list, worst, worstIdx, depth + i + 1);
            }
            link = func_00234608(link, node[0], key->key);
        }
        if (link == 0) {
            /* retail places this path after the leaf code */
            goto notfound;
        }
        node = (unsigned int *)(self->base + link->offset);
        link = (Link *)(node + 1);
    }
leaf:
    if (node != 0 && node[0] != 0) {
        start = self->Random() % node[0];
        items = (int *)(node + 1);
        k = start;
        do {
            index = items[k];
            flags = self->recs[index].flags;
            if ((flags & 4) && self->x5F9) {
            } else if (flags & 2) {
            } else if (flags & 1) {
            } else {
                if (*count < 10) {
                    list[*count].index = index;
                    list[*count].score = self->recs[index].score;
                    if (*worst < self->recs[index].score) {
                        *worst = self->recs[index].score;
                        *worstIdx = *count;
                    }
                    self->recs[index].flags |= 2;
                    (*count)++;
                } else if (self->recs[index].score < *worst) {
                    self->recs[list[*worstIdx].index].flags &= ~2;
                    list[*worstIdx].index = index;
                    list[*worstIdx].score = self->recs[index].score;
                    self->recs[index].flags |= 2;
                    max = -1;
                    for (j = 0; j < 10; j++) {
                        if (max < list[j].score) {
                            max = list[j].score;
                            *worstIdx = j;
                        }
                    }
                }
            }
            k++;
            if (k >= node[0]) {
                k = 0;
            }
        } while (k != start);
    }
    return;
notfound:
    node = 0;
    goto leaf;
}
