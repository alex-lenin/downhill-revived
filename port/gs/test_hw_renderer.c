/* Independent full-VRAM verification of the opt-in hardware GS path.
   CPU coverage uses int64 cross products; simple color/Z cases use exact
   integer arithmetic, not the production HLSL. Rich shading additionally
   compares the retained compute backend. Every comparison checks the
   complete4MB image. */
#define COBJMACROS
#include "gpu_win32.h"
#include <d3d11_3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int x, y;
  uint32_t z, color;
  unsigned u, v, fog;
  float s, t, q;
} Vertex;
static Gs *reference, *hardware, *expected;
static ID3D11Device *device;
static ID3D11DeviceContext *context;
static uint32_t *seed, *raw_reference, *raw_hardware;
static unsigned failures, checks, positive_cases, fallback_cases;
static uint64_t compared_words;
static const char *case_name;
static void check(int condition, const char *what) {
  if (!condition) {
    fprintf(stderr, "FAIL %s: %s\n", case_name ? case_name : "initialization",
            what);
    failures++;
  }
}
static int64_t edge(const Vertex *a, const Vertex *b, int x, int y) {
  return (int64_t)(b->x - a->x) * (y - a->y) -
         (int64_t)(b->y - a->y) * (x - a->x);
}
static int top_left(const Vertex *a, const Vertex *b) {
  return b->y < a->y || (b->y == a->y && b->x > a->x);
}
static int inside(int64_t value, const Vertex *a, const Vertex *b, int sign) {
  return value * sign > 0 ||
         (!value && (sign > 0 ? top_left(a, b) : top_left(b, a)));
}
static int floor_div(int value, int denominator) {
  return value >= 0 ? value / denominator
                    : -((-value + denominator - 1) / denominator);
}
static void put(unsigned reg, uint64_t value) {
  gs_write(reference, reg, value);
  gs_write(hardware, reg, value);
  gs_write(expected, reg, value);
}
static void configure(unsigned fp, unsigned zp, unsigned ztest, uint32_t mask,
                      unsigned zmask) {
  put(GS_PRMODECONT, 1);
  put(GS_COLCLAMP, 1);
  put(GS_FRAME_1, 1u << 16 | (uint64_t)fp << 24 | (uint64_t)mask << 32);
  put(GS_ZBUF_1, 64 | (uint64_t)(zp & 15) << 24 | (uint64_t)zmask << 32);
  put(GS_SCISSOR_1, UINT64_C(63) << 16 | UINT64_C(63) << 48);
  put(GS_XYOFFSET_1, 4096 | UINT64_C(4096) << 32);
  put(GS_TEST_1, 1u << 16 | ztest << 17);
  put(GS_ALPHA_1, 1u << 2 | 1u << 6);
}
static void reset_case(const char *name) {
  case_name = name;
  reference->sync(reference);
  hardware->sync(hardware);
  Gs *all[] = {reference, hardware, expected};
  for (unsigned i = 0; i < 3; i++) {
    Gs *g = all[i];
    memcpy(g->vram, seed, GS_VRAM_SIZE);
    memset(g->reg, 0, sizeof g->reg);
    memset(g->clut, 0, sizeof g->clut);
    g->vertex_count = g->strip_parity = 0;
    g->vertex.q = 1;
    g->pixels_written++;
    g->state_version++;
    memset(g->host_dirty, 255, sizeof g->host_dirty);
  }
  configure(0, 48, 1, 0, 0);
}
static void gpu_triangle(Gs *g, unsigned prim, const Vertex v[3]) {
  gs_write(g, GS_PRIM, prim);
  unsigned ox = (unsigned)g->reg[GS_XYOFFSET_1] & 65535,
           oy = (unsigned)(g->reg[GS_XYOFFSET_1] >> 32) & 65535;
  for (unsigned i = 0; i < ((prim & 7) == 6 ? 2u : 3u); i++) {
    float q = v[i].q ? v[i].q : 1.0f;
    uint32_t qb, sb, tb;
    memcpy(&qb, &q, 4);
    memcpy(&sb, &v[i].s, 4);
    memcpy(&tb, &v[i].t, 4);
    gs_write(g, GS_RGBAQ, v[i].color | (uint64_t)qb << 32);
    gs_write(g, GS_ST, sb | (uint64_t)tb << 32);
    gs_write(g, GS_UV, v[i].u | (uint64_t)v[i].v << 16);
    gs_write(g, GS_FOG, (uint64_t)v[i].fog << 56);
    check(v[i].x + (int)ox >= 0 && v[i].x + (int)ox <= 65535 &&
              v[i].y + (int)oy >= 0 && v[i].y + (int)oy <= 65535,
          "fixture raw coordinate range");
    gs_write(g, GS_XYZ2,
             (unsigned)(v[i].x + ox) | (uint64_t)(unsigned)(v[i].y + oy) << 16 |
                 (uint64_t)v[i].z << 32);
  }
}
static void cpu_triangle(unsigned prim, const Vertex v[3], int texture_override,
                         uint32_t texture_color) {
  int64_t area = edge(&v[0], &v[1], v[2].x, v[2].y);
  if (!area)
    return;
  int sign = area > 0 ? 1 : -1;
  uint64_t frame = expected->reg[GS_FRAME_1], zbuf = expected->reg[GS_ZBUF_1],
           sc = expected->reg[GS_SCISSOR_1], test = expected->reg[GS_TEST_1];
  unsigned fp = (unsigned)(frame >> 24) & 63,
           zp = 48 | ((unsigned)(zbuf >> 24) & 15),
           bw = (unsigned)(frame >> 16) & 63;
  unsigned fbp = ((unsigned)frame & 511) * 32,
           zbp = ((unsigned)zbuf & 511) * 32;
  unsigned x0 = sc & 2047, x1 = sc >> 16 & 2047, y0 = sc >> 32 & 2047,
           y1 = sc >> 48 & 2047;
  /* Fixtures with varying colors/Z use dyadic edge weights, making the exact
     rational oracle agree without depending on production float planes. */
  for (unsigned y = y0; y <= y1; y++)
    for (unsigned x = x0; x <= x1; x++) {
      int px = (int)x * 16, py = (int)y * 16;
      int64_t e[3] = {edge(&v[1], &v[2], px, py), edge(&v[2], &v[0], px, py),
                      edge(&v[0], &v[1], px, py)};
      if (!inside(e[0], &v[1], &v[2], sign) ||
          !inside(e[1], &v[2], &v[0], sign) ||
          !inside(e[2], &v[0], &v[1], sign))
        continue;
      uint32_t color = texture_override ? texture_color : v[2].color;
      if ((prim & 8) && !texture_override) {
        color = 0;
        for (unsigned c = 0; c < 4; c++) {
          int64_t n = 0;
          for (unsigned i = 0; i < 3; i++)
            n += (int64_t)(v[i].color >> (c * 8) & 255) * e[i];
          color |= (uint32_t)(n / area) << (c * 8);
        }
      }
      if (prim & 32) {
        int64_t n = 0;
        for (unsigned i = 0; i < 3; i++)
          n += (int64_t)v[i].fog * e[i];
        int fog = (int)(n / area);
        uint32_t out = color & 0xFF000000;
        for (unsigned c = 0; c < 3; c++) {
          int fc = expected->reg[GS_FOGCOL] >> (c * 8) & 255,
              src = color >> (c * 8) & 255;
          out |= (uint32_t)(fc + floor_div((src - fc) * fog, 256)) << (c * 8);
        }
        color = out;
      }
      int64_t zn = 0;
      for (unsigned i = 0; i < 3; i++)
        zn += (int64_t)v[i].z * e[i];
      uint64_t z = (uint64_t)(zn / area);
      uint32_t limit = zp == 49 ? 0xFFFFFF : zp == 48 ? UINT32_MAX : 0xFFFF;
      if (z > limit)
        z = limit;
      int write_color = 1, write_z = !(zbuf >> 32 & 1), rgb_only = 0;
      if (test & 1) {
        unsigned mode = test >> 1 & 7, ref = test >> 4 & 255, a = color >> 24;
        int pass = mode == 1 || (mode == 2 && a < ref) ||
                   (mode == 3 && a <= ref) || (mode == 4 && a == ref) ||
                   (mode == 5 && a >= ref) || (mode == 6 && a > ref) ||
                   (mode == 7 && a != ref);
        if (!pass) {
          unsigned fail = test >> 12 & 3;
          if (!fail)
            continue;
          write_color = fail != 2;
          write_z = write_z && fail == 2;
          rgb_only = fail == 3;
        }
      }
      uint32_t old = gs_read_pixel(expected, fp, fbp, bw, x, y),
               old_z = gs_read_pixel(expected, zp, zbp, bw, x, y);
      if (test & 0x10000) {
        unsigned zt = test >> 17 & 3;
        if (zt == 0 || (zt == 2 && z < old_z) || (zt == 3 && z <= old_z))
          continue;
      } else
        write_z = 0;
      if (write_color) {
        if (prim & 64) {
          uint32_t out = color & 0xFF000000;
          unsigned a = color >> 24;
          for (unsigned c = 0; c < 3; c++) {
            int src = color >> (c * 8) & 255, dst = old >> (c * 8) & 255;
            int result = floor_div((src - dst) * (int)a, 128) + dst;
            if (result < 0)
              result = 0;
            if (result > 255)
              result = 255;
            out |= (uint32_t)result << (c * 8);
          }
          color = out;
        }
        uint32_t mask = (uint32_t)(frame >> 32) | (rgb_only ? 0xFF000000 : 0);
        if (fp == 2 || fp == 10) {
          color = ((color >> 3) & 31) | ((color >> 6) & 0x3E0) |
                  ((color >> 9) & 0x7C00) | ((color >> 16) & 0x8000);
          mask = ((mask >> 3) & 31) | ((mask >> 6) & 0x3E0) |
                 ((mask >> 9) & 0x7C00) | ((mask >> 16) & 0x8000);
        }
        gs_write_pixel(expected, fp, fbp, bw, x, y,
                       (old & mask) | (color & ~mask));
      }
      if (write_z)
        gs_write_pixel(expected, zp, zbp, bw, x, y, (uint32_t)z);
    }
}
static void triangle(unsigned prim, const Vertex v[3], int oracle,
                     int texture_override, uint32_t texture_color) {
  gpu_triangle(reference, prim, v);
  gpu_triangle(hardware, prim, v);
  if (oracle)
    cpu_triangle(prim, v, texture_override, texture_color);
}
static void compare_vram(int oracle) {
  reference->sync(reference);
  hardware->sync(hardware);
  int ref_ok = gs_gpu_readback_all(reference, raw_reference),
      hw_ok = gs_gpu_readback_all(hardware, raw_hardware);
  check(ref_ok && hw_ok, "raw full GPU resource readback succeeded");
  if (!ref_ok || !hw_ok)
    return;
  unsigned diffs = 0;
  for (unsigned i = 0; i < GS_VRAM_SIZE / 4; i++) {
    compared_words++;
    if (raw_reference[i] != raw_hardware[i] ||
        (oracle && raw_hardware[i] != expected->vram[i]) ||
        raw_reference[i] != reference->vram[i] ||
        raw_hardware[i] != hardware->vram[i]) {
      if (diffs < 5)
        fprintf(stderr, "FAIL %s word=%X ref=%08X hw=%08X cpu=%08X\n",
                case_name, i, raw_reference[i], raw_hardware[i],
                expected->vram[i]);
      diffs++;
    }
  }
  check(!diffs, "complete VRAM comparison");
  checks++;
}
static GsGpuRasterStats before;
static void begin_stats(void) { gs_gpu_get_raster_stats(hardware, &before); }
static void end_stats(int should_hit) {
  GsGpuRasterStats after;
  gs_gpu_get_raster_stats(hardware, &after);
  if (should_hit) {
    check(after.hardware_primitives > before.hardware_primitives,
          "hardware actually submitted eligible primitives");
    positive_cases++;
  } else {
    check(after.hardware_primitives == before.hardware_primitives,
          "unsupported case did not submit hardware");
    check(after.reference_batches > before.reference_batches,
          "fallback actually submitted compute batch");
    fallback_cases++;
  }
}
static Vertex base_triangle[3] = {{0, 0, 100, 0x80402010, 0, 0, 128},
                                  {512, 0, 100, 0x80806050, 0, 0, 128},
                                  {0, 512, 100, 0x80C0A090, 0, 0, 128}};
