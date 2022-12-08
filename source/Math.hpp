#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <cmath>

#define RADIAN(x) (x * M_PI / 180.0f)

struct vec2 {
    float x = 0.0f, y = 0.0f;
};

struct vec3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    vec3 operator- (void) { return { -x, -y, -z }; };
    vec3 operator- (const vec3& rhs) const { return{ x - rhs.x, y - rhs.y, z - rhs.z }; }
    vec3 operator+ (const vec3& rhs) const { return{ x + rhs.x, y + rhs.y, z + rhs.z }; }
    float operator* (const vec3& rhs) const { return{ x * rhs.x + y * rhs.y + z * rhs.z }; } // DOT PRODUCT
    vec3 operator^ (const vec3& rhs) const { return{ y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x }; } // CROSS PRODUCT
    vec3 operator/ (const float& rhs) const { return{ x / rhs, y / rhs, z / rhs }; }
    vec3 operator* (const float& rhs)const { return{ x * rhs, y * rhs, z * rhs }; }
    vec3 Normalize() {
        float divisor = sqrt(x * x + y * y + z * z);
        return { x / divisor, y / divisor, z / divisor };
    }
};

struct vec4 {
    float x = 0.0f, y = 0.0f, z = 0.0f, t = 0.0f;
};

struct mat4 {
    vec4 x = { 1.0f, 0.0f, 0.0f, 0.0f };
    vec4 y = { 0.0f, 1.0f, 0.0f, 0.0f };
    vec4 z = { 0.0f, 0.0f, 1.0f, 0.0f };
    vec4 t = { 0.0f, 0.0f, 0.0f, 1.0f };
    mat4 operator* (mat4 const& rhs) {
        return {
            {(x.x * rhs.x.x) + (x.y * rhs.y.x) + (x.z * rhs.z.x) + (x.t * rhs.t.x),/**/ (x.x * rhs.x.y) + (x.y * rhs.y.y) + (x.z * rhs.z.y) + (x.t * rhs.t.y),/**/ (x.x * rhs.x.z) + (x.y * rhs.y.z) + (x.z * rhs.z.z) + (x.t * rhs.t.z),/**/ (x.x * rhs.x.t) + (x.y * rhs.y.t) + (x.z * rhs.z.t) + (x.t * rhs.t.t)},
            {(y.x * rhs.x.x) + (y.y * rhs.y.x) + (y.z * rhs.z.x) + (y.t * rhs.t.x),/**/ (y.x * rhs.x.y) + (y.y * rhs.y.y) + (y.z * rhs.z.y) + (y.t * rhs.t.y),/**/ (y.x * rhs.x.z) + (y.y * rhs.y.z) + (y.z * rhs.z.z) + (y.t * rhs.t.z),/**/ (y.x * rhs.x.t) + (y.y * rhs.y.t) + (y.z * rhs.z.t) + (y.t * rhs.t.t)},
            {(z.x * rhs.x.x) + (z.y * rhs.y.x) + (z.z * rhs.z.x) + (z.t * rhs.t.x),/**/ (z.x * rhs.x.y) + (z.y * rhs.y.y) + (z.z * rhs.z.y) + (z.t * rhs.t.y),/**/ (z.x * rhs.x.z) + (z.y * rhs.y.z) + (z.z * rhs.z.z) + (z.t * rhs.t.z),/**/ (z.x * rhs.x.t) + (z.y * rhs.y.t) + (z.z * rhs.z.t) + (z.t * rhs.t.t)},
            {(t.x * rhs.x.x) + (t.y * rhs.y.x) + (t.z * rhs.z.x) + (t.t * rhs.t.x),/**/ (t.x * rhs.x.y) + (t.y * rhs.y.y) + (t.z * rhs.z.y) + (t.t * rhs.t.y),/**/ (t.x * rhs.x.z) + (t.y * rhs.y.z) + (t.z * rhs.z.z) + (t.t * rhs.t.z),/**/ (t.x * rhs.x.t) + (t.y * rhs.y.t) + (t.z * rhs.z.t) + (t.t * rhs.t.t)},
        };
    }
    void Scaling(vec3 const& v) {
        mat4 mat;
        mat.x.x = v.x;
        mat.y.y = v.y;
        mat.z.z = v.z;
        *this = (mat * *this);
    }
    void RotateArbitraryAxis(vec3 rotate, float const& degree) {
        rotate = rotate.Normalize();
        float c = std::cos(degree), s = std::sin(degree), t = 1.0f - c;
        float x = rotate.x, y = rotate.y, z = rotate.z;
        *this = mat4{
            {{ t * x * x + c     }, { t * x * y - s * z }, { t * x * z + s * y }},
            {{ t * x * y + s * z }, { t * y * y + c     }, { t * y * z - s * x }},
            {{ t * x * z - s * y }, { t * y * z + s * x }, { t * z * z + c     }}
        } * *this;
    }
    void Translate(vec3 const& pos) {
        mat4 mat;
        mat.t.x = pos.x;
        mat.t.y = pos.y;
        mat.t.z = pos.z;
        *this = (mat * *this);
    }
};

inline vec3 GetLinePlaneIntersection(vec3 const& rayOrigin, vec3 const& rayVector, vec3 const& planePosition, vec3 const& planeNormal) {
    vec3 diff = planePosition - rayOrigin; float dot1 = planeNormal * diff, dot2 = planeNormal * rayVector;
    if (dot2) return rayOrigin + rayVector * dot1 / dot2;
    return rayOrigin;
}

inline mat4 Ortho(
    const float& left, const float& right, const float& bottom, const float& top,
    const float& zNear, const float& zFar)
{
    float tx = (right + left) / (right - left), ty = (top + bottom) / (top - bottom), tz = (zFar + zNear) / (zFar - zNear);
    return {
        { 2.0f / (right - left), 0.0f, 0.0f, tx },
        { 0.0f, 2.0f / (top - bottom), 0.0f, ty },
        { 0.0f, 0.0f, -2.0f / -(zFar + zNear), tz },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };
}

inline mat4 Perspective(float const& fov, float const& aspect, float const& zNear, float const& zFar)
{
    float tanHalfFov = (float)tan(RADIAN((double)fov) / 2.0);
    return {
        { 1.0f / (aspect * tanHalfFov), 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f / (tanHalfFov), 0.0f, 0.0f },
        { 0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear),-1.0f },
        { 0.0f, 0.0f, -(2.0f * zFar * zNear) / (zFar - zNear), 0.0f }
    };
}

inline mat4 LookAt(vec3  const& eye, vec3  const& center, vec3  const& up)
{
    vec3 cameraDirection = ((eye - center)).Normalize();
    vec3 cameraRight = ((cameraDirection ^ up)).Normalize();
    vec3 cameraUp = ((cameraRight ^ cameraDirection)).Normalize();
    return {
        {cameraRight.x, cameraUp.x, cameraDirection.x, 0.0f},
        {cameraRight.y, cameraUp.y, cameraDirection.y, 0.0f},
        {cameraRight.z, cameraUp.z, cameraDirection.z, 0.0f},
        {-(cameraRight * eye), -(cameraUp * eye), -(cameraDirection * eye), 1.0f}
    };
}

#endif
