/* UNIT 0x001692C8 0x00169570 */
typedef struct Obj {
 char pad[0x534]; int x534,x538; char pad53C[6]; unsigned short x542,x544; char pad546[4]; unsigned short x54A; char pad54C; unsigned char x54D,x54E,x54F,x550,x551,x552; char pad553[4]; unsigned char x557,x558;
} Obj;
typedef struct Entry { int kind, x4, index; } Entry;
extern struct Globals { char pad[0x50130]; char *x50130; } D_002AF2C0;
extern void func_001E5300(void *, int **, int **, int **, int **);
void func_001692C8(Obj *obj, Entry *entries, int count, int value, int flag1, int flag2) {
 int counts[8]; int *a,*b,*c,*d; int i,j,n; char *game; int *p,*q,*r,*t; int **ap,**bp,**cp,**dp; Entry *entry,*scan;
 obj->x557=flag1; obj->x558=flag2; n=0; game=D_002AF2C0.x50130; ap=&a; bp=&b; cp=&c; dp=&d; obj->x534=value;
 obj->x54E=0; obj->x54D=0; obj->x54F=0; obj->x550=0; obj->x552=0; obj->x551=0;
 obj->x542=0; obj->x544=0; obj->x54A=0;
 i=7; do { counts[i]=0; i--; } while(i>=0);
 func_001E5300(game+0x6610,ap,bp,cp,dp);
 p=a+1; q=b+1;
 { for(j=0;j<42;j++) {
  if(*p>0) { obj->x542++; if(*q>0) { unsigned short value=(unsigned short)*q; int sum=obj->x544+value; int total=obj->x54A+1; obj->x542++; obj->x544=sum; obj->x54A=total; } }
  q++; p++;
 } }
 r=a+29;
 { for(j=0;j<14;j++,r++) if(*r>0) obj->x551=1; }
 t=c+1;
 { for(j=0;j<7;j++,t++) {
  if(*t>0) { if(obj->x557 || obj->x558) obj->x552=1; obj->x542++; }
 } }
 obj->x538=count;
 if(count>0) { scan=entries; i=count; do { int next=n+1; if(scan->kind==46) n=next; scan++; } while(--i); }
 if(n && n<count && obj->x552) obj->x54E=1;
 if(count-n>=2) obj->x54D=1;
 entry=entries;
 if(count>0) { i=count; do { if(entry->kind==46) counts[entry->index]++; entry++; } while(--i); }
 for(i=0;i<8;i++) {
  if(obj->x54D) { if(counts[i]>=2) obj->x54F=1; if(obj->x54D && counts[i]>=3) obj->x550=1; }
 }
}
