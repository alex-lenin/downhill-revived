/* UNIT 0x0021AC08 0x0021AE70 */
struct Size { int bytes,pad; };
struct Sizes {struct Size entries[15];};
struct Entry { unsigned short count; signed char width,height; int bytes; };
struct Obj { unsigned int count; int offset; struct Entry entries[15]; };
union Flags { unsigned long raw; struct { unsigned long low:40; unsigned int width:4; unsigned int height:4; unsigned int tail:11; unsigned int index:5; } bits; };
struct Tag { unsigned int next; int pad4; union Flags value; };
extern int func_0021AE70(struct Obj *,struct Tag *);
extern void func_00254D08(const char *,...);
extern char D_00298978[];
void func_0021AC08(struct Obj *obj) {
 int i=14;
 struct Entry *entry=obj->entries;
 struct Sizes *sizes=(struct Sizes *)&obj->entries[0].bytes;
 struct Tag *tag;
 unsigned long flags,type;
 int found,size;
 obj->count=0;
 tag=(struct Tag *)((char *)obj+obj->offset*16);
 do { entry->bytes=0; i--; entry->count=0; entry++; } while(i>=0);
 do {
 found=func_0021AE70(obj,tag);
 if(found==-1) {
 obj->entries[obj->count].width=tag->value.bits.width;
 obj->entries[obj->count].height=tag->value.bits.height;
 obj->entries[obj->count].count=1;
 { unsigned long idx=obj->count; unsigned long raw=tag->value.raw; flags=raw&(~0UL>>5); flags|=idx<<59; }
 size=(1<<((unsigned int)(flags>>40)&15))<<((unsigned int)(flags>>44)&15);
 type=flags&((unsigned long)0xfc00<<22);
 tag->value.raw=flags;
 if(type==0) {
 sizes->entries[obj->count].bytes=size*4;
 } else if(type==((unsigned long)0x9800<<21) || type==((unsigned long)0xa000<<21)) {
 if((flags&((unsigned long)0xff3f<<32))==((unsigned long)0x9913<<32)) {
 sizes->entries[obj->count].bytes=0x40000;
 obj->entries[obj->count].width=9;
 obj->entries[obj->count].height=9;
 } else {
 sizes->entries[obj->count].bytes=0x100;
 obj->entries[obj->count].width=3;
 obj->entries[obj->count].height=3;
 }
 } else {func_00254D08(D_00298978,(int)((long)flags>>32)&63);}
 obj->count++;
 } else {
 obj->entries[found].count++;
 tag->value.raw=(tag->value.raw&(~0UL>>5))|((unsigned long)found<<59);
 }
 { struct Tag *old=tag; tag=(struct Tag *)((char *)tag+(tag->next>>2)*16); if(!old->next) break; }
 } while(1);
}
