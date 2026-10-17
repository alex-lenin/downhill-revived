/* UNIT 0x0010A2F8 0x0010A388 */
struct List { int x0,x4; unsigned int count; int entries[1]; };
extern void func_0010A528(int,int,int);
void func_0010A2F8(int a,int b,struct List *list) {
 unsigned short i=0;
 if(list->count) do { func_0010A528(list->entries[i],b,a); i++; } while(i<list->count);
}
