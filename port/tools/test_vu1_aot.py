"""Control-flow boundaries for generated VU1 blocks; no game data required."""
import unittest
from vu1_aot import blocks, delay_slots, ends_block

NOP = (0x80000030, 0x000002ff)  # IADD vi0, vi0, vi0 | NOP
BRANCH = (0x40000002, NOP[1])
END = (NOP[0], NOP[1] | (1 << 30))
KICK = (0x800006fc, NOP[1])


class BlockTransitions(unittest.TestCase):
    def test_branch_and_slot_fuse_but_slot_remains_an_entry(self):
        pairs = dict(enumerate((NOP, BRANCH, NOP, NOP, END, NOP)))
        result = blocks(pairs)
        self.assertEqual(result[0], (NOP, BRANCH, NOP))
        self.assertEqual(result[2][0], NOP)
        self.assertEqual(delay_slots(pairs)[2], (NOP,))

    def test_end_and_slot_fuse(self):
        self.assertEqual(blocks({0: END, 1: NOP})[0], (END, NOP))

    def test_callback_is_never_crossed(self):
        self.assertEqual(blocks({0: NOP, 1: KICK, 2: NOP})[0], (NOP, KICK))
        self.assertEqual(blocks({0: BRANCH, 1: KICK})[0], (BRANCH,))

    def test_control_transfer_in_slot_uses_existing_transition_loop(self):
        self.assertEqual(blocks({0: BRANCH, 1: END, 2: NOP})[0], (BRANCH,))
        self.assertEqual(delay_slots({0: BRANCH, 1: END})[1], (END,))

    def test_immediate_lower_bits_are_not_a_branch(self):
        immediate = (BRANCH[0], NOP[1] | (1 << 31))
        self.assertFalse(ends_block(*immediate))
        self.assertEqual(blocks({0: immediate, 1: NOP})[0], (immediate, NOP))

    def test_block_limit_and_missing_slot(self):
        pairs = {i: NOP for i in range(40)}
        self.assertEqual(len(blocks(pairs)[0]), 32)
        self.assertEqual(blocks({0: BRANCH, 2: NOP})[0], (BRANCH,))


if __name__ == '__main__':
    unittest.main()