static void flat_depth_matrix(void) {
  for (unsigned fp = 0; fp < 2; fp++)
    for (unsigned zp = 48; zp <= 49; zp++)
      for (unsigned ztest = 0; ztest < 4; ztest++) {
        reset_case("flat depth/mask matrix");
        configure(fp, zp, ztest, fp ? 0x005500AA : 0x55AA00FF, 0);
        begin_stats();
        Vertex v[3];
        memcpy(v, base_triangle, sizeof v);
        for (unsigned i = 0; i < 3; i++)
          v[i].z = ztest & 1 ? 0xFFFFFFFF : 0x7FFFFFFF;
        triangle(3, v, 1, 0, 0);
        compare_vram(1);
        end_stats(1);
      }
  for (unsigned shift = 0; shift < 16; shift++)
    for (unsigned winding = 0; winding < 2; winding++) {
      reset_case("subpixel/negative/shared-edge coverage");
      begin_stats();
      Vertex v[3];
      memcpy(v, base_triangle, sizeof v);
      for (unsigned i = 0; i < 3; i++) {
        v[i].x += (int)shift - 32;
        v[i].y += (int)shift - 16;
      }
      if (winding) {
        Vertex swap = v[1];
        v[1] = v[2];
        v[2] = swap;
      }
      triangle(3, v, 1, 0, 0);
      compare_vram(1);
      end_stats(1);
    }
}
static void shading_order(void) {
  reset_case("dyadic IIP/Z/fog");
  configure(1, 49, 1, 0, 0);
  put(GS_FOGCOL, 0xA05028);
  begin_stats();
  Vertex v[3];
  memcpy(v, base_triangle, sizeof v);
  v[0].z = 0xFFFF00;
  v[1].z = 0xFFFF80;
  v[2].z = 0xFFFFC0;
  v[0].fog = 0;
  v[1].fog = 128;
  v[2].fog = 256 - 1;
  /* Constant fog keeps exact integer expected interpolation independent of
     truncation at float rounding boundaries. Varying fog remains ref-vs-HW. */
  for (unsigned i = 0; i < 3; i++)
    v[i].fog = 128;
  triangle(3 | 8 | 32, v, 1, 0, 0);
  compare_vram(1);
  end_stats(1);
  reset_case("ordered alpha blend and equal Z");
  configure(0, 48, 1, 0, 0);
  begin_stats();
  for (unsigned n = 0; n < 64; n++) {
    memcpy(v, base_triangle, sizeof v);
    for (unsigned i = 0; i < 3; i++) {
      v[i].color = 0x40000000 | ((n * 0x030507u) & 0xFFFFFF);
      v[i].z = 100;
    }
    triangle(3 | 64, v, 1, 0, 0);
  }
  compare_vram(1);
  end_stats(1);
  for (unsigned action = 0; action < 4; action++) {
    reset_case("alpha NEVER failure action");
    put(GS_TEST_1, 1 | action << 12 | 1u << 16 | 1u << 17);
    begin_stats();
    triangle(3, base_triangle, 1, 0, 0);
    compare_vram(1);
    end_stats(1);
  }
}
static void fallback_cases_test(void) {
  reset_case("packed CT16 fallback");
  configure(2, 48, 1, 0, 0);
  begin_stats();
  triangle(3, base_triangle, 1, 0, 0);
  compare_vram(1);
  end_stats(0);
  reset_case("packed Z16 fallback");
  configure(0, 50, 1, 0, 0);
  begin_stats();
  triangle(3, base_triangle, 1, 0, 0);
  compare_vram(1);
  end_stats(0);
  reset_case("color/depth page alias fallback");
  put(GS_ZBUF_1, 0);
  begin_stats();
  triangle(3, base_triangle, 0, 0, 0);
  compare_vram(0);
  end_stats(0);
  reset_case("aliased stride fallback");
  put(GS_FRAME_1, 0);
  begin_stats();
  triangle(3, base_triangle, 0, 0, 0);
  compare_vram(0);
  end_stats(0);
  reset_case("degenerate fallback");
  begin_stats();
  Vertex v[3];
  memcpy(v, base_triangle, sizeof v);
  v[2] = v[1];
  triangle(3, v, 1, 0, 0);
  compare_vram(1);
  end_stats(0);
}


