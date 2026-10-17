/* UNIT 0x00208D68 0x00208E40 */
extern float func_00248D90(float, float);

/* Heading (degrees, 0..359) of the vector from b to a in the basis (axisX, axisY). */
int func_00208D68(float *axisX, float *axisY, float *b, float *a)
{
    float v[4] __attribute__((aligned(16)));
    float x, y, angle;

    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(v), "r"(a), "r"(b) : "memory");
    __asm__ volatile ("mula.s %1,%4\n\tmadda.s %2,%5\n\tmadd.s %0,%3,%6" : "=f"(x) : "f"(axisX[0]), "f"(axisX[1]), "f"(axisX[2]), "f"(v[0]), "f"(v[1]), "f"(v[2]));
    __asm__ volatile ("mula.s %1,%4\n\tmadda.s %2,%5\n\tmadd.s %0,%3,%6" : "=f"(y) : "f"(axisY[0]), "f"(axisY[1]), "f"(axisY[2]), "f"(v[0]), "f"(v[1]), "f"(v[2]));
    angle = func_00248D90(x, y) * 57.29578f;
    if (angle < 0.0f) {
        angle += 360.0f;
    } else if (angle > 359.0f) {
        angle -= 360.0f;
    }
    return (int)angle;
}
