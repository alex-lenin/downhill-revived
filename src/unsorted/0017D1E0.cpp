/* UNIT 0x0017D1E0 0x0017D388 */
class Part { public: int x0; virtual int v1(); virtual int v2(); virtual int v3(); virtual int test(); };
struct Obj { char pad0[0x1E40]; char x1E40[0x4000];
 Part x5E40;
 char pad5E48[0x28];
 Part x5E70;
 char pad5E78[0x18];
 Part x5E90;
 char pad5E98[0x158];
 Part x5FF0;
 char pad5FF8[0x218];
 Part x6210;
 char pad6218[0x4];
 Part x621C;
 char pad6224[0x24];
 Part x6248;
 char pad6250[0x48];
 Part x6298;
 char pad62A0[0x14];
 Part x62B4;
 char pad62BC[0x24];
 Part x62E0;
 char pad62E8[0x28];
 Part x6310;
};
struct State { Obj *x0; };
extern void func_001AC578(void *);
extern void func_0012FE38(void *,Part *);
void func_0017D1E0(State *state) {
 Obj *obj=state->x0;
 func_001AC578(obj->x1E40);
 { Part *part0=&obj->x5E40;
 if(part0->test()) { func_0012FE38(obj,part0); } else
 { Part *part1=&obj->x5E70;
 if(part1->test()) { func_0012FE38(obj,part1); } else
 { Part *part2=&obj->x5E90;
 if(part2->test()) { func_0012FE38(obj,part2); } else
 { Part *part3=&obj->x5FF0;
 if(part3->test()) { func_0012FE38(obj,part3); } else
 { Part *part4=&obj->x6210;
 if(part4->test()) { func_0012FE38(obj,part4); } else
 { Part *part5=&obj->x621C;
 if(part5->test()) { func_0012FE38(obj,part5); } else
 { Part *part6=&obj->x6298;
 if(part6->test()) { func_0012FE38(obj,part6); } else
 { Part *part7=&obj->x6248;
 if(part7->test()) { func_0012FE38(obj,part7); } else
 { Part *part8=&obj->x62B4;
 if(part8->test()) { func_0012FE38(obj,part8); } else
 { Part *part9=&obj->x62E0;
 if(part9->test()) { func_0012FE38(obj,part9); } else
 { Part *part10=&obj->x6310;
 if(part10->test()) { func_0012FE38(obj,part10); } else
 {}
 }
 }
 }
 }
 }
 }
 }
 }
 }
 }
 }
}
