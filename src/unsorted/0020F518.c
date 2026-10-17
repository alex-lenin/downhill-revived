/* UNIT 0x0020F518 0x0020F588 */
typedef struct { char pad[0x18]; char x18,x19,x1A,x1B; char pad1[4]; } Entry;
extern Entry D_004F3570[];
extern void func_0020F420(int, int);
void func_0020F518(int index, char a, char b, char c, char d) {
    int i;
    Entry *entry = &D_004F3570[index];
    entry->x18=a;
    entry->x19=b;
    entry->x1A=c;
    entry->x1B=d;
    for(i=0;i<4;i++) func_0020F420(index,i);
}
