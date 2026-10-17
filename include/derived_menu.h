#ifndef DHD_DERIVED_MENU_H
#define DHD_DERIVED_MENU_H
#include "menu.h"
typedef struct DerivedMenuMember { void *vptr; char x4, x5; char pad6[2]; } DerivedMenuMember;
typedef struct SharedDerivedMenu { SharedMenu base; int x4FC, x500; DerivedMenuMember member; } SharedDerivedMenu;
typedef char SharedDerivedMenu_size_check[sizeof(SharedDerivedMenu) == 0x50C ? 1 : -1];
#if !defined(__mips__)
#ifdef __cplusplus
extern "C" {
#endif
SharedDerivedMenu *func_0018F210(SharedDerivedMenu *);
#ifdef __cplusplus
}
#endif
#endif
#endif
