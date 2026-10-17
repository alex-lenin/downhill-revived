/* UNIT 0x001ABC28 0x001ABE78 */
typedef struct { int type,size; char data[0]; } Chunk;
extern int func_00230728(void *, char *);
extern int func_0011B588(void *);
extern int func_0011BA48(void *);
extern int func_00162C20(void *);
extern int func_00162F38(void *);
extern char D_0027EB90[],D_0027EB70[],D_0038DB90[];
extern void *memcpy(void *,const void *,unsigned);
int func_001ABC28(void *source,char *buffer) {
 int offset,size1,size2,size3,size4; Chunk *a,*b,*c,*d,*e,*f,*g;
 offset=func_00230728(source,buffer);
 a=(Chunk *)(buffer+offset); a->type=10;
 size1=func_0011B588(buffer+(offset+8))+8; a->size=size1; offset+=size1;
 b=(Chunk *)(buffer+offset); b->type=2007;
 size2=func_0011BA48(buffer+(offset+8))+8; b->size=size2; offset+=size2;
 c=(Chunk *)(buffer+offset); c->type=2005; c->size=40;
 memcpy(c->data,D_0027EB90,32); offset+=c->size;
 d=(Chunk *)(buffer+offset); d->type=2001; d->size=40;
 memcpy(d->data,D_0027EB70,32); offset+=d->size;
 e=(Chunk *)(buffer+offset); e->type=2002;
 size3=func_00162C20(buffer+(offset+8))+8; e->size=size3; offset+=size3;
 f=(Chunk *)(buffer+offset); f->type=2003;
 size4=func_00162F38(buffer+(offset+8))+8; f->size=size4; offset+=size4;
 g=(Chunk *)(buffer+offset); g->type=2008; g->size=264;
 memcpy(g->data,D_0038DB90,256); offset+=g->size;
 return offset;
}
