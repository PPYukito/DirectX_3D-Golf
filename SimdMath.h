/*==============================================================================

   SimdMath Library Header [SimdMath.h]
                                Date   : 2026/02/08
--------------------------------------------------------------------------------

    <Notes>
    DirectXMath Wrapper Library for vector/matrix math.

==============================================================================*/

#ifndef SIMD_MATH_H
#define SIMD_MATH_H

#pragma once
#include <DirectXMath.h>

#include <cmath>
#include <vector>

// 前方宣言
class Matrix;

// =======================================================================
// Vector2
// =======================================================================
class Vector2 : public DirectX::XMFLOAT2
{
public:
    using DirectX::XMFLOAT2::XMFLOAT2;

    constexpr Vector2() : DirectX::XMFLOAT2(0.0f, 0.0f) {}
    constexpr Vector2(float x, float y) : DirectX::XMFLOAT2(x, y) {}
    constexpr Vector2(const DirectX::XMFLOAT2& v) : DirectX::XMFLOAT2(v) {}
    Vector2(const DirectX::XMVECTOR& v) { DirectX::XMStoreFloat2(this, v); }

    // キャスト演算子
    operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat2(this); }

    // 加算
    Vector2 operator+(const Vector2& other) const { return DirectX::XMVectorAdd(*this, other); }
    Vector2& operator+=(const Vector2& other)
    {
        *this = *this + other;
        return *this;
    }

    // 減算
    Vector2 operator-(const Vector2& other) const { return DirectX::XMVectorSubtract(*this, other); }
    Vector2& operator-=(const Vector2& other)
    {
        *this = *this - other;
        return *this;
    }

    // 乗算 (スカラー)
    Vector2 operator*(float scalar) const { return DirectX::XMVectorScale(*this, scalar); }

    // 除算 (スカラー)
    Vector2 operator/(float scalar) const { return DirectX::XMVectorScale(*this, 1.0f / scalar); }

    // 便利関数
    float Length() const { return DirectX::XMVectorGetX(DirectX::XMVector2Length(*this)); }
    float LengthSq() const { return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(*this)); }
    Vector2 Normalize() const { return DirectX::XMVector2Normalize(*this); }
    float Dot(const Vector2& other) const { return DirectX::XMVectorGetX(DirectX::XMVector2Dot(*this, other)); }
    float Cross(const Vector2& other) const { return x * other.y - y * other.x; }  // 2D外積(スカラー)
};

// =======================================================================
// Rect
// =======================================================================
struct Rect
{
    float x, y, w, h;

    constexpr Rect() : x(0), y(0), w(0), h(0) {}
    constexpr Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
    constexpr Rect(const Vector2& pos, const Vector2& size) : x(pos.x), y(pos.y), w(size.x), h(size.y) {}

    float Left() const { return x; }
    float Right() const { return x + w; }
    float Top() const { return y; }
    float Bottom() const { return y + h; }

    Vector2 TopLeft() const { return {x, y}; }
    Vector2 TopRight() const { return {x + w, y}; }
    Vector2 BottomLeft() const { return {x, y + h}; }
    Vector2 BottomRight() const { return {x + w, y + h}; }
    Vector2 Center() const { return {x + w * 0.5f, y + h * 0.5f}; }

    bool Contains(const Vector2& p) const { return p.x >= x && p.x <= x + w && p.y >= y && p.y <= y + h; }

    Rect Inflate(float horizontal, float vertical) const { return {x - horizontal, y - vertical, w + horizontal * 2, h + vertical * 2}; }
    Rect Offset(const Vector2& offset) const { return {x + offset.x, y + offset.y, w, h}; }
};

// =======================================================================
// Vector3
// =======================================================================
class Vector3 : public DirectX::XMFLOAT3
{
public:
    using DirectX::XMFLOAT3::XMFLOAT3;

