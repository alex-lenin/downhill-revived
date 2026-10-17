/* UNIT 0x001A38F8 0x001A39D8 */
/* Writes into order[] the indices of values[] sorted ascending (selection sort). */
void func_001A38F8(void *unused, int *values, int *order, int count)
{
    char used[count];
    int i;
    int j;
    int min;
    int best;

    best = 0;
    for (i = 0; i < count; i++) {
        used[i] = 0;
    }
    for (i = 0; i < count; i++) {
        min = 0x0FFFFFFF;
        for (j = 0; j < count; j++) {
            if (values[j] < min && !used[j]) {
                min = values[j];
                best = j;
            }
        }
        order[i] = best;
        used[best] = 1;
    }
}