static void pixel_all(unsigned psm, unsigned bp, unsigned bw, unsigned x,
                      unsigned y, uint32_t color) {
  gs_write_pixel(reference, psm, bp, bw, x, y, color);
  gs_write_pixel(hardware, psm, bp, bw, x, y, color);
  gs_write_pixel(expected, psm, bp, bw, x, y, color);
}
static void texture_state(unsigned bp, unsigned psm, unsigned cbp) {
  put(GS_TEXA, UINT64_C(0x80) << 32);
  put(GS_TEX0_1, bp | 1u << 14 | (uint64_t)psm << 20 | 6u << 26 |
                     UINT64_C(6) << 30 | UINT64_C(1) << 34 | UINT64_C(1) << 35 |
                     (uint64_t)cbp << 37 | (psm == 19 ? UINT64_C(1) << 61 : 0));
}
static void known_textures_and_coherence(void) {
  const uint32_t texel = 0x40765432;
  Vertex v[3];
  for (unsigned indexed = 0; indexed < 2; indexed++) {
    reset_case(indexed ? "indexed T8 known texel" : "CT32 known texel");
    begin_stats();
    if (indexed) {
      pixel_all(19, 4096, 1, 3, 4, 5);
      pixel_all(0, 6144, 1, 5, 0, texel);
      texture_state(4096, 19, 6144);
    } else {
      pixel_all(0, 4096, 1, 3, 4, texel);
      texture_state(4096, 0, 0);
    }
    memcpy(v, base_triangle, sizeof v);
    for (unsigned i = 0; i < 3; i++) {
      v[i].u = 3 * 16;
      v[i].v = 4 * 16;
    }
    triangle(3 | 16 | 256, v, 1, 1, texel);
    compare_vram(1);
    end_stats(1);
  }
  reset_case("hardware framebuffer feedback snapshot");
  begin_stats();
  triangle(3, base_triangle, 1, 0, 0);
  texture_state(0, 0, 0);
  memcpy(v, base_triangle, sizeof v);
  for (unsigned i = 0; i < 3; i++) {
    v[i].x += 256;
    v[i].y += 256;
    v[i].u = 3 * 16;
    v[i].v = 4 * 16;
  }
  triangle(3 | 16 | 256, v, 1, 1, base_triangle[2].color);
  compare_vram(1);
  end_stats(1);

  reset_case("hardware CLUT write to CPU CLD reload");
  begin_stats();
  pixel_all(19, 4096, 1, 3, 4, 5);
  triangle(3, base_triangle, 1, 0, 0);
  texture_state(4096, 19, 0);
  memcpy(v, base_triangle, sizeof v);
  for (unsigned i = 0; i < 3; i++) {
    v[i].x += 256;
    v[i].y += 256;
    v[i].u = 3 * 16;
    v[i].v = 4 * 16;
  }
  triangle(3 | 16 | 256, v, 1, 1, base_triangle[2].color);
  compare_vram(1);
  end_stats(1);

  reset_case("hardware to CPU upload to feedback");
  begin_stats();
  triangle(3, base_triangle, 1, 0, 0);
  pixel_all(0, 0, 1, 4, 4, 0x99ABCDEF);
  texture_state(0, 0, 0);
  for (unsigned i = 0; i < 3; i++) {
    v[i].u = 4 * 16;
    v[i].v = 4 * 16;
  }
  triangle(3 | 16 | 256, v, 1, 1, 0x99ABCDEF);
  compare_vram(1);
  end_stats(1);

  reset_case("hardware compute point hardware transition");
  begin_stats();
  triangle(3, base_triangle, 1, 0, 0);
  put(GS_TEST_1, 1 | 1u << 1 | 1u << 16 | 1u << 17);
  Gs *both[] = {reference, hardware};
  for (unsigned n = 0; n < 2; n++) {
    Gs *g = both[n];
    gs_write(g, GS_PRIM, 0);
    gs_write(g, GS_RGBAQ, UINT64_C(0x3F80000099112233));
    gs_write(g, GS_XYZ2,
             (4096 + 5 * 16) | (uint64_t)(4096 + 5 * 16) << 16 |
                 UINT64_C(200) << 32);
  }
  gs_write_pixel(expected, 0, 0, 1, 5, 5, 0x99112233);
  gs_write_pixel(expected, 48, 64 * 32, 1, 5, 5, 200);
  put(GS_TEST_1, 1u << 16 | 1u << 17);
  triangle(3, v, 1, 0, 0);
  compare_vram(1);
  end_stats(1);

  reset_case("hardware to local copy");
  begin_stats();
  triangle(3, base_triangle, 1, 0, 0);
  put(GS_BITBLTBUF, 1u << 16 | UINT64_C(3072) << 32 | UINT64_C(1) << 48);
  put(GS_TRXPOS, UINT64_C(8) << 32 | UINT64_C(8) << 48);
  put(GS_TRXREG, 16 | UINT64_C(16) << 32);
  put(GS_TRXDIR, 2);
  compare_vram(1);
  end_stats(1);
}
static void deferred_snapshot(void) {
  reset_case("deferred Z captures original GPU issue bytes");
  begin_stats();
  triangle(3, base_triangle, 1, 0, 0);
  uint32_t *issue = malloc(GS_VRAM_SIZE), *image = malloc(GS_VRAM_SIZE);
  check(issue && image, "snapshot allocation");
  if (!issue || !image)
    exit(2);
  memcpy(issue, expected->vram, GS_VRAM_SIZE);
  uint32_t want[16] = {0}, staged[2][16] = {{0}};
  want[2] = 1;
  Gs *both[] = {reference, hardware};
  for (unsigned mode = 0; mode < 2; mode++)
    both[mode]->stage_pages(both[mode], want, staged[mode]);
  Vertex v[3];
  memcpy(v, base_triangle, sizeof v);
  for (unsigned i = 0; i < 3; i++)
    v[i].z = 200;
  triangle(3, v, 1, 0, 0);
  for (unsigned mode = 0; mode < 2; mode++) {
    memset(image, 0xA5, GS_VRAM_SIZE);
    both[mode]->fetch_pages(both[mode], staged[mode], image);
    check(staged[mode][2] & 1, "requested dirty Z page staged");
    unsigned mismatches = 0;
    for (unsigned page = 0; page < 512; page++) {
      int captured = (staged[mode][page / 32] >> (page & 31)) & 1;
      for (unsigned word = page * 2048; word < (page + 1) * 2048; word++) {
        uint32_t wanted = captured ? issue[word] : 0xA5A5A5A5;
        if (image[word] != wanted) {
          if (mismatches < 5)
            fprintf(stderr,
                    "FAIL deferred mode=%u word=%X expected=%08X actual=%08X\n",
                    mode, word, wanted, image[word]);
          mismatches++;
        }
      }
    }
    check(!mismatches, "deferred staging bytes/untouched pages");
  }
  free(issue);
  free(image);
  compare_vram(1);
  end_stats(1);
}