    constexpr Vector3() : DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) {}
    constexpr Vector3(float x, float y, float z) : DirectX::XMFLOAT3(x, y, z) {}
    constexpr Vector3(const DirectX::XMFLOAT3& v) : DirectX::XMFLOAT3(v) {}
    Vector3(const DirectX::XMVECTOR& v) { DirectX::XMStoreFloat3(this, v); }

    // キャスト演算子
    operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat3(this); }

    // 加算
    Vector3 operator+(const Vector3& other) const { return DirectX::XMVectorAdd(*this, other); }
    Vector3& operator+=(const Vector3& other)
    {
        *this = *this + other;
        return *this;
    }

    // 減算
    Vector3 operator-(const Vector3& other) const { return DirectX::XMVectorSubtract(*this, other); }
    Vector3& operator-=(const Vector3& other)
    {
        *this = *this - other;
        return *this;
    }

    // 乗算 (スカラー)
    Vector3 operator*(float scalar) const { return DirectX::XMVectorScale(*this, scalar); }

    // 除算 (スカラー)
    Vector3 operator/(float scalar) const { return DirectX::XMVectorScale(*this, 1.0f / scalar); }

    // 便利関数
    float Length() const { return DirectX::XMVectorGetX(DirectX::XMVector3Length(*this)); }
    float LengthSq() const { return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(*this)); }
    Vector3 Normalize() const { return DirectX::XMVector3Normalize(*this); }
    float Dot(const Vector3& other) const { return DirectX::XMVectorGetX(DirectX::XMVector3Dot(*this, other)); }
    Vector3 Cross(const Vector3& other) const { return DirectX::XMVector3Cross(*this, other); }

    // 座標変換
    Vector3 Transform(const Matrix& m) const;
    Vector3 TransformNormal(const Matrix& m) const;
    Vector3 RotateAround(const Vector3& center, const Vector3& axis, float angle) const;

    // 球面座標からデカルト座標への変換
    static Vector3 FromSpherical(float radius, float pitch, float yaw)
    {
        float cp = std::cos(pitch);
        float sp = std::sin(pitch);
        float cy = std::cos(yaw);
        float sy = std::sin(yaw);
        return Vector3(radius * cp * sy, radius * sp, radius * cp * cy);
    }

    // 配列一括変換
    static void TransformArray(const Vector3* input, size_t count, const Matrix& m, Vector3* output);
    static void TransformArray(const std::vector<Vector3>& input, const Matrix& m, std::vector<Vector3>& output);
};

// =======================================================================
// Vector4
// =======================================================================
class Vector4 : public DirectX::XMFLOAT4
{
public:
    using DirectX::XMFLOAT4::XMFLOAT4;

    constexpr Vector4() : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {}
    constexpr Vector4(float x, float y, float z, float w) : DirectX::XMFLOAT4(x, y, z, w) {}
    constexpr Vector4(const Vector3& v, float w) : DirectX::XMFLOAT4(v.x, v.y, v.z, w) {}
    constexpr Vector4(const DirectX::XMFLOAT4& v) : DirectX::XMFLOAT4(v) {}
    Vector4(const DirectX::XMVECTOR& v) { DirectX::XMStoreFloat4(this, v); }

    // キャスト演算子
    operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat4(this); }

    // 加算
    Vector4 operator+(const Vector4& other) const { return DirectX::XMVectorAdd(*this, other); }
    Vector4& operator+=(const Vector4& other)
    {
        *this = *this + other;
        return *this;
    }

    // 減算
    Vector4 operator-(const Vector4& other) const { return DirectX::XMVectorSubtract(*this, other); }
    Vector4& operator-=(const Vector4& other)
    {
        *this = *this - other;
        return *this;
    }

    // 乗算 (スカラー)
    Vector4 operator*(float scalar) const { return DirectX::XMVectorScale(*this, scalar); }

    // 除算 (スカラー)
    Vector4 operator/(float scalar) const { return DirectX::XMVectorScale(*this, 1.0f / scalar); }

    // 便利関数
    float Length() const { return DirectX::XMVectorGetX(DirectX::XMVector4Length(*this)); }
    float LengthSq() const { return DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(*this)); }
    Vector4 Normalize() const { return DirectX::XMVector4Normalize(*this); }
    float Dot(const Vector4& other) const { return DirectX::XMVectorGetX(DirectX::XMVector4Dot(*this, other)); }

    // 座標変換
    Vector4 Transform(const Matrix& m) const;
};

