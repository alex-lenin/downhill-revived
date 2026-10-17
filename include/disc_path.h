#ifndef DHD_DISC_PATH_H
#define DHD_DISC_PATH_H
#ifdef __cplusplus
extern "C" {
#endif
void func_0020DC28(char *, const char *, const char *, const char *);
/* Returns the global resource name; the historical caller's input is unused. */
char *func_001BA3A8(void);
#if defined(__mips__)
char *GetResourceNameLegacy(const void *) __asm__("func_001BA3A8");
#else
static inline char *GetResourceNameLegacy(const void *ignored) { return func_001BA3A8(); }
#endif

#if defined(__mips__)
/* Historical callers set an ignored device-mode slot; retain its EE scheduling. */
void BuildDiscPathWithMode(char *, const char *, const char *, const char *, int) __asm__("func_0020DC28");
#else
static inline void BuildDiscPathWithMode(char *out, const char *dir, const char *name, const char *ext, int ignored) {
    func_0020DC28(out, dir, name, ext);
}
#endif
#ifdef __cplusplus
}
#endif
#endif
