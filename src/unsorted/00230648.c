/* UNIT 0x00230648 0x00230690 */
typedef struct { char pad[0x50000]; int x50000, x50004, x50008; } Buffer;
int func_00230648(Buffer *buffer, int count) {
    int amount = count;
    int remaining = buffer->x50008 - buffer->x50004;
    if (count > remaining) amount = remaining;
    buffer->x50000 = (buffer->x50000 + amount) % buffer->x50008;
    buffer->x50004 += amount;
    return amount;
}
