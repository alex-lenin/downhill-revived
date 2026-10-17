/* UNIT 0x00230FC8 0x00231058 */
extern void func_002311C8(unsigned short);
extern void func_00254D08(char *);
extern int func_0025A4F0(int);
extern int func_0025A6F0(int);
extern char D_002991B0[];
extern int D_0029E33C,D_0029E344,D_0029E348;
extern volatile int D_0029E34C;
typedef struct Arena {
 int x0;
 int *x4;
 int *x8;
} Arena;
extern Arena D_00663B08[];
void func_00230FC8(void) {
 Arena *arena;
 int *current;
 while (!D_0029E344) func_002311C8(50);
 if(func_0025A4F0(D_0029E33C)==-1) func_00254D08(D_002991B0);
 func_0025A6F0(D_0029E348);
 func_0025A6F0(D_0029E34C);
 arena = D_00663B08;
 current = arena[1].x8;
 arena[1].x4 = current + 1;
 arena[1].x8 = (int *)*current;
}
