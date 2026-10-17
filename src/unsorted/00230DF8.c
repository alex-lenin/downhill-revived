/* UNIT 0x00230DF8 0x00230E80 */
extern void func_00254544(void *, void *);
void func_00230DF8(char *obj, void *source, int flag) {
 signed char buffer[64]; int i, count; char *dest; char *tail;
 func_00254544(buffer,source);
 dest=obj+0x8040; count=0; i=0;
 do { if(buffer[i]!=32) { *(char *)((int)dest+count)=(unsigned char)buffer[i]; count++; } i++; } while(i<64);
 tail=obj+0x8000; ((char *)((int)tail+count))[0x40]=0; tail[0x80]=flag;
}