static void rich_shading_reference(int force_compute) {
  for (unsigned format = 0; format < 3; format++)
    for (unsigned filtered = 0; filtered < 2; filtered++) {
      reset_case("perspective indexed/filter/fog/IIP reference comparison");
      configure(force_compute ? 2 : 1, 49, 2, 0x00110044, 0);
      unsigned psm = format == 0 ? 0 : format == 1 ? 19 : 20;
      for (unsigned y = 0; y < 64; y++)
        for (unsigned x = 0; x < 64; x++) {
          uint32_t color =
              0x80000000 | ((x * 0x070309u + y * 0x110503u) & 0xFFFFFF);
          pixel_all(psm, 4096, 1, x, y,
                    psm == 0 ? color
                             : (x * 3 + y * 5) & (psm == 19 ? 255 : 15));
        }
      for (unsigned i = 0; i < 256; i++)
        pixel_all(0, 6144, 1, i % 64, i / 64,
                  0x80000000 | ((i * 0x070B11u) & 0xFFFFFF));
      put(GS_TEXA, UINT64_C(0x80) << 32);
      put(GS_TEX0_1, 4096 | 1u << 14 | (uint64_t)psm << 20 | 6u << 26 |
                         UINT64_C(6) << 30 | UINT64_C(1) << 34 |
                         UINT64_C(6144) << 37 |
                         (format ? UINT64_C(1) << 61 : 0));
      put(GS_TEX1_1, filtered ? 32 : 0);
      put(GS_FOGCOL, 0x4060A0);
      begin_stats();
      for (unsigned n = 0; n < 24; n++) {
        Vertex v[3];
        memcpy(v, base_triangle, sizeof v);
        for (unsigned i = 0; i < 3; i++) {
          v[i].x += (int)(n % 4) * 96 + (int)(n % 7);
          v[i].y += (int)(n / 4) * 48 + (int)(n % 5);
          v[i].z = 0xFFF000 + n * 7 + i * 13;
          v[i].color = 0x80000000 |
                       ((0x203040 + n * 0x030507 + i * 0x111709) & 0xFFFFFF);
          v[i].fog = (n * 13 + i * 47) & 255;
          v[i].s = i == 1 ? .9f : .05f;
          v[i].t = i == 2 ? .85f : .1f;
          v[i].q = i == 0 ? 1.0f : i == 1 ? .75f : 1.5f;
        }
        triangle(3 | 8 | 16 | 32 | 64, v, 0, 0, 0);
      }
      compare_vram(0);
      end_stats(!force_compute);
    }
}
static void extreme_coordinates(void) {
  Vertex v[3] = {{32640, 0, 100, 0x80442211, 0, 0, 128},
                 {65535, 256, 100, 0x80442211, 0, 0, 128},
                 {32640, 512, 100, 0x80442211, 0, 0, 128}};
  reset_case("far positive X and viewport clipping");
  put(GS_XYOFFSET_1, 0);
  put(GS_FRAME_1, 32u << 16);
  put(GS_SCISSOR_1, 2040 | UINT64_C(2047) << 16 | UINT64_C(7) << 48);
  begin_stats();
  triangle(3, v, 1, 0, 0);
  compare_vram(1);
  end_stats(1);
  reset_case("far positive Y and viewport clipping");
  put(GS_XYOFFSET_1, 0);
  put(GS_ZBUF_1, 256);
  put(GS_SCISSOR_1,
      UINT64_C(31) << 16 | UINT64_C(2040) << 32 | UINT64_C(2047) << 48);
  Vertex vy[3] = {{0, 32640, 100, 0x80442211, 0, 0, 128},
                  {512, 32640, 100, 0x80442211, 0, 0, 128},
                  {256, 65535, 100, 0x80442211, 0, 0, 128}};
  begin_stats();
  triangle(3, vy, 1, 0, 0);
  compare_vram(1);
  end_stats(1);
  reset_case("negative signed16 boundary");
  put(GS_XYOFFSET_1, 65535 | UINT64_C(4096) << 32);
  Vertex vn[3] = {{-65535, 0, 100, 0x80442211, 0, 0, 128},
                  {0, 0, 100, 0x80442211, 0, 0, 128},
                  {0, 512, 100, 0x80442211, 0, 0, 128}};
  begin_stats();
  triangle(3, vn, 1, 0, 0);
  compare_vram(1);
  end_stats(1);
  reset_case("full2048 stride wrapping rejects hardware");
  put(GS_FRAME_1, 32u << 16);
  put(GS_SCISSOR_1, UINT64_C(2047) << 16 | UINT64_C(2047) << 48);
  begin_stats();
  triangle(3, base_triangle, 0, 0, 0);
  compare_vram(0);
  end_stats(0);
}


