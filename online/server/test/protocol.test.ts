import { describe, expect, it } from "vitest";
import { DesyncTracker } from "../src/desync";
import {
  CODE_ALPHABET,
  inputFrameLastField,
  isValidCode,
  measuredRtt,
  randomCode,
  readU64Hex,
  suggestedInputDelay,
} from "../src/protocol";
import { hashFrame, inputFrame } from "./helpers";

describe("join codes", () => {
  it("are 6 characters with no confusable ones", () => {
    for (const c of "01ILO") expect(CODE_ALPHABET).not.toContain(c);
    for (let i = 0; i < 200; i++) {
      const code = randomCode();
      expect(code).toHaveLength(6);
      expect(isValidCode(code)).toBe(true);
    }
    expect(isValidCode("ABC0EF")).toBe(false);
  });
});

describe("measuredRtt", () => {
  it("uses the first ping only until a later one arrives", () => {
    expect(measuredRtt(null, [])).toBeNull();
    expect(measuredRtt(140, [])).toBe(140);
    // the warm-up ping reads high; 53-60 ms links must give delay 5, not 8
    expect(measuredRtt(140, [55, 60, 53])).toBe(55);
    expect(suggestedInputDelay([measuredRtt(140, [55, 60, 53])!, measuredRtt(150, [58, 60])!])).toBe(5);
  });
});

describe("suggestedInputDelay", () => {
  it("uses the two worst round-trips", () => {
    expect(suggestedInputDelay([])).toBe(1);
    expect(suggestedInputDelay([0, 0])).toBe(1);
    // 40 ms + 60 ms -> 50 ms one way -> ceil(50 / 16.68) = 3, + 1
    expect(suggestedInputDelay([60, 10, 40])).toBe(4);
    // one member: half its round-trip
    expect(suggestedInputDelay([100])).toBe(4);
    expect(suggestedInputDelay([30, 30])).toBe(3);
  });
  it("is clamped to 1..8", () => {
    expect(suggestedInputDelay([1000, 1000])).toBe(8);
  });
});

describe("frames", () => {
  it("INPUT: last field and length checks", () => {
    const f = new Uint8Array(inputFrame(0, 100, [[0xffff, 128, 128, 128, 128], [0, 1, 2, 3, 4], [5, 6, 7, 8, 9]]));
    expect(f.length).toBe(7 + 18);
    expect(inputFrameLastField(f)).toBe(102);
    expect(inputFrameLastField(f.subarray(0, 20))).toBe(-1);
    const zero = new Uint8Array(7);
    zero[0] = 1;
    expect(inputFrameLastField(zero)).toBe(-1);
  });
  it("HASH: hash read as 16 hex digits", () => {
    const f = new Uint8Array(hashFrame(1, 59, 0x0123456789abcdefn));
    expect(readU64Hex(f, 6)).toBe("0123456789abcdef");
  });
});

describe("DesyncTracker", () => {
  it("reports the first differing field once", () => {
    const t = new DesyncTracker([0, 1, 2]);
    expect(t.add(0, 59, "aa")).toBeNull();
    expect(t.add(1, 59, "aa")).toBeNull();
    expect(t.add(2, 59, "aa")).toBeNull();
    expect(t.add(0, 119, "aa")).toBeNull();
    expect(t.add(1, 119, "bb")).toEqual({ field: 119, hashes: { "0": "aa", "1": "bb" } });
    expect(t.add(2, 119, "cc")).toBeNull(); // already reported
    expect(t.add(0, 179, "aa")).toBeNull();
    expect(t.add(1, 179, "dd")).toBeNull(); // later than 119
  });
  it("reports an earlier field found later", () => {
    const t = new DesyncTracker([0, 1]);
    t.add(0, 119, "a");
    expect(t.add(1, 119, "b")?.field).toBe(119);
    t.add(0, 59, "a");
    expect(t.add(1, 59, "b")?.field).toBe(59);
  });
  it("ignores hashes from slots that left", () => {
    const t = new DesyncTracker([0, 1]);
    t.remove(1);
    expect(t.add(1, 59, "b")).toBeNull();
    expect(t.add(0, 59, "a")).toBeNull();
  });
});
