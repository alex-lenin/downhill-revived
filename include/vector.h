#ifndef DHD_VECTOR_H
#define DHD_VECTOR_H

typedef struct Vector3 {
    float x, y, z;
} Vector3;

typedef struct Vector4 {
    float x, y, z, w;
} Vector4 __attribute__((aligned(16)));

typedef struct Matrix4 {
    Vector4 row[4];
} Matrix4;

typedef char Vector4_size_check[sizeof(Vector4) == 16 ? 1 : -1];
typedef char Vector3_size_check[sizeof(Vector3) == 12 ? 1 : -1];
typedef char Matrix4_size_check[sizeof(Matrix4) == 64 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void func_00221888(const Matrix4 *, const Vector4 *, Vector4 *);
void func_002219A8(Matrix4 *, const Matrix4 *, const Matrix4 *);
void func_00221930(Matrix4 *, const Matrix4 *, const Matrix4 *);
void func_00221A60(Matrix4 *, const Matrix4 *, const Matrix4 *);
void func_00221F20(Matrix4 *);
void func_002224E0(const Matrix4 *, Vector3 *);
void func_002226C8(Matrix4 *, const Vector4 *);
#ifdef __cplusplus
}
#endif

#endif
