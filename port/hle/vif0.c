/* Module VIF0 shares the proven parser with the native VIF1 frontend.
   The wasm core compiles this unit, never the native VIF1/GS worker implementation. */
#define DHD_VIF0_ONLY 1
#define vif_feed module_vif0_feed
#include "vif.c"
