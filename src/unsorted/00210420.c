/* UNIT 0x00210420 0x00210460 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Packet { u128 all; unsigned int words[4]; } Packet;
typedef struct Obj { char pad[0xC90]; Packet *xC90; } Obj;
void func_00210420(Obj *obj, unsigned int address) {
    Packet *packet = obj->xC90;
    address &= 0x0FFFFFFF;
    packet->all = 0;
    packet->words[0] = 0x50000000;
    packet->words[1] = address;
    ++packet;
    packet->all = 0;
    packet->words[0] = 0x70000000;
    obj->xC90 = packet;
}
