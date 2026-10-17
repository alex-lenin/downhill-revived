/* UNIT 0x001C3178 0x001C32D0 */
typedef unsigned int U128 __attribute__((mode(TI)));
typedef union Packet {unsigned long halves[2]; int words[4]; U128 full;} Packet;
extern void func_0023D618(int,int*,int*);
extern void func_0023D650(int,int*,int*);
void func_001C3178(int unused,Packet *packet,int id,int **out){
 int x,y,width,height; int left,top; Packet *start;
 start=packet;
 packet++;
 packet->halves[0]=((unsigned long)0x8000<<45)|0x8001;
 packet->halves[1]=14;
 packet++;
 packet->halves[0]=((unsigned long)0x8000<<23)|0x44;
 packet->halves[1]=0x42;
 packet++;
 packet->halves[0]=((unsigned long)0xC08D<<46)|0x8001;
 packet->halves[1]=0x551;
 func_0023D618(id,&x,&y);
 packet++;
 func_0023D650(id,&width,&height);
 left=x-(width>>1); top=y-(height>>1);
 packet->words[0]=0;packet->words[1]=0;packet->words[2]=0;packet->words[3]=0;
 *out=&packet->words[3];
 packet++;
 packet->words[0]=left<<4;packet->words[1]=top<<4;packet->words[2]=0xFFFF28;packet->words[3]=0;
 packet++;
 packet->words[0]=(left+width)<<4;packet->words[1]=(top+height)<<4;packet->words[2]=0xFFFF28;packet->words[3]=0;
 start->full=0;
 start->words[0]=(((int)packet-(int)start)>>4)|0x60000000;
}
