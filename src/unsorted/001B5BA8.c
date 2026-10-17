/* UNIT 0x001B5BA8 0x001B5C18 */
extern int func_001B5B88(unsigned char *);
extern void func_001B5B70(unsigned char *,int);
unsigned char *func_001B5BA8(unsigned char *dest, unsigned char *src) {
    int i=0;
    do { dest[i]=src[i]; i++; } while(i<9);
    func_001B5B70(dest,func_001B5B88(src));
    dest[0x60]=src[0x60];
    return dest;
}
