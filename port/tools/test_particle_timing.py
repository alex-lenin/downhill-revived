"""Synthetic data-conversion regression for packed particle timing, finding 061."""
from pathlib import Path
import tempfile
import unittest
import warnings
from unittest.mock import patch

import data


class ParticleTimingTests(unittest.TestCase):
    def convert(self, slot, value, target=None):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'timing.s'
            path.write_text('/* 000000 %08X %s */ .word %s\n' %
                            (slot, value.to_bytes(4, 'little').hex().upper(),
                             target or 'D_%08X' % value))
            out, refs, fixups = [], set(), data.Fixups()
            with warnings.catch_warnings(), patch.object(data, 'data_entries', return_value={}):
                warnings.simplefilter('ignore', ResourceWarning)  # convert's existing file iterator
                data.convert(str(path), out, {}, refs, fixups)
            return out, refs, fixups

    def test_timing_halfwords_never_relocated(self):
        for index in range(77):
            with self.subTest(index=index):
                out, refs, fixups = self.convert(0x27D300 + index * 0x50 + 0x18, 0x00780000)
                self.assertEqual(out, ['.long 0x00780000'])
                self.assertFalse(refs)
                self.assertFalse(fixups)

    def test_same_bits_outside_timing_still_form_a_pointer(self):
        # Unclassified fields retain the existing pointer policy, including
        # the real vector-pointer slot in the particle defaults table.
        for slot in (0x27BFD4, 0x27D314, 0x27EB28):
            out, refs, fixups = self.convert(slot, 0x00780000)
            self.assertIn('_D_00780000', refs)
            self.assertEqual(fixups[0][1], ['_D_00780000'])

    def test_audited_numeric_schemas_preserve_address_shaped_values(self):
        for slots, value in ((data.INSTRUCTION_WORDS, 0x00661824),
                             (data.TLB_WORDS, 0x007FE000),
                             (data.TEXT_WORDS, 0x006E6170),
                             (data.INFLATE_EXTRA_WORDS, 0x00630000)):
            for slot in slots:
                with self.subTest(slot=hex(slot)):
                    out, refs, fixups = self.convert(slot, value)
                    self.assertEqual(out, ['.long 0x%08X' % value])
                    self.assertFalse(refs)
                    self.assertFalse(fixups)

    def test_tlb_entryhi_is_not_a_function_pointer(self):
        out, refs, fixups = self.convert(0x283654, 0x00180000, 'func_00180000')
        self.assertEqual(out, ['.long 0x00180000'])
        self.assertFalse(refs)
        self.assertFalse(fixups)

    def test_sdk_descriptors_and_table_pointers_still_relocate(self):
        for slot, value, target in ((0x28221C, 0x25EF38, 'kFindAddress'),
                                    (0x2829F4, 0x25F260, 'kCopy'),
                                    (0x283164, 0x25F2E8, 'func_0025F2E8'),
                                    (0x283524, 0x25F570, 'func_0025F570'),
                                    (0x2837E4, 0x283630, 'D_00283630'),
                                    (0x29D99C, 0x663B08, 'D_00663B08')):
            out, refs, fixups = self.convert(slot, value, target)
            self.assertEqual(fixups[0][1], ['_' + target])
            self.assertIn('_' + target, refs)


if __name__ == '__main__':
    unittest.main()
