/* UNIT 0x00223778 0x002237F8 */
extern float func_00263B88(float);

/* Odd-symmetric ease curve built from func_00263B88:
 *   x > 0:  1 - (f(1 - x) + 1) / 2
 *   else:   (f(1 + x) + 1) / 2 - 1
 * Retail computes the else-branch argument as `one + x` (constant first). CSE puts a
 * known constant second, so `one` must be unknown to both CSE passes at that add. The
 * never-taken back jump to `again` splits the block for cse1/cse2: gcse turns k into 1,
 * then cse2 folds the jump away, leaving no code behind. */
float func_00223778(float x)
{
    float one;
    float r;
    int k, k0;

    if (!(x > 0.0f)) {
        one = 1.0f;
        k0 = 0;
    again:
        r = (func_00263B88(one + x) + one) * 0.5f - one;
        k = k0 + 1;
        if (k != 1) goto again;
        return r;
    } else {
        one = 1.0f;
        return one - (func_00263B88(one - x) + one) * 0.5f;
    }
}
