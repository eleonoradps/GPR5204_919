/*
MIT License

Copyright (c) 2021 SAE Institute Geneva

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include "maths/angle.h"

namespace maths {
/**
 *  \brief Class used to represent a 3D vector.
 */
class Vector3f {
public:
    union {
        struct {
            float x;
            float y;
            float z;
        };

        float coord[3]{};
    };

    Vector3f()
        : x(0),
          y(0),
          z(0) {
    }

    Vector3f(float x, float y, float z);

    Vector3f operator+(const Vector3f& rhs) const;

    Vector3f& operator+=(const Vector3f& rhs);

    Vector3f operator-(const Vector3f& rhs) const;

    Vector3f& operator-=(const Vector3f& rhs);

    Vector3f operator*(float scalar) const;

    Vector3f& operator*=(float scalar);

    Vector3f operator/(float scalar) const;

    Vector3f& operator/=(float scalar);

    bool operator==(const Vector3f& rhs) const;

    bool operator!=(const Vector3f& rhs) const;

    // This function does the Dot product of three vectors.
    float Dot(const Vector3f& v2) const;

    static float Dot(const Vector3f& v1, const Vector3f& v2);

    // This function does the Cross product of three vectors.
    Vector3f Cross(const Vector3f& v2) const;

    static Vector3f Cross(const Vector3f& v1, const Vector3f& v2);

    // This function calculates the norm.
    float Magnitude() const;

    // This function calculates the squared length of a vector.
    float SqrMagnitude() const;

    // This function calculates the angle between two vectors.
    radian_t AngleBetween(const Vector3f& v2) const;

    static radian_t AngleBetween(const Vector3f& v1, const Vector3f& v2);

    // Allows to read value at index.
    float operator[](std::size_t index) const;

    // Allows to write value at index.
    float& operator[](std::size_t index);

    // This function makes a vector have a magnitude of 1.
    Vector3f Normalized() const;

    void Normalize();

    // The function Lerp linearly interpolates between two points.
    Vector3f Lerp(const Vector3f& v2, float t) const;

    static Vector3f Lerp(const Vector3f& v1, const Vector3f& v2, float t);

    // The function Slerp spherically interpolates between two vectors.
    Vector3f Slerp(Vector3f& v2, float t) const;
};
} // namespace maths
