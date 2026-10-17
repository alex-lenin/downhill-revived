/* UNIT 0x00207A10 0x00207B40 */
typedef struct Names { char pad[0x488]; int count; char names[1][32]; } Names;
extern Names D_002794A8;
extern unsigned char D_0029B301[];
extern unsigned int strlen(const char *);
int func_00207A10(char *name) {
    int index;
    unsigned int character;
    int equal;
    unsigned int length;
    int a,b,upperA,upperB;
    for(index=0;index<D_002794A8.count;index++) {
        character=0;
        length=strlen(name);
        equal=length==strlen(D_002794A8.names[index]);
        while(equal && character<strlen(name)) {
            a=name[character];
            b=D_002794A8.names[index][character];
            character++;
            upperA=D_0029B301[a]&2 ? a-32:a;
            upperB=D_0029B301[b]&2 ? b-32:b;
            if(upperA^upperB) equal=0;
        }
        if(equal) return 1;
    }
    return 0;
}