// =======================================================================
// Math Utilities
// =======================================================================
namespace Math {
constexpr float PI = 3.1415926535f;
constexpr float TwoPI = 6.2831853071f;
constexpr float HalfPI = 1.5707963267f;

inline float ToRadians(float degrees)
{
    return degrees * (PI / 180.0f);
}
inline float ToDegrees(float radians)
{
    return radians * (180.0f / PI);
}

constexpr bool IsPowerOfTwo(size_t n) { return n > 0 && (n & (n - 1)) == 0; }

// 複素数乗算 (SIMDを利用した高速化)
inline Vector2 ComplexMul(const Vector2& a, const Vector2& b)
{
    DirectX::XMVECTOR vA = DirectX::XMLoadFloat2(&a);
    DirectX::XMVECTOR vB = DirectX::XMLoadFloat2(&b);
    DirectX::XMVECTOR vBr = DirectX::XMVectorSwizzle<0, 0, 0, 0>(vB);
    DirectX::XMVECTOR vBi = DirectX::XMVectorSwizzle<1, 1, 1, 1>(vB);
    DirectX::XMVECTOR vA_swap = DirectX::XMVectorSwizzle<1, 0, 0, 0>(vA);

    // p1 = [a.x * b.x, a.y * b.x, ...]
    // p2 = [a.y * b.y, a.x * b.y, ...]
    DirectX::XMVECTOR p1 = DirectX::XMVectorMultiply(vA, vBr);
    DirectX::XMVECTOR p2 = DirectX::XMVectorMultiply(vA_swap, vBi);
    
    // sign = [-1, 1, 0, 0]
    // 求める値: real = a.x*b.x - a.y*b.y,  imag = a.y*b.x + a.x*b.y
    DirectX::XMVECTOR sign = DirectX::XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f);
    
    Vector2 result;
    DirectX::XMStoreFloat2(&result, DirectX::XMVectorAdd(p1, DirectX::XMVectorMultiply(p2, sign)));
    return result;
}

// =======================================================================
// Fast Fourier Transform (FFT)
// =======================================================================

// FFTキャッシュプロセッサ (サイズNの回転因子とビット反転配列を事前計算)
class FftProcessor
{
public:
    explicit FftProcessor(size_t size);
    void Process(std::vector<Vector2>& data, bool inverse = false) const;
    size_t GetSize() const { return m_size; }

private:
    size_t m_size;
    std::vector<size_t> m_bit_rev;
    std::vector<Vector2> m_twiddles;
};

// 単発版 FFT (内部で一時的に FftProcessor を生成して実行)
inline void FFT(std::vector<Vector2>& data, bool inverse = false)
{
    FftProcessor processor(data.size());
    processor.Process(data, inverse);
}

}  // namespace Math

// =======================================================================
// Matrix
// =======================================================================
class Matrix : public DirectX::XMFLOAT4X4
{
public:
    using DirectX::XMFLOAT4X4::XMFLOAT4X4;

    Matrix() { *this = Identity(); }
    Matrix(const DirectX::XMFLOAT4X4& m) : DirectX::XMFLOAT4X4(m) {}
    Matrix(const DirectX::XMMATRIX& m) { DirectX::XMStoreFloat4x4(this, m); }

    // キャスト演算子
    operator DirectX::XMMATRIX() const { return DirectX::XMLoadFloat4x4(this); }

    // 乗算 (行列同士)
    Matrix operator*(const Matrix& other) const { return DirectX::XMMatrixMultiply(*this, other); }
    Matrix& operator*=(const Matrix& other)
    {
        *this = *this * other;
        return *this;
    }

    // 便利関数
    static Matrix Identity() { return DirectX::XMMatrixIdentity(); }

    Matrix Transpose() const { return DirectX::XMMatrixTranspose(*this); }
    Matrix Inverse() const { return DirectX::XMMatrixInverse(nullptr, *this); }

    // 生成関数
    static Matrix Translation(float x, float y, float z) { return DirectX::XMMatrixTranslation(x, y, z); }
    static Matrix Translation(const Vector3& v) { return DirectX::XMMatrixTranslationFromVector(v); }

    static Matrix Scaling(float x, float y, float z) { return DirectX::XMMatrixScaling(x, y, z); }
    static Matrix Scaling(const Vector3& v) { return DirectX::XMMatrixScalingFromVector(v); }
    static Matrix Scaling(float s) { return DirectX::XMMatrixScaling(s, s, s); }

    static Matrix RotationX(float angle) { return DirectX::XMMatrixRotationX(angle); }
    static Matrix RotationY(float angle) { return DirectX::XMMatrixRotationY(angle); }
    static Matrix RotationZ(float angle) { return DirectX::XMMatrixRotationZ(angle); }
    static Matrix RotationAxis(const Vector3& axis, float angle) { return DirectX::XMMatrixRotationAxis(axis, angle); }

    static Matrix LookAtLH(const Vector3& eye, const Vector3& focus, const Vector3& up)
    {
        return DirectX::XMMatrixLookAtLH(eye, focus, up);
    }
    static Matrix LookToLH(const Vector3& eye, const Vector3& dir, const Vector3& up)
    {
        return DirectX::XMMatrixLookToLH(eye, dir, up);
    }
    static Matrix PerspectiveFovLH(float fov, float aspect, float nearZ, float farZ)
    {
        return DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
    }
    static Matrix OrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
    {
        return DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
    }
    static Matrix OrthographicOffCenterLH(float l, float r, float b, float t, float nearZ, float farZ)
    {
        return DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, nearZ, farZ);
    }
};

