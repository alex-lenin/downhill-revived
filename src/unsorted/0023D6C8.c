/* UNIT 0x0023D6C8 0x0023D788 */
typedef struct Entry {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
} Entry;

extern Entry D_0075A9D0[];
extern int D_0075A9C0[];
extern float D_0026CF48[];
extern int D_0029E458;
extern int D_0029E45C;
extern int D_0029E444;
extern int D_0029E448;
extern int D_0029E44C;
extern void func_0023D788(int);

void func_0023D6C8(void) {
    int i;

    for (i = 0; i < 6; i++) {
        D_0075A9D0[i].x0 = 0;
        D_0075A9D0[i].x4 = 0;
        D_0075A9D0[i].x8 = 0;
        D_0075A9D0[i].xC = 0;
        D_0075A9C0[i] = 0;
        func_0023D788(i);
    }
    D_0029E458 = 1;
    D_0026CF48[2] = -2048.0f;
    D_0026CF48[3] = 2047.0f;
    D_0029E45C = 0;
    D_0029E444 = 0;
    D_0029E448 = 0;
    D_0029E44C = 0;
}
