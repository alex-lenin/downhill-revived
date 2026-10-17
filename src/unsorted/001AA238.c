/* UNIT 0x001AA238 0x001AA318 */
typedef struct Flags { unsigned int flags:6; unsigned int rest:26; } Flags;
typedef struct Entry { char pad[0x1C]; Flags *x1C; char pad20[8]; short x28,x2A; char pad2C[4]; } Entry;
extern unsigned char D_0029E4D0;
extern Entry D_004A4190[];
extern char D_004A5990[];
extern int func_001AA7D8(int,void *);
extern void func_001AA6B8(Entry *,int,int,int);
Entry *func_001AA238(Flags *obj,int a,int b,int c,int d) {
 int offset; Entry *entry;
 if(D_0029E4D0>=127 || !func_001AA7D8(a,D_004A5990)) return 0;
 offset=D_0029E4D0++;
 offset*=0x30;
 entry=(Entry *)((char *)D_004A4190+offset);
 entry->x1C=obj;
 if(obj) entry->x2A=obj->flags;
 func_001AA6B8(entry,a,b,c);
 entry->x28=d;
 return entry;
}
