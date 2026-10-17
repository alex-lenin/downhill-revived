/* UNIT 0x0020FFD0 0x00210068 */
typedef struct Glyph { unsigned short width, height; char pad[28]; } Glyph;
extern Glyph D_004F3570[];
extern int D_0029E188;
void func_0020FFD0(int index, int character, int *x, int *y) {
 Glyph *glyph = &D_004F3570[index];
 int row, col;
 int ch = character & 255;
 if (D_0029E188 == 0) {
  if ((unsigned int)(ch - 'a') < 26) ch = (ch - 32) & 255;
  col = ch - 33;
  row = col >> 3;
  col -= row << 3;
  *x = glyph->width * col + 8;
  *y = glyph->height * row + 8;
 } else {
  row = ch >> 4;
  col = ch - (row << 4);
  *x = glyph->width * col + 16;
  *y = glyph->height * row + 16;
 }
}
