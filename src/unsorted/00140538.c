/* UNIT 0x00140538 0x00140678 .sdata=0x29C46C:0x29C470 */
typedef struct Vec4 { float x,y,z,w; } Vec4 __attribute__((aligned(16)));
typedef struct Mtx { Vec4 r[4]; } Mtx;
typedef struct Body { unsigned int *x0; char pad4[12]; Vec4 x10; char pad20[32]; Mtx x40; } Body;
typedef struct Game { char pad[0x78D8]; int x78D8,x78DC,x78E0,x78E4; char pad78E8[0x7970-0x78E8]; unsigned int x7970; char pad7974[8]; int x797C; } Game;
Body *D_0029C46C = 0;
extern Game D_002AF2C0;
extern char D_0036B338[];
extern int func_00140690(void *, Body *);
extern void func_00218278(Body *, int);
extern void func_0012BEF0(Game *, Body *);
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
        : "=m"((dst).r[0]), "=m"((dst).r[1]), "=m"((dst).r[2]), "=m"((dst).r[3]) \
        : "r"(src) : "$8", "$9", "$10", "$11", "memory")

typedef struct Input { int x0,x4,x8,xC,x10,x14[1]; } Input;
void func_00140538(Input *arg0, int *arg1) {
 Vec4 pos; Mtx mat; Game *game;
 if (D_0029C46C) {
 game=&D_002AF2C0;
 game->x797C=0;
 COPY_VECTOR(pos,&D_0029C46C->x10);
 COPY_MATRIX(mat,&D_0029C46C->x40);
 if ((unsigned)(arg1[0]-35)>=3) func_00218278(D_0029C46C,func_00140690(D_0036B338,D_0029C46C));
 func_0012BEF0(game,D_0029C46C);
 game->x7970=*D_0029C46C->x0 >>18;
 COPY_VECTOR(D_0029C46C->x10,&pos);
 COPY_MATRIX(D_0029C46C->x40,&mat);
 if ((unsigned)(arg1[0]-35)>=3) {
 int *p=&game->x78D8;
 p[1]=arg0->x4; game->x78D8=arg0->x10; game->x78E0=arg0->x8; game->x78E4=arg0->xC; p[6]=((Input *)((unsigned int)arg0 + (arg1[2]<<2)))->x14[0];
 }
 }
}
