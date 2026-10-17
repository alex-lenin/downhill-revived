/* UNIT 0x00230A70 0x00230BB0 */
struct Obj {
 char pad[0x8094]; float x8094;
 virtual int serialize(char *);
 virtual int deserialize(char *);
 virtual void reset();
 void func_00230A70(char *);
};
void Obj::func_00230A70(char *data) {
 int offset=12;
 int *record;
 float version;
 if(*(float *)data==1.2630964e9f) {
 
 if(*(float *)(data+4)>=1.0f && *(float *)(data+4)<2.0f) reset();
 else if(*(float *)(data+4)==2.0f) {
 record=(int *)(data+12);
 while(*record!=1) {
 deserialize(data+offset);
 offset+=record[1];
 record=(int *)(data+offset);
 }
 }
 }
 x8094=*(float *)(data+4);
}
