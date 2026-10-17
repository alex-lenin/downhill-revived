/* UNIT 0x001E75D8 0x001E7A00 */
struct Handle { int x0, x4, x8, xC; };

struct Entry {
    char p0[4];
    int x4;
    char p8[4];
    int xC;
    char p10[4];
    float x14;
    char p18[4];
    float x1C;
    char p20[0x28];
    float x48;
    float x4C;
    float x50;
};

struct Link { int x0; int x4; };

struct Rider;
struct Anim { struct Rider *x0; };

extern struct Entry D_00272888[];
extern struct Entry D_00273D60[];
extern struct Link D_002752B0[];
extern void func_00203E10(struct Handle *, float);
extern float func_00204280(struct Handle *);
extern float func_001E5198(void *);
extern float func_001E5190(void);

static inline struct Handle *GetHandle(char *state, int index)
{
    struct Handle *hp = (struct Handle *)(state + (index * 16 + 0x14));
    return hp;
}

/* recompute the two blend factors of one entry */
static inline void Update(struct Entry *e, char *state, void *control, float one)
{
    struct Handle h;
    float t;

    h = *GetHandle(state, e->x4);
    func_00203E10(&h, one);
    h = *GetHandle(state, e->xC);
    func_00203E10(&h, one);
    h = *GetHandle(state, e->x4);
    t = func_00204280(&h) * (one / e->x14);
    h = *GetHandle(state, e->xC);
    t += func_00204280(&h) * (one / e->x1C) * e->x48;
    t *= one / func_001E5198(control);
    e->x4C = (float)(int)t * func_001E5190();
    h = *GetHandle(state, e->xC);
    e->x50 = func_00204280(&h) * (one / e->x1C) * e->x48 * (one / func_001E5198(control)) * func_001E5190();
}

/* The (dead) initializers of e and pair are needed: they change gcse's hash
   table, which decides which %hi stays in $fp across the loop. */
void func_001E75D8(struct Anim *a)
{
    struct Entry *e = D_00272888;
    struct Entry *pair = D_00273D60;
    char *state = (char *)a->x0 + 0x1E40;
    void *control = (char *)a->x0 + 0x6610;
    int i;

    for (i = 0; i < 43; i++) {
        e = &D_00272888[i];
        Update(e, state, control, 1.0f);
        pair = &D_00273D60[D_002752B0[i].x4];
        Update(pair, state, control, 1.0f);
    }
}
