// Worker entry: routes lobby-list requests to the LobbyIndex and WebSockets to a Room by code.
import { PROTOCOL, minVersionOf, randomCode } from "./protocol";

export { Room } from "./room";
export { LobbyIndex } from "./lobbies";

const CORS = { "Access-Control-Allow-Origin": "*" };

function json(body: unknown, status = 200): Response {
  return Response.json(body, { status, headers: CORS });
}

export default {
  async fetch(request: Request, env: Env): Promise<Response> {
    const path = new URL(request.url).pathname.replace(/\/+$/, "");

    // minVersion: the oldest client release the server lets play (MIN_VERSION, wrangler.jsonc).
    const minVersion = minVersionOf(env.MIN_VERSION);
    if (path === "/v1/health") return json({ ok: true, protocol: PROTOCOL, minVersion });

    if (path === "/v1/lobbies") {
      if (request.method !== "GET") return json({ error: "GET only" }, 405);
      const lobbies = await env.LOBBIES.get(env.LOBBIES.idFromName("global")).list();
      return json({ protocol: PROTOCOL, minVersion, lobbies });
    }

    const m = path.match(/^\/v1\/rooms\/([^/]+)$/);
    if (m) {
      if (request.headers.get("Upgrade")?.toLowerCase() !== "websocket")
        return json({ error: "Expected a WebSocket upgrade" }, 426);
      const headers = new Headers(request.headers);
      headers.delete("x-dhd-create");
      if (m[1] === "new") {
        headers.set("x-dhd-create", "1");
        for (let attempt = 0; attempt < 8; attempt++) {
          const code = randomCode();
          const room = env.ROOM.get(env.ROOM.idFromName(code));
          if (await room.claim(code)) return room.fetch(new Request(request, { headers }));
        }
        return json({ error: "Could not allocate a room code" }, 503);
      }
      const code = m[1].toUpperCase();
      if (!/^[A-Z0-9]{1,12}$/.test(code)) return json({ error: "Bad room code" }, 404);
      return env.ROOM.get(env.ROOM.idFromName(code)).fetch(new Request(request, { headers }));
    }

    return json({ error: "Not found" }, 404);
  },
} satisfies ExportedHandler<Env>;
