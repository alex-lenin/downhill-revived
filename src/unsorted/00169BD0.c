/* UNIT 0x00169BD0 0x00169DE8 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Packet {u128 q; unsigned long d[2]; unsigned int w[4];} Packet;
typedef struct Obj {char pad[0x55c]; unsigned int *x55C,*x560,*x564;} Obj;
extern void func_0023D618(int,int *,int *);
extern void func_0023D650(int,int *,int *);
void func_00169BD0(Obj *obj, Packet *p, int left,int top,int width,int height) {
    int x,y,w,h;
    int x0,y0,x1,y1;
    Packet *base=p;
    p++;
    p->d[0]=((unsigned long)0x8000<<45)|0x8001;
    p->d[1]=0xE;
    p++;
    p->d[0]=((unsigned long)0xA000<<23)|0x44;
    p->d[1]=0x42;
    p++;
    p->d[0]=((unsigned long)0xC08D<<46)|0x8003;
    p->d[1]=0x551;
    func_0023D618(0,&x,&y);
    p++;
    func_0023D650(0,&w,&h);
    left += x-(w>>1);
    top += y-(h>>1);
    p->q=0; p->w[3]=0; obj->x55C=&p->w[3]; p++;
    x0=left<<4; y0=top<<4;
    p->q=0; p->w[0]=x0; p->w[1]=y0; p->w[2]=0xFFFF28; p++;
    x1=(left+width)<<4;
    p->q=0; p->w[0]=x1; p->w[1]=(top+2)<<4; p->w[2]=0xFFFF28; p++;
    top+=height; y1=top<<4;
    p->q=0; p->w[3]=0; obj->x560=&p->w[3]; p++;
    p->q=0; p->w[0]=x0; p->w[1]=(top-2)<<4; p->w[2]=0xFFFF28; p++;
    p->q=0; p->w[0]=x1; p->w[1]=y1; p->w[2]=0xFFFF28; p++;
    p->q=0; p->w[3]=0; obj->x564=&p->w[3]; p++;
    p->q=0; p->w[0]=x0; p->w[1]=y0; p->w[2]=0xFFFF28; p++;
    p->q=0; p->w[0]=x1; p->w[1]=y1; p->w[2]=0xFFFF28; p++;
    base->q=0;
    base->w[0]=((int)((char *)p-(char *)base)>>4)|0x60000000;
}
