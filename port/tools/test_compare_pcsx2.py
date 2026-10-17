"""Synthetic checks for input quantization, dump decoding and comparison scores."""
from pathlib import Path
import struct
import tempfile
import unittest

import numpy as np
from PIL import Image

import compare_pcsx2 as compare
import pcsx2_retail as retail
import race_compare as race


class CompareTests(unittest.TestCase):
    def test_movie_pad_boundaries_and_layout(self):
        events = compare.parse_pad('0:0.02:cross+start,0.01:0.02:triangle')
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'input.p2m2'
            compare.write_movie(path, events, 4)
            data = path.read_bytes()
        self.assertEqual(len(data), 570 + 4 * 36)
        self.assertEqual(struct.unpack_from('<I', data, 561)[0], 4)
        masks = [struct.unpack_from('<H', data, 570 + i*36)[0] ^ 65535 for i in range(4)]
        self.assertEqual(masks, [0x4008, 0x5008, 0, 0])
        self.assertEqual(data[570+12], 255)  # Cross pressure.
        self.assertEqual(data[570+18:570+36], bytes([255,255]+[128]*4+[0]*12))

    def test_invalid_scripts(self):
        for script in ('', 'nan:1:cross', '-1:1:cross', '0:0:cross', '0:1:nope',
                       '0:1:cross,', '0:1:cross++start'):
            with self.subTest(script=script), self.assertRaises(ValueError):
                compare.parse_pad(script)

    def test_dump_field_register_pairing_and_truncation(self):
        regs = bytearray(8192)
        struct.pack_into('<Q', regs, 0, 2)
        struct.pack_into('<Q', regs, 0x20, 3)
        struct.pack_into('<Q', regs, 0x90, 1 << 9)
        struct.pack_into('<Q', regs, 0xa0, (7 << 32) | (15 << 44))
        freeze = struct.pack('<I', 8) + bytes(4194752-4)
        data = struct.pack('<II', 0, len(freeze)) + freeze + regs
        data += b'\3' + regs + b'\1\0\2' + struct.pack('<I', 1)
        data += b'\3' + regs + b'\1\1'
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'synthetic.gs'
            path.write_bytes(data)
            states = compare.read_dump(path)
            self.assertEqual([s['field'] for s in states], [0, 1])
            self.assertEqual(states[1]['circuit'], 1)
            self.assertEqual((states[1]['width'], states[1]['height']), (8, 16))
            path.write_bytes(data[:-1])
            with self.assertRaises(ValueError):
                compare.read_dump(path)

    def test_bob_crop_ignores_unused_rows(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'raw.png'
            pixels = np.zeros((16, 8, 3), dtype=np.uint8)
            pixels[:8] = np.arange(8, dtype=np.uint8)[:, None, None]
            pixels[8:] = 255
            Image.fromarray(pixels).save(path)
            result = np.asarray(compare.scanout(path, dict(width=8, height=16, half_height=True,
                                                          dbx=0, dby=0, psm=1)))
        self.assertTrue(np.array_equal(result[:, 0, 0], np.repeat(np.arange(8), 2)))

    def test_scores_and_worst_region(self):
        a = np.zeros((128, 128, 3), dtype=np.uint8)
        b = a.copy()
        b[64:,64:] = 64
        identical, _ = compare.metrics(Image.fromarray(a), Image.fromarray(a), 16)
        score, _ = compare.metrics(Image.fromarray(a), Image.fromarray(b), 16)
        self.assertEqual(identical['mae'], 0)
        self.assertAlmostEqual(identical['block_ssim'], 1)
        self.assertEqual(score['mae'], 16)
        self.assertEqual(score['pixels_over_threshold_percent'], 25)
        self.assertEqual(score['worst_regions'][0]['box'], [64,64,128,128])

    def test_shift_correction_keeps_raw_score(self):
        rng = np.random.default_rng(23)
        a = rng.integers(0, 256, size=(32,32,3), dtype=np.uint8)
        b = np.zeros_like(a)
        b[:-1] = a[1:]
        class Args:
            threshold = 16
            line_shift = 1
        with tempfile.TemporaryDirectory() as tmp:
            score = compare.comparison(Image.fromarray(a), Image.fromarray(b), Args,
                                       Path(tmp) / 'panel.png')
        self.assertEqual(score['mae'], 0)
        self.assertGreater(score['raw_mae'], 0)
        self.assertEqual(score['vertical_shift'], 1)
        self.assertEqual(score['scored_pixels'], 31*32)

    def test_temporal_matching_stays_in_supplied_window(self):
        native = Image.new('RGB', (64,56), (90,90,90))
        with tempfile.TemporaryDirectory() as tmp:
            paths = {}
            for field, level in [(98,10), (99,89), (100,0), (101,110), (102,90)]:
                path = Path(tmp) / f'{field}.png'
                Image.new('RGB',(64,56),(level,level,level)).save(path)
                paths[field] = path
            window = {field: path for field, path in paths.items() if abs(field-100) <= 1}
            selected, _ = retail.choose_reference(native, window, 100)
        self.assertEqual(selected, 99)  # Exact image at 102 is outside the requested radius.

    def test_retail_bios_config_references_file_in_place(self):
        with tempfile.TemporaryDirectory() as tmp:
            directory = Path(tmp)
            bios = directory / 'original/owned.ROM0'
            bios.parent.mkdir()
            bios.write_bytes(b'synthetic BIOS path test; not a real ROM')
            data = retail.config(directory, bios, 13)
            ini = (data/'PCSX2/inis/PCSX2.ini').read_text()
            self.assertIn(str(bios.parent), ini)
            self.assertIn('BIOS = owned.ROM0', ini)
            self.assertEqual(list(directory.rglob('*.ROM0')), [bios])

    def test_race_clock_pad_matches_autopad_intervals(self):
        # autopad_read gets clock / 60 seconds: start inclusive, end exclusive.
        self.assertEqual(race.clock_events('0.5:0.3:left,3:2:cross+r1'),
                         [(30, 48, compare.BUTTONS['left'], 'left'),
                          (180, 300, compare.BUTTONS['cross'] | compare.BUTTONS['r1'], 'cross+r1')])
        self.assertEqual(race.spec(race.MOMENTS), 'c+120,g+30,g+600,g+1800,g+3000')


if __name__ == '__main__':
    unittest.main()
