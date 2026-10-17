#ifndef DHD_UI_STYLE_H
#define DHD_UI_STYLE_H
typedef struct UiStyle {
    int x, y, width, height;
    int red, green, blue, alpha;
} UiStyle;
typedef char UiStyle_size_check[sizeof(UiStyle) == 0x20 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
UiStyle *func_0018D7D0(UiStyle *, int, int, int, int, int, int, int, int);
void func_0018D800(const UiStyle *);
#if defined(__mips__)
void InitializeUiStyleDiscard(UiStyle *, int, int, int, int, int, int, int, int) __asm__("func_0018D7D0");
#else
static inline void InitializeUiStyleDiscard(UiStyle *style, int x, int y, int w, int h, int r, int g, int b, int a) {
    func_0018D7D0(style, x, y, w, h, r, g, b, a);
}
#endif
#ifdef __cplusplus
}
#endif
#endif
