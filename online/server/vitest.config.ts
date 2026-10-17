import { cloudflareTest } from "@cloudflare/vitest-plugin";
import { defineConfig } from "vitest/config";

export default defineConfig({
  plugins: [
    cloudflareTest({
      wrangler: { configPath: "./wrangler.jsonc" },
      // The tests run with the version gate raised past 0.1.0 (wrangler.jsonc may still allow it):
      // the helpers' hello is release 0.2.0, and a hello without clientVersion (0.1.0) is refused.
      miniflare: { bindings: { MIN_VERSION: "0.2.0" } },
    }),
  ],
});
