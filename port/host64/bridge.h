#ifndef DHD_HOST64_BRIDGE_H
#define DHD_HOST64_BRIDGE_H
#include <stdint.h>
#include "wasm-rt.h"
void host64_bridge_bind(wasm_rt_memory_t *memory);
void *host64_bridge_pointer(uint32_t address, uint32_t bytes);
const char *host64_bridge_string(uint32_t address);
uint32_t host64_bridge_pcm_bytes(int32_t frames);
uint32_t host64_bridge_array_bytes(int32_t count, uint32_t stride);
uint32_t host64_bridge_alloc(uint32_t bytes, uint32_t alignment);
#endif
