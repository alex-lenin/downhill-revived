/* UNIT 0x00221B80 0x00221BE0 */
void func_00221B80(float out[4][4], float a[4][4], float b[4][4]) {
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++) out[i][j]=a[i][j]+b[j][i];
}
