#pragma once

#include <cmath>
#include <iostream>
#include "vec4.h"

/* Represents a 4x4 matrix in row-major order. */
class Mat4 {
    float m[4][4];

public:

    Mat4() {
        setIdentity();
    }

    Mat4(float diagonal) {
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                m[row][col] = (row == col) ? diagonal : 0.0f;
            }
        }
    }

    static Mat4 identity() {
        return Mat4(1.0f);
    }

    static Mat4 zero() {
        Mat4 result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.m[row][col] = 0.0f;
            }
        }
        return result;
    }

    static Mat4 translation(const Vec3& t) {
        Mat4 result = Mat4::identity();
        result.m[0][3] = t.x;
        result.m[1][3] = t.y;
        result.m[2][3] = t.z;
        return result;
    }

    static Mat4 scale(const Vec3& s) {
        Mat4 result = Mat4::identity();
        result.m[0][0] = s.x;
        result.m[1][1] = s.y;
        result.m[2][2] = s.z;
        return result;
    }

    static Mat4 rotationX(float angle) {
        Mat4 result = Mat4::identity();
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        result.m[1][1] = cos;
        result.m[1][2] = -sin;
        result.m[2][1] = sin;
        result.m[2][2] = cos;
        return result;
    }

    static Mat4 rotationY(float angle) {
        Mat4 result = Mat4::identity();
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        result.m[0][0] = cos;
        result.m[0][2] = sin;
        result.m[2][0] = -sin;
        result.m[2][2] = cos;
        return result;
    }

    static Mat4 rotationZ(float angle) {
        Mat4 result = Mat4::identity();
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        result.m[0][0] = cos;
        result.m[0][1] = -sin;
        result.m[1][0] = sin;
        result.m[1][1] = cos;
        return result;
    }

    /**
     * Returns a rotation matrix around the origin.
     * Angles are in radians.
     */
    static Mat4 rotation(float angleX, float angleY, float angleZ) {
        return rotationZ(angleZ) * rotationY(angleY) * rotationX(angleX);
    }

    static Mat4 fromBasis(const Vec3 &u, const Vec3 &v, const Vec3 &n) {
        Mat4 result = Mat4::identity();
        result.m[0][0] = u.x; result.m[0][1] = u.y; result.m[0][2] = u.z;
        result.m[1][0] = v.x; result.m[1][1] = v.y; result.m[1][2] = v.z;
        result.m[2][0] = n.x; result.m[2][1] = n.y; result.m[2][2] = n.z;
        return result;
    }

    /**
     * Returns a matrix from four row vectors.
     */
    static Mat4 fromRows(const Vec4 &u, const Vec4 &v, const Vec4 &w, const Vec4 &x) {
        Mat4 result = Mat4::zero();
        result.m[0][0] = u.x; result.m[0][1] = u.y; result.m[0][2] = u.z; result.m[0][3] = u.w;
        result.m[1][0] = v.x; result.m[1][1] = v.y; result.m[1][2] = v.z; result.m[1][3] = v.w;
        result.m[2][0] = w.x; result.m[2][1] = w.y; result.m[2][2] = w.z; result.m[2][3] = w.w;
        result.m[3][0] = x.x; result.m[3][1] = x.y; result.m[3][2] = x.z; result.m[3][3] = x.w;
        return result;
    }

    Mat4 transpose() const {
        Mat4 result = Mat4::zero();
        for (int row = 0; row < 4; ++row)
            for (int col = 0; col < 4; ++col)
                result.m[row][col] = m[col][row];
        return result;
    }

    /**
     * Inverse of a 4x4 matrix.
     * Adapted from here: https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
     */ 
    Mat4 inverse() const {
        float inv[16], det;
        const float* m = &this->m[0][0];

        inv[0]  =  m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15] + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
        inv[4]  = -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15] - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
        inv[8]  =  m[4]*m[9]*m[15]  - m[4]*m[11]*m[13] - m[8]*m[5]*m[15] + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
        inv[12] = -m[4]*m[9]*m[14]  + m[4]*m[10]*m[13] + m[8]*m[5]*m[14] - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
        inv[1]  = -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15] - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
        inv[5]  =  m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15] + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
        inv[9]  = -m[0]*m[9]*m[15]  + m[0]*m[11]*m[13] + m[8]*m[1]*m[15] - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
        inv[13] =  m[0]*m[9]*m[14]  - m[0]*m[10]*m[13] - m[8]*m[1]*m[14] + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
        inv[2]  =  m[1]*m[6]*m[15]  - m[1]*m[7]*m[14]  - m[5]*m[2]*m[15] + m[5]*m[3]*m[14] + m[13]*m[2]*m[7]  - m[13]*m[3]*m[6];
        inv[6]  = -m[0]*m[6]*m[15]  + m[0]*m[7]*m[14]  + m[4]*m[2]*m[15] - m[4]*m[3]*m[14] - m[12]*m[2]*m[7]  + m[12]*m[3]*m[6];
        inv[10] =  m[0]*m[5]*m[15]  - m[0]*m[7]*m[13]  - m[4]*m[1]*m[15] + m[4]*m[3]*m[13] + m[12]*m[1]*m[7]  - m[12]*m[3]*m[5];
        inv[14] = -m[0]*m[5]*m[14]  + m[0]*m[6]*m[13]  + m[4]*m[1]*m[14] - m[4]*m[2]*m[13] - m[12]*m[1]*m[6]  + m[12]*m[2]*m[5];
        inv[3]  = -m[1]*m[6]*m[11]  + m[1]*m[7]*m[10]  + m[5]*m[2]*m[11] - m[5]*m[3]*m[10] - m[9]*m[2]*m[7]   + m[9]*m[3]*m[6];
        inv[7]  =  m[0]*m[6]*m[11]  - m[0]*m[7]*m[10]  - m[4]*m[2]*m[11] + m[4]*m[3]*m[10] + m[8]*m[2]*m[7]   - m[8]*m[3]*m[6];
        inv[11] = -m[0]*m[5]*m[11]  + m[0]*m[7]*m[9]   + m[4]*m[1]*m[11] - m[4]*m[3]*m[9]  - m[8]*m[1]*m[7]   + m[8]*m[3]*m[5];
        inv[15] =  m[0]*m[5]*m[10]  - m[0]*m[6]*m[9]   - m[4]*m[1]*m[10] + m[4]*m[2]*m[9]  + m[8]*m[1]*m[6]   - m[8]*m[2]*m[5];

        det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
        if (std::abs(det) < 1e-6f) return Mat4::identity(); // not invertible

        det = 1.0f / det;
        Mat4 result = Mat4::zero();
        for (int i = 0; i < 16; ++i)
            (&result.m[0][0])[i] = inv[i] * det;
        return result;
    }

    Mat4 inverseTranspose() const {
        return inverse().transpose();
    }

    void print() const {
        for (size_t i = 0; i <  4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                std::cout << m[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    ///
    ////// * Operator Overloads
    ///

    Mat4 operator*(const Mat4& other) const {
        Mat4 result = Mat4::zero();
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                float sum = 0.0f;
                for (int i = 0; i < 4; ++i) {
                    sum += m[row][i] * other.m[i][col];
                }
                result.m[row][col] = sum;
            }
        }
        return result;
    }

    Vec4 operator*(const Vec4& v) const {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
            m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
        };
    }

private:
    void setIdentity() {
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                m[row][col] = (row == col) ? 1.0f : 0.0f;
            }
        }
    }
};