// =======================================================================
// Vector3::RotateAround Implementation
// =======================================================================
inline Vector3 Vector3::RotateAround(const Vector3& center, const Vector3& axis, float angle) const
{
    Matrix transform = Matrix::Translation(-center.x, -center.y, -center.z) * Matrix::RotationAxis(axis, angle) *
                       Matrix::Translation(center.x, center.y, center.z);
    return Transform(transform);
}

// =======================================================================
// Implementations
// =======================================================================
inline Vector3 Vector3::Transform(const Matrix& m) const
{
    return DirectX::XMVector3Transform(*this, m);
}

inline Vector3 Vector3::TransformNormal(const Matrix& m) const
{
    return DirectX::XMVector3TransformNormal(*this, m);
}

inline void Vector3::TransformArray(const Vector3* input, size_t count, const Matrix& m, Vector3* output)
{
    // 安全のためループ実装を使用 (XMVector3TransformStreamは型制約が厳しいため)
    for (size_t i = 0; i < count; ++i) {
        output[i] = input[i].Transform(m);
    }
}

inline void Vector3::TransformArray(const std::vector<Vector3>& input, const Matrix& m, std::vector<Vector3>& output)
{
    if (output.size() < input.size()) {
        output.resize(input.size());
    }
    TransformArray(input.data(), input.size(), m, output.data());
}

inline Vector4 Vector4::Transform(const Matrix& m) const
{
    return DirectX::XMVector4Transform(*this, m);
}

// =======================================================================
// FFT Implementations
// =======================================================================
namespace Math {

inline FftProcessor::FftProcessor(size_t size) : m_size(size)
{
    if (!IsPowerOfTwo(m_size)) return;
    
    // 1. ビット反転テーブルの事前計算
    m_bit_rev.resize(m_size);
    size_t log2n = 0;
    while ((1ULL << log2n) < m_size) {
        log2n++;
    }

    for (size_t i = 0; i < m_size; ++i) {
        size_t rev = 0;
        for (size_t j = 0; j < log2n; ++j) {
            if ((i >> j) & 1) rev |= (1ULL << (log2n - 1 - j));
        }
        m_bit_rev[i] = rev;
    }

    // 2. 回転因子(Twiddle Factors)の事前計算
    // W_N^k を N/2 個だけ保持する (各ステージではステップ読み渡しで再利用)
    m_twiddles.resize(m_size / 2);
    for (size_t i = 0; i < m_size / 2; ++i) {
        float angle = -TwoPI * static_cast<float>(i) / static_cast<float>(m_size);
        m_twiddles[i] = { std::cos(angle), std::sin(angle) };
    }
}

inline void FftProcessor::Process(std::vector<Vector2>& data, bool inverse) const
{
    if (data.size() != m_size || !IsPowerOfTwo(m_size)) return;

    // 1. 並び替え
    for (size_t i = 0; i < m_size; ++i) {
        if (i < m_bit_rev[i]) {
            std::swap(data[i], data[m_bit_rev[i]]);
        }
    }

    // 2. バタフライ演算
    for (size_t len = 2; len <= m_size; len <<= 1) {
        size_t half = len / 2;
        size_t step = m_size / len;
        
        for (size_t i = 0; i < m_size; i += len) {
            for (size_t j = 0; j < half; ++j) {
                Vector2 w = m_twiddles[j * step];
                if (inverse) {
                    w.y = -w.y; // IFFT用: 共役複素数にする
                }
                
                Vector2 u = data[i + j];
                Vector2 v = ComplexMul(data[i + j + half], w); // SIMD 乗算
                
                data[i + j] = u + v;
                data[i + j + half] = u - v;
            }
        }
    }

    // 3. IFFT時のスケーリング
    if (inverse) {
        float inv_n = 1.0f / static_cast<float>(m_size);
        for (auto& val : data) {
            val = val * inv_n;
        }
    }
}

} // namespace Math


// グローバル演算子オーバーロード (double * Vector)

inline Vector2 operator*(float s, const Vector2& v)
{
    return v * s;
}

inline Vector3 operator*(float s, const Vector3& v)
{
    return v * s;
}

inline Vector4 operator*(float s, const Vector4& v)
{
    return v * s;
}

#endif  // SIMD_MATH_H
