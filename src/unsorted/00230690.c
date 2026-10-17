/* UNIT 0x00230690 0x002306D8 */
typedef struct { char pad[0x50000]; int x50000, x50004, x50008; } Buffer;
int func_00230690(Buffer *buffer, char **out) {
    int used = buffer->x50004;
    if (used != 0) {
        int capacity = buffer->x50008;
        *out = (char *)buffer + ((buffer->x50000 - used + capacity) % capacity);
    }
    return buffer->x50004;
}
