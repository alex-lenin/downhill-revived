/* UNIT 0x001ABE78 0x001AC098 */
typedef struct Packet { unsigned int type; int size; char data[256]; } Packet;
extern char D_0027EB90[32], D_0027EB70[32], D_0038DB90[256];
extern void *memcpy(void *, const void *, unsigned int);
extern int func_0011B778(void *, int);
extern int func_0011BC18(void *, int);
extern int func_00162B98(void *, int);
extern int func_00162DE8(void *, int);
extern int func_00230928(void *, Packet *);
int func_001ABE78(void *obj, Packet *packet) {
    int size;
    switch (packet->type) {
    case 10:
        size = func_0011B778(packet->data, packet->size) + 8;
        break;
    case 2007:
        size = func_0011BC18(packet->data, packet->size) + 8;
        break;
    case 2005:
        memcpy(D_0027EB90, packet->data, 32);
        size = 40;
        break;
    case 2001:
        memcpy(D_0027EB70, packet->data, 32);
        size = 40;
        break;
    case 2002:
        size = func_00162B98(packet->data, packet->size) + 8;
        break;
    case 2003:
        size = func_00162DE8(packet->data, packet->size) + 8;
        break;
    case 2008:
        memcpy(D_0038DB90, packet->data, 256);
        size = 264;
        break;
    default:
        size = func_00230928(obj, packet);
        break;
    }
    return size;
}
