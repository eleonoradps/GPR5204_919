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

#include "maths/vector2.h"
#include "maths/maths_utils.h"

namespace maths
{
Vector2f Vector2f::operator+(const Vector2f rhs) const {
    return Vector2f(x + rhs.x, y + rhs.y);
}

Vector2f& Vector2f::operator+=(const Vector2f rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vector2f Vector2f::operator-(const Vector2f rhs) const {
    return Vector2f(x - rhs.x, y - rhs.y);
}

Vector2f& Vector2f::operator-=(const Vector2f rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2f Vector2f::operator*(const float scalar) const {
    return Vector2f(x * scalar, y * scalar);
}

Vector2f& Vector2f::operator*=(const float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2f Vector2f::operator/(const float scalar) const {
    return Vector2f(x / scalar, y / scalar);
}

Vector2f& Vector2f::operator/=(const float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

bool Vector2f::operator==(const Vector2f rhs) const {
    return Equal(x, rhs.x) && Equal(y, rhs.y);
}

bool Vector2f::operator!=(const Vector2f rhs) const {
    return !Equal(x, rhs.x) || !Equal(y, rhs.y);
}

// Allows to read value at index.
const float Vector2f::operator[](const size_t component) const {
    return coord[component];
}

float& Vector2f::operator[](const size_t component) {
	return coord[component];
}

// This function calculates the norm.
float Vector2f::Magnitude() const {
	return std::sqrt(x * x + y * y);
}

// This function calculates the squared length of a vector.
float Vector2f::SqrMagnitude() const {
	return x * x + y * y;
}

// This function does the Dot product of two vectors.
float Vector2f::Dot(const Vector2f v2) const { return Dot(*this, v2); }

float Vector2f::Dot(const Vector2f v1, const Vector2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
	
// This function does the Cross product of two vectors.
Vector3f Vector2f::Cross(const Vector2f v2) const {
    return Cross(*this, v2);
}

Vector3f Vector2f::Cross(const Vector2f v1, const Vector2f v2) {
    return Vector3f(0, 0, v1.x * v2.y - v1.y * v2.x);
}

// This functions calculates the angle between two vectors.
radian_t Vector2f::AngleBetween(const Vector2f v2) const {
    return AngleBetween(*this, v2);
}

radian_t Vector2f::AngleBetween(const Vector2f v1, const Vector2f v2) {
    const float dot = Dot(v1, v2);
    const float v1_magnitude = v1.Magnitude();
    const float v2_magnitude = v2.Magnitude();
    const radian_t angle = acos(dot / (v1_magnitude * v2_magnitude));
    return angle;
}

// This functions make a vector have a magnitude of 1.
Vector2f Vector2f::Normalized() const {
    const float magnitude = Magnitude();
    if (Equal(magnitude, 0)) {
        return Vector2f(0, 0);
    }
    return {x / magnitude, y / magnitude};
}

void Vector2f::Normalize() {
    *this = Normalized();
}


// The function Lerp linearly interpolates between two points.
Vector2f Vector2f::Lerp(const Vector2f v2, const float t) const {
    return Lerp(*this, v2, t);
}

Vector2f Vector2f::Lerp(const Vector2f v1, const Vector2f v2, const float t) {
    return v1 + (v2 - v1) * t;
}

// The function Slerp spherically interpolates between two vectors.
Vector2f Vector2f::Slerp(Vector2f v2, const float t) const {
    const double v1_magnitude = Magnitude();
    const double v2_magnitude = v2.Magnitude();
    const Vector2f v1 = *this / v1_magnitude;
    v2 /= v2_magnitude;
    float dot = Dot(v1, v2);
    // Makes sure dot value cannot be under -1
    dot = fmax(dot, -1.0f);
    // Makes sure dot value cannot be over 1
    dot = fmin(dot, 1.0f);

    const radian_t theta = acos(dot) * t;
    const Vector2f relative_vec = (v2 - v1 * dot).Normalized();
    const Vector2f new_vec = v1 * maths::cos(theta) + relative_vec *
                             maths::sin(theta);
    return new_vec * (v1_magnitude + (v2_magnitude - v1_magnitude) * t);
}

// The function rotate a vector depending on an angle.
Vector2f Vector2f::Rotation(const radian_t angle) const {
    return Rotation(*this, angle);
}

Vector2f Vector2f::Rotation(const Vector2f v1, const radian_t angle) {
    return Vector2f(
        (v1.x * cos(angle)) + (v1.y * -sin(angle)),
        (v1.x * sin(angle)) + (v1.y * cos(angle)));
}
} // namespace maths
