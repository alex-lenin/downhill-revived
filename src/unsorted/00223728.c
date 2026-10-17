/* UNIT 0x00223728 0x00223778 */
float func_00223728(float x)
{
    float neg;

    if (x > 0.0f) {
        return (x - x * x) * 4.0f;
    }
    neg = -x;
    return -((neg - neg * neg) * 4.0f);
}
