"""Persist only player preferences; never invoke the game's career save."""
def add(patches):
    patches.setdefault('unsorted/001EB1D8.cpp', []).extend([
        ('text', 'int func_001EB4D0(void)\n{',
         'extern "C" void port_game_options_boot(void);\n'
         'extern "C" void port_game_options_save(void);\n'
         'int func_001EB4D0(void)\n{'),
        ('text', '    D_004B25A0__target.xB174 = 0;',
         '    port_game_options_boot();\n'
         '    D_004B25A0__target.xB174 = 0;'),
        ('text', '        D_0029DEEC__n42=13;\n        PlayMenuSelectLegacy',
         '        port_game_options_save();\n'
         '        D_0029DEEC__n42=13;\n        PlayMenuSelectLegacy'),
        ('text', '        D_0029DED8__c12 = 2;\n        D_0029DF50__c12 = 2;',
         '        port_game_options_save();\n'
         '        D_0029DED8__c12 = 2;\n        D_0029DF50__c12 = 2;'),
    ])
    patches.setdefault('unsorted/0015F410.c', []).append(('text',
        '    func_00190688(D_003F52D0);',
        '    { extern void port_game_options_save(void); port_game_options_save(); }\n'
        '    func_00190688(D_003F52D0);'))
    patches.setdefault('unsorted/00193B20.c', []).append(('text',
        'p=o+0x11a60;',
        '{ extern void port_game_options_apply(void); port_game_options_apply(); }\np=o+0x11a60;'))
