/* UNIT 0x0021AE70 0x0021AFF8 */
typedef struct { unsigned short x0; unsigned char x2,x3; unsigned int x4; } Entry;
typedef struct { int count,x4; Entry entries[1]; } Table;
typedef struct { unsigned long x0,x8; } Texture;
extern char D_002989B8[];
extern void func_00254D08(const char *,...);
int func_0021AE70(Table *table,Texture *texture) {
 unsigned long bits=texture->x8;
 unsigned long kind=bits & (0xFC00UL<<22);
 if(kind==(0x9800UL<<21) || kind==(0xA000UL<<21)) {
  unsigned int size=(1<<((int)(bits>>40)&15))<<((int)(bits>>44)&15);

  if((bits&(0xFF3FUL<<32))==(0x9913UL<<32)) {
   int i;
   for(i=0;i<table->count;i++) if(table->entries[i].x4==size) return i;
  } else {
   int n,i;
   if(kind==(0x9800UL<<21)) n=size; else n=(int)size/2;
   if(n>256) func_00254D08(D_002989B8);
   for(i=0;i<table->count;i++) if(table->entries[i].x4<=256) return i;
  }
 } else {
  int i;
 for(i=0;i<table->count;i++) {
   if(table->entries[i].x2==((int)(bits>>40)&15) && table->entries[i].x3==((int)(bits>>44)&15)) return i;
  }
  return -1;
 }
 return -1;
}
