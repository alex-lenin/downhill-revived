// Compares the per-field state hashes racers send (HASH frames) and finds the first field where
// two of them differ.

/** Fields kept waiting for hashes from every racer; older ones are dropped. */
const MAX_PENDING_FIELDS = 64;

export interface Desync {
  field: number;
  /** slot -> 16 hex digits, for every slot that had reported this field. */
  hashes: Record<string, string>;
}

export class DesyncTracker {
  private pending = new Map<number, Map<number, string>>();
  private active: Set<number>;
  /** Earliest bad field reported so far. */
  private reported = Infinity;

  constructor(slots: Iterable<number>) {
    this.active = new Set(slots);
  }

  /** Records one hash; returns a desync to broadcast, or null. */
  add(slot: number, field: number, hash: string): Desync | null {
    if (!this.active.has(slot)) return null;
    let byField = this.pending.get(field);
    if (!byField) {
      byField = new Map();
      this.pending.set(field, byField);
      this.prune();
    }
    if (byField.has(slot)) return null;
    byField.set(slot, hash);

    let result: Desync | null = null;
    for (const h of byField.values()) {
      if (h !== hash) {
        if (field < this.reported) {
          this.reported = field;
          result = { field, hashes: Object.fromEntries([...byField].map(([s, v]) => [String(s), v])) };
        }
        break;
      }
    }
    this.settle(field, byField);
    return result;
  }

  /** A racer left: stop waiting for its hashes. */
  remove(slot: number): void {
    this.active.delete(slot);
    for (const [field, byField] of this.pending) {
      byField.delete(slot);
      this.settle(field, byField);
    }
  }

  private settle(field: number, byField: Map<number, string>): void {
    for (const s of this.active) if (!byField.has(s)) return;
    this.pending.delete(field);
  }

  private prune(): void {
    if (this.pending.size <= MAX_PENDING_FIELDS) return;
    const oldest = Math.min(...this.pending.keys());
    this.pending.delete(oldest);
  }
}
