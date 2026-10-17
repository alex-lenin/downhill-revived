/* UNIT 0x0020FDB0 0x0020FFD0 */
typedef struct { unsigned short x0,x2,x4,x6,x8,xA,xC,xE,x10,x12; char pad14[12]; } Font;
typedef struct { unsigned short x0,x2; int x4; short x8,xA; int xC; } Vertex;
extern Font D_004F3570[];
extern Vertex *D_00513AC0[], *D_00513AD0[];
extern int D_00513AE0[],D_00513AF0[],D_0029E188;
extern int func_0020F6C0(int,int *);
extern void func_0023D618(int,int *,int *),func_0023D650(int,int *,int *),func_0020FFD0(int,int,int *,int *);
static inline void position(Vertex *p,int x,int y) { p->x8=x; p->xC=0xFFFFFF; p->xA=y; }
static inline void position_end(Vertex *p,Font *font,int x,int y) { p->x8=font->x8+x; p->xC=0xFFFFFF; p->xA=font->xA+y; }
static inline void set_x(Vertex *p,int x) {p->x8=x;}
static inline void set_y(Vertex *p,int y) {p->xA=y;}
static inline void end_position(Vertex *p,int x,unsigned short *y,int dy) { p->x8=x; p->xC=0xFFFFFF; p->xA=*y+dy; }
int func_0020FDB0(int index,int arg1,int ch) {
 int result,x,y,w,h,advance,dx,dy;
 int px,py,lx,ly;
 int *count;
 int u,v;
 Font *font=&D_004F3570[index]; Vertex *packet=D_00513AC0[arg1];
 ch &=255; result=0;
 advance=func_0020F6C0(ch,&result); font->x10+=advance;
 func_0023D618(arg1,&x,&y); func_0023D650(arg1,&w,&h);
 dx=x*16-w*8; dy=y*16-h*8;
 if(!D_0029E188 && (unsigned)(ch-0x61)<26) ch=(ch-32)&255;
 if(D_00513AF0[arg1]!=2) {
 D_00513AF0[arg1]=2; D_00513AD0[arg1]=packet;
 ((unsigned long *)packet)[0]=0x4400000000000001UL;
 ((unsigned long *)packet)[1]=0x53D3;
 packet++;
 }
 func_0020FFD0(0,ch,&u,&v);
 packet->x0=*(unsigned short *)&u; count=&D_00513AE0[arg1]; packet->x2=*(unsigned short *)&v;
 px=font->x10+dx; py=font->x12+dy;
 position(packet,px,py);
 packet++;
 packet->x0=*(unsigned short *)&u+font->x4; packet->x2=*(unsigned short *)&v+font->x6;
 lx=font->x8+px; set_x(packet,lx);
 ly=font->xA+py; packet->xC=0xFFFFFF; set_y(packet,ly);
 packet++; D_00513AC0[arg1]=packet;
 font->x10+=font->xE+advance; (*count)++;
 return result;
}
