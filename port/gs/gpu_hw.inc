/* Windows hardware coverage bridge. Shared VRAM and draw_pixel remain the
   authority. ROV ordering protects same screen pixels only: reject every
   state with cross-pixel or frame/depth aliases before choosing this path. */
static void trace_dispatch(Gpu *p, int end);

void gs_gpu_get_raster_stats(const Gs *g, GsGpuRasterStats *out)
{
    memset(out, 0, sizeof *out);
    if (g && g->backend) *out = ((const Gpu *)g->backend)->raster_stats;
}

static unsigned hardware_state_rejection(Gs *g, unsigned ctxt)
{
    uint64_t frame = g->reg[GS_FRAME_1 + ctxt], z = g->reg[GS_ZBUF_1 + ctxt];
    uint64_t scissor = g->reg[GS_SCISSOR_1 + ctxt];
    unsigned psm = (unsigned)(frame >> 24) & 63, zpsm = 48 | ((unsigned)(z >> 24) & 15);
    if ((psm != 0 && psm != 1) || (zpsm != 48 && zpsm != 49)) return GS_GPU_HW_FORMAT;
    unsigned bw = (unsigned)(frame >> 16) & 63;
    unsigned x = (unsigned)(scissor >> 16) & 2047, y = (unsigned)(scissor >> 48) & 2047;
    if (!bw || x >= bw * 64 || (y / 32 + 1) * bw > 512) return GS_GPU_HW_ALIAS;
    uint32_t color[16] = {0}, depth[16] = {0};
    gs_mark_pages(color, psm, ((unsigned)frame & 511)*32, bw, 0,0,x,y);
    gs_mark_pages(depth, zpsm, ((unsigned)z & 511)*32, bw, 0,0,x,y);
    for (unsigned i = 0; i < 16; i++) if (color[i] & depth[i]) return GS_GPU_HW_ALIAS;
    return GS_GPU_HW_REASON_COUNT;
}

static unsigned hardware_rejection(Gpu *p)
{
    if (!p->raster_stats.hardware_enabled) return GS_GPU_HW_DISABLED;
    if (p->clip_on) return GS_GPU_HW_DISABLED;  /* clipped probe work is small: compute */
    if (!p->raster_stats.hardware_available) return GS_GPU_HW_UNAVAILABLE;
    if (p->hardware_rejection != GS_GPU_HW_REASON_COUNT) return p->hardware_rejection;
    for (unsigned i = 0; i < p->pending_count; i++) {
        const Primitive *d = &p->pending[i];
        if (d->info[0] < 3 || d->info[0] > 5 || d->info[2] != 3) return GS_GPU_HW_PRIMITIVE;
        int32_t x[3], y[3];
        for (unsigned j = 0; j < 3; j++) {
            x[j] = (int32_t)d->position[j][0]; y[j] = (int32_t)d->position[j][1];
            if (x[j] < -65535 || x[j] > 65535 || y[j] < -65535 || y[j] > 65535) return GS_GPU_HW_COORDINATE;
            for (unsigned k = 0; k < j; k++)
                if (llabs((long long)x[j]-x[k]) > 65535 || llabs((long long)y[j]-y[k]) > 65535) return GS_GPU_HW_COORDINATE;
        }
        int64_t area = (int64_t)(x[1]-x[0])*(y[2]-y[0]) - (int64_t)(y[1]-y[0])*(x[2]-x[0]);
        if (!area) return GS_GPU_HW_DEGENERATE;
    }
    return GS_GPU_HW_REASON_COUNT;
}