int main(int argc, char **argv) {
  int use_default=0;
  for(int i=1;i<argc;i++) {
    if(!strcmp(argv[i],"--hardware-default"))use_default=1;
    else return 2;
  }
  D3D_FEATURE_LEVEL level,
      levels[] = {D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0,
                  D3D_FEATURE_LEVEL_11_1};
  HRESULT hr =
      D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, levels, 3,
                        D3D11_SDK_VERSION, &device, &level, &context);
  if (FAILED(hr)) {
    puts("UNSUPPORTED hardware FL11_1+");
    return 3;
  }
  reference = calloc(1, sizeof(Gs));
  hardware = calloc(1, sizeof(Gs));
  expected = calloc(1, sizeof(Gs));
  seed = malloc(GS_VRAM_SIZE);
  raw_reference = malloc(GS_VRAM_SIZE);
  raw_hardware = malloc(GS_VRAM_SIZE);
  if (!reference || !hardware || !expected || !seed || !raw_reference ||
      !raw_hardware)
    return 2;
  uint32_t random = 0xDDBA1234;
  for (unsigned i = 0; i < GS_VRAM_SIZE / 4; i++) {
    random = random * 1664525u + 1013904223u;
    seed[i] = random;
  }
  gs_reset(reference);
  gs_reset(hardware);
  gs_reset(expected);
  _putenv_s("DHD_GS_HARDWARE", "0");
  if (!gs_gpu_init(reference, device, context))
    return 2;
  _putenv_s("DHD_GS_HARDWARE", use_default ? "" : "1");
  if (!gs_gpu_init(hardware, device, context))
    return 2;
  GsGpuRasterStats stats;
  gs_gpu_get_raster_stats(hardware, &stats);
  if (!stats.hardware_available) {
    puts("UNSUPPORTED conservative/ROV hardware");
    return 3;
  }
  check(stats.hardware_enabled, use_default
                                    ? "hardware executable default enabled"
                                    : "hardware test override enabled");
  flat_depth_matrix();
  shading_order();
  fallback_cases_test();
  known_textures_and_coherence();
  deferred_snapshot();
  extreme_coordinates();
  rich_shading_reference(0);
  gs_gpu_get_raster_stats(hardware, &stats);
  printf("hardware integration checks=%u positive_cases=%u fallback_cases=%u "
         "full_vram_words=%llu failures=%u\n",
         checks, positive_cases, fallback_cases, compared_words, failures);
  gs_gpu_get_raster_stats(hardware, &stats);
  printf("actual HW batches=%llu primitives=%llu compute_batches=%llu\n",
         stats.hardware_batches, stats.hardware_primitives,
         stats.reference_batches);
  gs_gpu_destroy(reference);
  gs_gpu_destroy(hardware);
  ID3D11DeviceContext_Release(context);
  ID3D11Device_Release(device);
  free(reference);
  free(hardware);
  free(expected);
  free(seed);
  free(raw_reference);
  free(raw_hardware);
  puts(failures ? "RESULT=FAIL" : "RESULT=PASS");
  return failures ? 1 : 0;
}
