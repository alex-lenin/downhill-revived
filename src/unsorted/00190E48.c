/* UNIT 0x00190E48 0x00191078 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Packet { u128 all; struct { unsigned long lo; unsigned long hi; } h; unsigned int word[4]; } Packet;
typedef struct Widget { char pad0[0x20]; int handle; char pad24[0x118]; unsigned int *x13C,*x140,*x144; } Widget;
extern void func_0023D618(int,int *,int *),func_0023D650(int,int *,int *);
void func_00190E48(Widget *obj,Packet *p,int x,int y,int width,int height) {
    int vals[4];
    int handle=obj->handle;
    Packet *buffer;
    int x0,y0,x1,y1;
    buffer=p; ++p;
    p->h.lo=((unsigned long)0x8000<<45)|0x8001;
    p->h.hi=0xE; p=(Packet *)((int)p+16);
    p->h.lo=((unsigned long)0xA000<<23)|0x44; p->h.hi=0x42; p=(Packet *)((int)p+16);
    p->h.lo=((unsigned long)0xC08D<<46)|0x8003; p->h.hi=0x551;
    func_0023D618(handle,&vals[0],&vals[1]); p=(Packet *)((int)p+16);
    func_0023D650(handle,&vals[2],&vals[3]);
    p->all=0; p->word[3]=0x7F; obj->x13C=&p->word[3]; p=(Packet *)((int)p+16);
    x+=vals[0]-(vals[2]>>1); y+=vals[1]-(vals[3]>>1);
    p->all=0; x0=x*16; y0=y*16;
    p->word[0]=x0; p->word[1]=y0; p->word[2]=0xFFFF28; p=(Packet *)((int)p+16);
    x+=width; p->all=0; x1=x*16;
    p->word[0]=x1; p->word[1]=(y+2)*16; y+=height; p->word[2]=0xFFFF28; p=(Packet *)((int)p+16);
    p->all=0; p->word[3]=0x7F; obj->x140=&p->word[3]; p=(Packet *)((int)p+16);
    p->all=0; y1=y*16;
    p->word[1]=(y-2)*16; p->word[0]=x0; p->word[2]=0xFFFF28; p=(Packet *)((int)p+16);
    p->all=0; p->word[0]=x1; p->word[1]=y1; p->word[2]=0xFFFF28; p=(Packet *)((int)p+16);
    p->all=0; p->word[3]=0x28; obj->x144=&p->word[3]; p=(Packet *)((int)p+16);
    p->all=0; p->word[0]=x0; p->word[1]=y0; p->word[2]=0xFFFF28; p=(Packet *)((int)p+16);
    p->all=0; p->word[0]=x1; p->word[1]=y1; p->word[2]=0xFFFF28; p=(Packet *)((int)p+16);
    buffer->all=0; buffer->word[0]=((int)((char *)p-(char *)buffer)>>4)|0x60000000;
}
