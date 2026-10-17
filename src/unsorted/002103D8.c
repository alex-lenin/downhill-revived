/* UNIT 0x002103D8 0x00210420 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union {
    u128 all;
    unsigned int word[4];
} Packet;
void func_002103D8(Packet **p,int i,unsigned int value) {
    Packet *q;
    p+=i;
    q=*p;
    q->all=0;
    q->word[0]=0x50000000;
    q->word[1]=value&0xfffffff;
    q++;
    q->all=0;
    q->word[0]=0x70000000;
    *p=q;
}
