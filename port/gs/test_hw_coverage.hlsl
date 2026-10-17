struct Primitive { int2 a; int2 b; int2 c; };
StructuredBuffer<Primitive> primitives : register(t0);
cbuffer DrawBase : register(b0) { uint base_primitive; uint3 padding; };
struct Vertex { float4 position : SV_Position; nointerpolation uint primitive : TEXCOORD0; };
Vertex vertex_main(uint vertex : SV_VertexID) {
    uint index=base_primitive+vertex/3;
    Primitive p = primitives[index];
    int2 fixed_position = vertex % 3 == 0 ? p.a : vertex % 3 == 1 ? p.b : p.c;
    // GS samples integer pixel coordinates. Hardware samples x+.5,y+.5.
    // Exact binary conversions for12.4 positions and64x64 viewport.
    float2 screen = float2(fixed_position) * (1.0 / 16.0) + 0.5;
    Vertex v; v.position = float4(screen * float2(2.0/64.0,-2.0/64.0) + float2(-1,1), .5, 1);
    v.primitive = index; return v;
}
#ifdef PIXEL_SHADER
RasterizerOrderedStructuredBuffer<uint2> result : register(u0);
// Sign-only exact signed-magnitude cross product; coordinate differences fit
//16unsigned bits. No float rounding and no signed32-bit multiply overflow.
int edge_sign(int2 a, int2 b, int2 p) {
    int2 d = b-a, q = p-a;
    uint u = uint(abs(d.x)) * uint(abs(q.y));
    uint v = uint(abs(d.y)) * uint(abs(q.x));
    bool un = (d.x<0) != (q.y<0), vn = (d.y<0) != (q.x<0);
    if (un != vn) return (u==0 && v==0) ? 0 : un ? -1 : 1;
    int sign = u==v ? 0 : u>v ? 1 : -1;
    return un ? -sign : sign;
}
bool top_left(int2 a,int2 b) { int2 d=b-a; return d.y<0 || (d.y==0 && d.x>0); }
bool edge_inside(int2 a,int2 b,int2 p,int orientation) {
    int sign=edge_sign(a,b,p)*orientation;
    return sign>0 || (sign==0 && (orientation>0 ? top_left(a,b) : top_left(b,a)));
}
void pixel_main(Vertex input) {
    uint2 xy=uint2(input.position.xy);
    if(any(xy>=64)) return;
    Primitive p=primitives[input.primitive]; int2 sample_position=int2(xy)*16;
    int orientation=edge_sign(p.a,p.b,p.c);
    if (!orientation || !edge_inside(p.a,p.b,sample_position,orientation) ||
        !edge_inside(p.b,p.c,sample_position,orientation) || !edge_inside(p.c,p.a,sample_position,orientation)) return;
    uint address=xy.y*64+xy.x;
    uint2 old=result[address];
    // Noncommutative recurrence: detects reversed/reordered overlap, and count
    // verifies accepted coverage independently of the32-bit hash.
    result[address]=uint2(old.x*1664525u + input.primitive + 1013904223u, old.y+1u);
}
#endif

