/* UNIT 0x00194B50 0x00194B98 */
typedef struct Object { long words[0x12240 / 8]; } Object;
Object *func_00194B50(Object *dest, const Object *source) {
    *dest = *source;
    return dest;
}
