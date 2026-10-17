/* UNIT 0x0017D520 0x0017D580 */
typedef struct Entry { char x0[4]; short x4; char x6[18]; } Entry;
extern int D_0029E4B4;
extern Entry D_003B7D98[];
Entry *func_0017D520(int id) {
 int i;
 for(i=0;i<D_0029E4B4;i++) { Entry *p=&D_003B7D98[i]; if(p->x4==id) return p; }
 return &D_003B7D98[D_0029E4B4++];
}
