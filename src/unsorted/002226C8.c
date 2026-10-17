/* UNIT 0x002226C8 0x002227A8 */
/* Build an orientation matrix whose Y axis is dir; X stays in the XY plane. */
#include "vector.h"
void func_002226C8(Matrix4 *m, const Vector4 *dir)
{
    float len;

    m->row[1].x = dir->x;
    m->row[1].y = dir->y;
    m->row[1].z = dir->z;
    __asm__ volatile ("sqrt.s %0, %1" : "=&f"(len) : "f"(dir->x * dir->x + dir->y * dir->y));
    if (len > 1e-6f) {
        m->row[0].x = dir->y / len;
        m->row[0].y = -dir->x / len;
        m->row[2].x = m->row[0].y * dir->z;
        m->row[2].y = -m->row[0].x * dir->z;
    } else {
        m->row[0].x = 1.0f;
        m->row[0].y = 0.0f;
        m->row[2].x = 0.0f;
        m->row[2].y = -m->row[1].z;
    }
    m->row[3].w = 1.0f;
    m->row[0].z = m->row[3].x = m->row[3].y = m->row[3].z = m->row[0].w = m->row[1].w = m->row[2].w = 0.0f;
    m->row[2].z = len;
}
