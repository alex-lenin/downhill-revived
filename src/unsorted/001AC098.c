/* UNIT 0x001AC098 0x001AC140 */
extern void func_00254204(void *,int,int);
extern void func_001B0B90(void *);
extern void func_001CFEE0(void *,void *);
extern void func_001AC140(void *,int,int);
#define I(o) (*(int *)(obj+(o)))
void func_001AC098(char *obj,int value) {
 char *p; int count;
 func_00254204(obj,0,0x2190);
 I(0x16D0)=value;
 p=obj+0x24; count=0xFC;
 do { *(int *)p=0; count--; p+=0x10; } while(count>=0);
 I(0x16E0)=1;
 func_001B0B90(obj+0x16F0);
 obj[0x2160]=1;
 I(8)=0; I(0xC)=0; I(0x10)=0;
 func_001CFEE0(obj+0x2168,obj);
 I(0x2164)=0;
 func_001AC140(obj,0,0);
 I(0xFF4)=0;
}