static void hardware_submit(Gpu *p, ID3D11ShaderResourceView **views)
{
    ID3D11DeviceContext *c = p->context;
    ID3D11ShaderResourceView *none[3] = {NULL,NULL,NULL};
    ID3D11UnorderedAccessView *no_uav = NULL;
    /* Explicitly clear both domains on every transition. No decoded caches or
       duplicate render targets exist; transfers still operate on this VRAM. */
    ID3D11DeviceContext_CSSetUnorderedAccessViews(c,0,1,&no_uav,NULL);
    ID3D11DeviceContext_CSSetShaderResources(c,0,3,none);
    ID3D11DeviceContext_IASetInputLayout(c,NULL);
    ID3D11DeviceContext_IASetPrimitiveTopology(c,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_VSSetShader(c,p->hardware_vertex,NULL,0);
    ID3D11DeviceContext_GSSetShader(c,NULL,NULL,0);
    ID3D11DeviceContext_PSSetShader(c,p->hardware_pixel,NULL,0);
    ID3D11DeviceContext_VSSetShaderResources(c,1,1,&p->primitive_srv);
    ID3D11DeviceContext_PSSetShaderResources(c,0,3,views);
    ID3D11DeviceContext_PSSetConstantBuffers(c,0,1,&p->constants);
    ID3D11DeviceContext_RSSetState(c,(ID3D11RasterizerState *)p->hardware_rasterizer);
    D3D11_VIEWPORT viewport = {0,0,2048,2048,0,1};
    D3D11_RECT scissor = {0,0,2048,2048};
    ID3D11DeviceContext_RSSetViewports(c,1,&viewport);
    ID3D11DeviceContext_RSSetScissorRects(c,1,&scissor);
    ID3D11DeviceContext_OMSetDepthStencilState(c,p->hardware_depth,0);
    ID3D11DeviceContext_OMSetBlendState(c,NULL,NULL,~0u);
    ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews(c,0,NULL,NULL,0,1,&p->uav,NULL);
    p->trace_dispatch_hardware = 1;
    trace_dispatch(p,0);
    ID3D11DeviceContext_Draw(c,p->pending_count*3,0);
    trace_dispatch(p,1);
    if (p->trace) { p->trace_primitives += p->pending_count; p->trace_raster_count++; }
    p->raster_stats.hardware_batches++;
    p->raster_stats.hardware_primitives += p->pending_count;
    ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews(c,0,NULL,NULL,0,1,&no_uav,NULL);
    ID3D11DeviceContext_PSSetShaderResources(c,0,3,none);
    ID3D11DeviceContext_VSSetShaderResources(c,0,3,none);
    ID3D11DeviceContext_PSSetShader(c,NULL,NULL,0);
    ID3D11DeviceContext_VSSetShader(c,NULL,NULL,0);
}

static void hardware_init(Gpu *p, const D3D_SHADER_MACRO *raster_defines)
{
    const char *option = getenv("DHD_GS_HARDWARE");
    p->raster_stats.hardware_enabled = !option || atoi(option);
    if (!p->raster_stats.hardware_enabled) return;
    D3D11_FEATURE_DATA_D3D11_OPTIONS2 caps = {0};
    ID3D11Device3 *d3 = NULL;
    ID3DBlob *code = NULL, *errors = NULL;
    /* Local IID avoids adding a dxguid link dependency to existing builds. */
    static const GUID device3_iid = {0xa05c8c37,0xd2c6,0x4732,{0xb3,0xa0,0x9c,0xe0,0xb0,0xdc,0x9a,0xe6}};
    if (FAILED(ID3D11Device_CheckFeatureSupport(p->device,D3D11_FEATURE_D3D11_OPTIONS2,&caps,sizeof caps)) ||
        !caps.ROVsSupported || caps.ConservativeRasterizationTier < D3D11_CONSERVATIVE_RASTERIZATION_TIER_3 ||
        FAILED(ID3D11Device_QueryInterface(p->device,&device3_iid,(void **)&d3))) goto unavailable;
    D3D11_RASTERIZER_DESC2 rd = {0};
    rd.FillMode=D3D11_FILL_SOLID; rd.CullMode=D3D11_CULL_NONE;
    rd.DepthClipEnable=TRUE; rd.ScissorEnable=TRUE; rd.ForcedSampleCount=1;
    rd.ConservativeRaster=D3D11_CONSERVATIVE_RASTERIZATION_MODE_ON;
    HRESULT hr = ID3D11Device3_CreateRasterizerState2(d3,&rd,&p->hardware_rasterizer);
    ID3D11Device3_Release(d3); d3=NULL;
    if (FAILED(hr)) goto unavailable;
    D3D11_DEPTH_STENCIL_DESC depth = {0}; depth.DepthFunc=D3D11_COMPARISON_ALWAYS;
    if (FAILED(ID3D11Device_CreateDepthStencilState(p->device,&depth,&p->hardware_depth))) goto unavailable;
    D3D_SHADER_MACRO defines[5];
    memcpy(defines,raster_defines,3*sizeof *defines);
    defines[3].Name="DHD_HARDWARE_PIXEL"; defines[3].Definition="0";
    defines[4].Name=defines[4].Definition=NULL;
    hr=shader_compile(defines, "hardware_vertex", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_IEEE_STRICTNESS, &code, &errors);
    if (errors) { fprintf(stderr,"%s",(const char *)ID3D10Blob_GetBufferPointer(errors)); ID3D10Blob_Release(errors); errors=NULL; }
    if (FAILED(hr)) goto unavailable;
    hr=ID3D11Device_CreateVertexShader(p->device,ID3D10Blob_GetBufferPointer(code),ID3D10Blob_GetBufferSize(code),NULL,&p->hardware_vertex);
    ID3D10Blob_Release(code); code=NULL;
    if (FAILED(hr)) goto unavailable;
    defines[3].Definition="1";
    hr=shader_compile(defines, "hardware_pixel", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_IEEE_STRICTNESS, &code, &errors);
    if (errors) { fprintf(stderr,"%s",(const char *)ID3D10Blob_GetBufferPointer(errors)); ID3D10Blob_Release(errors); errors=NULL; }
    if (FAILED(hr)) goto unavailable;
    hr=ID3D11Device_CreatePixelShader(p->device,ID3D10Blob_GetBufferPointer(code),ID3D10Blob_GetBufferSize(code),NULL,&p->hardware_pixel);
    ID3D10Blob_Release(code); code=NULL;
    if (FAILED(hr)) goto unavailable;
    p->raster_stats.hardware_available=1;
    fprintf(stderr,"gs: hardware coverage bridge available (ROV, conservative tier 3)\n");
    return;
unavailable:
    if (d3) ID3D11Device3_Release(d3);
    if (code) ID3D10Blob_Release(code);
    fprintf(stderr,"gs: hardware coverage bridge unavailable; using reference compute\n");
}

