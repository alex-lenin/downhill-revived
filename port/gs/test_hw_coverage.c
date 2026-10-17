/* Standalone synthetic coverage / ROV test, not a game renderer.
   Run through port/tools/test_hw_coverage.py. Requires actual hardware ROV and
   conservative-raster support; unsupported exits3 and never counts as PASS.
   Sources consulted (no external implementation copied):
   https://learn.microsoft.com/en-us/windows/win32/direct3d11/conservative-rasterization
   https://learn.microsoft.com/en-us/windows/win32/direct3d11/rasterizer-order-views
   https://microsoft.github.io/hlsl-specs/proposals/0015-extended-command-info/
   ROV ordering applies to the same screen pixel/sample, not arbitrary VRAM
   aliasing between different screen pixels. Existing compute remains oracle. */
#define COBJMACROS
#include <windows.h>
#include <initguid.h>
#include <d3d11_3.h>
#include <d3dcompiler.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum { WIDTH=64, HEIGHT=64, PIXELS=WIDTH*HEIGHT, MAX_PRIMITIVES=512 };
typedef struct { int32_t a[2],b[2],c[2]; } Primitive;
typedef struct { uint32_t hash,count; } Pixel;
static const wchar_t *shader_path;
static Primitive primitive[MAX_PRIMITIVES];
static unsigned primitive_count, fixed_count, errors, checks;
static uint64_t verified_words, accepted_pixels, overlapping_pixels;
static ID3D11Device *device;
static ID3D11DeviceContext *context;
static ID3D11Buffer *output,*readback,*input,*draw_base;
static ID3D11UnorderedAccessView *uav;
static ID3D11ShaderResourceView *srv;
static ID3D11VertexShader *vs;
static ID3D11PixelShader *ps;
static ID3D11RasterizerState2 *rasterizer;
static void require(HRESULT hr,const char *operation) {
    if(FAILED(hr)) { fprintf(stderr,"ERROR %s HRESULT=%08lX\n",operation,(unsigned long)hr); exit(2); }
}
static int sign64(int64_t x) { return x>0?1:x<0?-1:0; }
static int64_t edge(const int32_t a[2],const int32_t b[2],const int32_t p[2]) {
    return (int64_t)(b[0]-a[0])*(p[1]-a[1])-(int64_t)(b[1]-a[1])*(p[0]-a[0]);
}
static int top_left(const int32_t a[2],const int32_t b[2]) { return b[1]<a[1] || (b[1]==a[1] && b[0]>a[0]); }
static int inside_edge(const int32_t a[2],const int32_t b[2],const int32_t p[2],int orientation) {
    int sign=sign64(edge(a,b,p))*orientation;
    return sign>0 || (!sign && (orientation>0?top_left(a,b):top_left(b,a)));
}
static int covered(const Primitive *p,unsigned x,unsigned y) {
    int32_t point[2]={(int)x*16,(int)y*16}; int orientation=sign64(edge(p->a,p->b,p->c));
    return orientation && inside_edge(p->a,p->b,point,orientation) && inside_edge(p->b,p->c,point,orientation) && inside_edge(p->c,p->a,point,orientation);
}
static void add(int ax,int ay,int bx,int by,int cx,int cy) {
    if(primitive_count>=MAX_PRIMITIVES) abort();
    primitive[primitive_count++]=(Primitive){{ax,ay},{bx,by},{cx,cy}};
}
static uint32_t random_state=0xDD60F00Du;
static uint32_t random_word(void) { random_state=random_state*1664525u+1013904223u;return random_state; }
static void fixtures(void) {
    /* Shared diagonal/axis edges; integer and subpixel positions; both windings;
       negative coordinates and clipped geometry; thin/sliver/degenerate cases. */
    add(128,128,896,128,896,896); add(128,128,896,896,128,896);
    add(128,128,896,896,896,128); add(128,128,128,896,896,896);
    add(-256,-256,1024,-256,-256,1024); add(-1,-1,1025,-1,-1,1025);
    add(1,1,1023,1,1,1023); add(7,9,999,31,33,1007);
    add(0,15,1024,15,512,17); add(15,0,17,512,15,1024);
    add(0,0,1024,1,1024,2); add(0,0,1,1024,2,1024);
    add(512,512,512,512,512,512); add(0,0,512,512,1024,1024);
    add(512,0,512,1024,512,100); add(0,512,1024,512,100,512);
    add(-32768,-32768,32767,-32768,0,32767);
    add(-32768,-32768,0,32767,32767,-32768);
    add(-32768,512,32767,511,32767,513);
    add(511,-32768,513,32767,512,32767);
    add(0,0,16,0,0,16); add(0,0,0,16,16,0);
    add(16,16,32,16,16,32); add(16,16,16,32,32,16);
    for(unsigned shift=0;shift<16;shift++) {
        add(shift,shift,1024+shift,shift,shift,1024+shift);
        add(shift,shift,shift,1024+shift,1024+shift,shift);
    }
    fixed_count=primitive_count;
    for(unsigned i=0;i<256;i++) {
        int coordinate[6];
        for(unsigned j=0;j<6;j++) coordinate[j]=(int)(random_word()%2049)-512;
        add(coordinate[0],coordinate[1],coordinate[2],coordinate[3],coordinate[4],coordinate[5]);
    }
    /* Heavy order pressure: identical full viewport coverage, alternating winding. */
    for(unsigned i=0;i<64;i++) {
        if(i&1)add(-2048,-2048,-2048,4096,4096,-2048);
        else add(-2048,-2048,4096,-2048,-2048,4096);
    }
}
static ID3DBlob *compile(const char *entry,const char *target,int pixel) {
    ID3DBlob *code=NULL,*error=NULL;
    D3D_SHADER_MACRO definitions[]={{"PIXEL_SHADER",pixel?"1":NULL},{NULL,NULL}};
    HRESULT hr=D3DCompileFromFile(shader_path,pixel?definitions:NULL,NULL,entry,target,
        D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_IEEE_STRICTNESS,0,&code,&error);
    if(error) { fprintf(stderr,"%s",(const char *)ID3D10Blob_GetBufferPointer(error));ID3D10Blob_Release(error); }
    require(hr,entry);
    return code;
}
static void initialize(void) {
    D3D_FEATURE_LEVEL level,requested[]={D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0,D3D_FEATURE_LEVEL_11_1};
    HRESULT hr=D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,0,requested,3,D3D11_SDK_VERSION,&device,&level,&context);
    /* Older runtimes may reject the12.x feature-level entries outright. */
    if(hr==E_INVALIDARG) hr=D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,0,&requested[2],1,D3D11_SDK_VERSION,&device,&level,&context);
    if(FAILED(hr)) { fprintf(stderr,"UNSUPPORTED hardware FL11_1+ device HRESULT=%08lX\n",(unsigned long)hr);exit(3); }
    D3D11_FEATURE_DATA_D3D11_OPTIONS2 options={0};
    hr=ID3D11Device_CheckFeatureSupport(device,D3D11_FEATURE_D3D11_OPTIONS2,&options,sizeof options);
    if(FAILED(hr)) { fprintf(stderr,"UNSUPPORTED OPTIONS2 query HRESULT=%08lX\n",(unsigned long)hr);exit(3); }
    printf("feature_level=%X ROVs=%u conservative_tier=%u\n",level,options.ROVsSupported,options.ConservativeRasterizationTier);
    if(!options.ROVsSupported || !options.ConservativeRasterizationTier) { fprintf(stderr,"UNSUPPORTED required ROV/conservative-raster capabilities\n");exit(3); }
    IDXGIDevice *dx=NULL;IDXGIAdapter *adapter=NULL;DXGI_ADAPTER_DESC ad;
    require(ID3D11Device_QueryInterface(device,&IID_IDXGIDevice,(void **)&dx),"DXGI device");
    require(IDXGIDevice_GetAdapter(dx,&adapter),"Adapter");require(IDXGIAdapter_GetDesc(adapter,&ad),"Adapter description");
    printf("adapter=%ls vendor=%04X device=%04X\n",ad.Description,ad.VendorId,ad.DeviceId);
    IDXGIAdapter_Release(adapter);IDXGIDevice_Release(dx);
    ID3D11Device3 *device3=NULL;
    hr=ID3D11Device_QueryInterface(device,&IID_ID3D11Device3,(void **)&device3);
    if(FAILED(hr)) { fprintf(stderr,"UNSUPPORTED Device3 interface HRESULT=%08lX\n",(unsigned long)hr);exit(3); }
    D3D11_RASTERIZER_DESC2 rd={0};rd.FillMode=D3D11_FILL_SOLID;rd.CullMode=D3D11_CULL_NONE;
    rd.DepthClipEnable=TRUE;rd.ScissorEnable=TRUE;rd.ForcedSampleCount=1;rd.ConservativeRaster=D3D11_CONSERVATIVE_RASTERIZATION_MODE_ON;
    require(ID3D11Device3_CreateRasterizerState2(device3,&rd,&rasterizer),"Conservative rasterizer");ID3D11Device3_Release(device3);
    ID3DBlob *blob=compile("vertex_main","vs_5_0",0);
    require(ID3D11Device_CreateVertexShader(device,ID3D10Blob_GetBufferPointer(blob),ID3D10Blob_GetBufferSize(blob),NULL,&vs),"Vertex shader");ID3D10Blob_Release(blob);
    blob=compile("pixel_main","ps_5_0",1);
    require(ID3D11Device_CreatePixelShader(device,ID3D10Blob_GetBufferPointer(blob),ID3D10Blob_GetBufferSize(blob),NULL,&ps),"ROV pixel shader");ID3D10Blob_Release(blob);
    D3D11_BUFFER_DESC bd={0};bd.ByteWidth=PIXELS*sizeof(Pixel);bd.Usage=D3D11_USAGE_DEFAULT;bd.BindFlags=D3D11_BIND_UNORDERED_ACCESS;bd.MiscFlags=D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;bd.StructureByteStride=sizeof(Pixel);
    require(ID3D11Device_CreateBuffer(device,&bd,NULL,&output),"Output buffer");
    D3D11_UNORDERED_ACCESS_VIEW_DESC ud={0};ud.ViewDimension=D3D11_UAV_DIMENSION_BUFFER;ud.Buffer.NumElements=PIXELS;
    require(ID3D11Device_CreateUnorderedAccessView(device,(ID3D11Resource *)output,&ud,&uav),"ROV UAV");
    bd.Usage=D3D11_USAGE_STAGING;bd.BindFlags=0;bd.CPUAccessFlags=D3D11_CPU_ACCESS_READ;bd.MiscFlags=0;bd.StructureByteStride=0;
    require(ID3D11Device_CreateBuffer(device,&bd,NULL,&readback),"Full readback buffer");
    bd.ByteWidth=primitive_count*sizeof(Primitive);bd.Usage=D3D11_USAGE_IMMUTABLE;bd.BindFlags=D3D11_BIND_SHADER_RESOURCE;bd.CPUAccessFlags=0;bd.MiscFlags=D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;bd.StructureByteStride=sizeof(Primitive);
    D3D11_SUBRESOURCE_DATA data={primitive,0,0};require(ID3D11Device_CreateBuffer(device,&bd,&data,&input),"Primitive buffer");
    D3D11_SHADER_RESOURCE_VIEW_DESC sd={0};sd.ViewDimension=D3D11_SRV_DIMENSION_BUFFER;sd.Buffer.NumElements=primitive_count;
    require(ID3D11Device_CreateShaderResourceView(device,(ID3D11Resource *)input,&sd,&srv),"Primitive SRV");
    D3D11_BUFFER_DESC cb={0};cb.ByteWidth=16;cb.Usage=D3D11_USAGE_DEFAULT;cb.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
    require(ID3D11Device_CreateBuffer(device,&cb,NULL,&draw_base),"Draw base constant buffer");
    ID3D11DeviceContext_VSSetConstantBuffers(context,0,1,&draw_base);
    D3D11_VIEWPORT viewport={0,0,WIDTH,HEIGHT,0,1};ID3D11DeviceContext_RSSetViewports(context,1,&viewport);
    ID3D11DeviceContext_RSSetState(context,(ID3D11RasterizerState *)rasterizer);
    ID3D11DeviceContext_IASetPrimitiveTopology(context,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_VSSetShader(context,vs,NULL,0);ID3D11DeviceContext_PSSetShader(context,ps,NULL,0);
    ID3D11DeviceContext_VSSetShaderResources(context,0,1,&srv);ID3D11DeviceContext_PSSetShaderResources(context,0,1,&srv);
}
static void submit(unsigned first,unsigned count) {
    uint32_t data[4]={first,0,0,0};
    ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)draw_base,0,NULL,data,0,0);
    ID3D11DeviceContext_Draw(context,count*3,0);
}
static void verify(unsigned first,unsigned count,D3D11_RECT scissor,int separate_draws) {
    Pixel expected[PIXELS]={{0}},zero[PIXELS]={{0}};
    for(unsigned index=first;index<first+count;index++) {
        const Primitive *p=&primitive[index];
        for(unsigned y=0;y<HEIGHT;y++) for(unsigned x=0;x<WIDTH;x++) {
            if((int)x<scissor.left || (int)x>=scissor.right || (int)y<scissor.top || (int)y>=scissor.bottom || !covered(p,x,y))continue;
            Pixel *pixel=&expected[y*WIDTH+x];pixel->hash=pixel->hash*1664525u+index+1013904223u;pixel->count++;
        }
    }
    ID3D11DeviceContext_UpdateSubresource(context,(ID3D11Resource *)output,0,NULL,zero,0,0);
    ID3D11DeviceContext_RSSetScissorRects(context,1,&scissor);
    ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews(context,0,NULL,NULL,0,1,&uav,NULL);
    if(separate_draws)for(unsigned i=first;i<first+count;i++)submit(i,1);
    else submit(first,count);
    ID3D11UnorderedAccessView *none=NULL;
    ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews(context,0,NULL,NULL,0,1,&none,NULL);
    ID3D11DeviceContext_CopyResource(context,(ID3D11Resource *)readback,(ID3D11Resource *)output);
    D3D11_MAPPED_SUBRESOURCE map;require(ID3D11DeviceContext_Map(context,(ID3D11Resource *)readback,0,D3D11_MAP_READ,0,&map),"Full verification Map");
    const Pixel *actual=map.pData;unsigned mismatches=0;
    for(unsigned i=0;i<PIXELS;i++) {
        verified_words+=2;
        accepted_pixels+=expected[i].count>0;overlapping_pixels+=expected[i].count>1;
        if(actual[i].hash!=expected[i].hash || actual[i].count!=expected[i].count) {
            if(mismatches<4)fprintf(stderr,"MISMATCH first=%u count=%u separate=%d scissor=%ld,%ld,%ld,%ld pixel=%u,%u expected=%08X/%u actual=%08X/%u\n",first,count,separate_draws,scissor.left,scissor.top,scissor.right,scissor.bottom,i%WIDTH,i/WIDTH,expected[i].hash,expected[i].count,actual[i].hash,actual[i].count);
            mismatches++;
        }
    }
    ID3D11DeviceContext_Unmap(context,(ID3D11Resource *)readback,0);checks++;
    if(mismatches) { errors++;fprintf(stderr,"FAILED check mismatching_pixels=%u\n",mismatches); }
}
int wmain(int argc,wchar_t **argv) {
    if(argc!=2) { fprintf(stderr,"Usage: test_hw_coverage.exe ABSOLUTE_SHADER_PATH\n");return 2; }
    shader_path=argv[1];
    fixtures();initialize();
    const D3D11_RECT scissors[]={{0,0,64,64},{8,9,57,51},{0,16,64,17},{31,0,32,64}};
    for(unsigned clip=0;clip<sizeof scissors/sizeof *scissors;clip++) {
        for(unsigned i=0;i<primitive_count;i++)verify(i,1,scissors[clip],0);
        /* Shared diagonal pair must cover a square exactly once per pixel. */
        verify(0,2,scissors[clip],0);
        /* Same draw heavy overlap and order; then separate draw submission order. */
        verify(0,primitive_count,scissors[clip],0);
        verify(0,primitive_count,scissors[clip],1);
    }
    printf("primitives=%u fixed=%u randomized=256 repeated_overlap=64 checks=%u failed_checks=%u verified_words=%llu accepted_pixels=%llu overlapping_pixels=%llu\n",primitive_count,fixed_count,checks,errors,verified_words,accepted_pixels,overlapping_pixels);
    puts("Scope: exact synthetic integer coverage + same-pixel ROV order only; not depth/blend/texture/game equivalence or cross-pixel alias ordering.");
    ID3D11DeviceContext_ClearState(context);
    ID3D11RasterizerState2_Release(rasterizer);ID3D11PixelShader_Release(ps);ID3D11VertexShader_Release(vs);
    ID3D11ShaderResourceView_Release(srv);ID3D11UnorderedAccessView_Release(uav);ID3D11Buffer_Release(draw_base);ID3D11Buffer_Release(input);ID3D11Buffer_Release(output);ID3D11Buffer_Release(readback);
    ID3D11DeviceContext_Release(context);ID3D11Device_Release(device);
    puts(errors?"RESULT=FAIL":"RESULT=PASS");
    return errors?1:0;
}
