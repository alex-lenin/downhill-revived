"""EXIT GAME (port/hle/exit_menu.c, finding 056): a fifth main menu row (func_001ED5B8) under
ONLINE that asks "Quit Downhill Revived?" in the game's popup and ends the game on Yes. Applied
after port/patches.py's ONLINE patch, on its text. Leaving the main menu by the game's rows still
puts its last row back to 2 (the ONLINE patch), so the gates' route is unchanged.
"""

UNIT = 'unsorted/001EB1D8.cpp'


def add(PATCHES):
    PATCHES[UNIT] = PATCHES.get(UNIT, []) + [
        ('text', 'extern int port_main_label(int *table, int index);\n',
         'extern int port_main_label(int *table, int index);\n'
         'extern int port_exit_label(int *table, int index);\n'
         'extern void port_exit_menu(void);\n'),
        ('text', 'void func_001ED5B8__cED5B8(void) {\n    u8__cED5B8 disabled[4];',
         'void func_001ED5B8__cED5B8(void) {\n    u8__cED5B8 disabled[5];'),
        ('text', 'func_001F7420__cED5B8(&D_004C6CA8__cED5B8, 10, 0, 0, 3, port_main_row, disabled, 4);',
         'func_001F7420__cED5B8(&D_004C6CA8__cED5B8, 10, 0, 0, 4, port_main_row, disabled, 5);'),
        ('re', (r'port_main_label\(D_0029E054__cED5B8, index\)', 3), 'port_exit_label(D_0029E054__cED5B8, index)'),
        ('text', '            port_main_row = port_online_menu() ? 0 : 3;\n'
                 '            func_001F57D0__cED5B8(0x2329, 1);\n        }\n',
         '            port_main_row = port_online_menu() ? 0 : 3;\n'
         '            func_001F57D0__cED5B8(0x2329, 1);\n'
         '        } else if (choice == 4) {\n'
         '            func_001F57D0__cED5B8(0x2329, 0);\n'
         '            port_exit_menu();\n'
         '            port_main_row = 4;\n'
         '            func_001F57D0__cED5B8(0x2329, 1);\n'
         '        }\n'),
    ]
