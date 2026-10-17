/* UNIT 0x00232B08 0x00232B60 */
void func_00232B08(void *unused, float *matrix, float *vector) {
    matrix[0] = matrix[5] = matrix[10] = 0.0f;
    matrix[1] = -vector[2];
    matrix[4] = vector[2];
    matrix[2] = vector[1];
    matrix[8] = -vector[1];
    matrix[6] = -vector[0];
    matrix[9] = vector[0];
}
