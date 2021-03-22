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

#include "maths/vector3.h"
#include "maths/angle.h"
#include "maths/maths_utils.h"
#include <cmath>

namespace maths {
Vector3f::Vector3f(float x, float y, float z)
    : x(x),
      y(y),
      z(z) {
}

Vector3f Vector3f::operator+(const Vector3f& rhs) const {
    return {x + rhs.x, y + rhs.y, z + rhs.z};
}

Vector3f& Vector3f::operator+=(const Vector3f& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector3f Vector3f::operator-(const Vector3f& rhs) const {
    return {x - rhs.x, y - rhs.y, z - rhs.z};
}

Vector3f& Vector3f::operator-=(const Vector3f& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector3f Vector3f::operator*(const float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

Vector3f& Vector3f::operator*=(const float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3f Vector3f::operator/(const float scalar) const {
    return {x / scalar, y / scalar, z / scalar};
}

Vector3f& Vector3f::operator/=(const float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

bool Vector3f::operator==(const Vector3f& rhs) const {
    return Equal(x, rhs.x) && Equal(y, rhs.y) && Equal(z, rhs.z);
}

bool Vector3f::operator!=(const Vector3f& rhs) const {
    return !Equal(x, rhs.x) || !Equal(y, rhs.y) || !Equal(z, rhs.z);
}

// This function does the Dot product of three vectors.
float Vector3f::Dot(const Vector3f& v2) const {
    return Dot(*this, v2);
}

float Vector3f::Dot(const Vector3f& v1, const Vector3f& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// This function does the Cross product of three vectors.
Vector3f Vector3f::Cross(const Vector3f& v2) const {
    return Cross(*this, v2);
}

Vector3f Vector3f::Cross(const Vector3f& v1, const Vector3f& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

// This function calculates the norm.
float Vector3f::Magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

// This function calculates the squared length of a vector.
float Vector3f::SqrMagnitude() const {
    return x * x + y * y + z * z;
}

// This function calculates the angle between two vectors.
radian_t Vector3f::AngleBetween(const Vector3f& v2) const {
    return AngleBetween(*this, v2);
}

radian_t Vector3f::AngleBetween(const Vector3f& v1, const Vector3f& v2) {
    const float dot = Dot(v1, v2);
    const float otherMagnitude1 = v1.Magnitude();
    const float otherMagnitude2 = v2.Magnitude();
    return {maths::acos(dot / (otherMagnitude1 * otherMagnitude2))};
}

// Allows to read value at index.
float Vector3f::operator[](std::size_t index) const {
    return coord[index];
}

// Allows to write value at index.
float& Vector3f::operator[](std::size_t index) {
    return coord[index];
}

// This function makes a vector have a magnitude of 1.
Vector3f Vector3f::Normalized() const {
    const float magnitude = Magnitude();

    if (Equal(magnitude, 0)) {
        return Vector3f(0, 0, 0);
    }

    return {x / magnitude, y / magnitude, z / magnitude};
}

void Vector3f::Normalize() {
    const float magnitude = Magnitude();

    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

// The function Lerp linearly interpolates between two points.
Vector3f Vector3f::Lerp(const Vector3f& v2, const float t) const {
    return Lerp(*this, v2, t);
}

Vector3f Vector3f::Lerp(const Vector3f& v1, const Vector3f& v2, const float t) {
    return v1 + (v2 - v1) * t;
}

// The function Slerp spherically interpolates between two vectors.
Vector3f Vector3f::Slerp(Vector3f& v2, const float t) const {
    const float magnitude_v1 = Magnitude();
    const float magnitude_v2 = v2.Magnitude();
    const Vector3f v1 = *this / magnitude_v1;
    v2 /= magnitude_v2;
    float dot = Dot(v1, v2);
    // Makes sure dot value cannot be under -1.
    dot = fmax(dot, -1.0f);
    // Makes sure dot value cannot be over 1.
    dot = fmin(dot, 1.0f);

    const radian_t theta = maths::acos(dot) * t;
    const Vector3f relative_vec = (v2 - v1 * dot).Normalized();
    const Vector3f new_vec =
        v1 * maths::cos(theta) + relative_vec * maths::sin(theta);
    return new_vec * (magnitude_v1 + (magnitude_v2 - magnitude_v1) * t);
}
} // namespace maths
