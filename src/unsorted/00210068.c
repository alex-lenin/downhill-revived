/* UNIT 0x00210068 0x00210250 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Packet { u128 q; unsigned int w[4]; long d[2]; unsigned char b[16]; float f[4]; } Packet;
extern Packet *D_004F35B0[],*D_00513AC0[],*D_00513AD0[];
extern int D_00513AF0[];
void func_00210068(int index,Packet *p,Packet *q,int size) {
 Packet *start; int n; long x,y;
 p->q=0; p->w[0]=0x60000000; start=p; p++;
 p->d[1]=14;p++;
 y=(((long)(((size-0x840)<<10)>>16))<<48)>>48;
 x=(((long)(((size-0x40)<<10)>>16))<<48)>>48;

 p->d[0]=0;p->d[1]=63;p++;
 D_004F35B0[index]=p;
 p->d[0]=((y|((0xBBA8L<<19)|0x8000))|(x<<37))|(0x8000L<<46);
 p->d[1]=6;p++;
 p->d[0]=0x61;p->d[1]=20;p++;
 p->d[0]=0x3008d;p->d[1]=0x47;p++;
 p->d[0]=(0x8000L<<24)|0x44;p->d[1]=0x42;p++;
 p->d[0]=0;p->d[1]=0x4a;p++;
 p->d[0]=0;p->d[1]=0x49;p++;
 p->d[0]=1;p->d[1]=0x1a;p++;
 n=p-start;
 start->w[0]=(n-1)|0x60000000;
 start[1].d[0]=(n-2)|0x8000|(0x8000L<<45);
 D_00513AD0[index]=p;D_00513AF0[index]=1;
 q->q=0;q++;
 q->d[0]=(0x9000L<<46)|1;q->d[1]=16;q++;
 q->d[0]=0x156;
 q->b[8]=0x80;q->b[9]=0x80;q->b[10]=0x40;q->b[11]=0x80;q->f[3]=1.0f;q++;
 D_00513AC0[index]=q;
}
