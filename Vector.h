// -*- C++ -*-

#ifndef _VIS_3D_VECTOR
#define _VIS_3D_VECTOR

#include <math.h>
#include <ostream>

using namespace std;

struct Vector3D {
public:
  float x, y, z;
  
  Vector3D() : x(0.0), y(0.0), z(0.0) { }
  Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
  ~Vector3D() { }

  static Vector3D init_random(void);
           // generate a random vector with all coordinates in the range [-1,1]

  float norm(void) const {
    return sqrt(x*x + y*y + z*z);
  }

  Vector3D normalize(void) const {
    float f = 1.0 / norm();
    return Vector3D(f*x, f*y, f*z);
  }

  static float dot_product(const Vector3D&a, const Vector3D& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
  }

  static Vector3D vec_product(const Vector3D& a, const Vector3D& b) {
    return Vector3D(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
  }

  friend Vector3D operator+(const Vector3D& a, const Vector3D& b) {
    return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
  }

  friend Vector3D& operator+=(Vector3D& a, const Vector3D& b) {
    a.x += b.x;  a.y += b.y;  a.z += b.z;
    return a;
  }

  friend Vector3D operator-(const Vector3D& a, const Vector3D& b) {
    return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
  }
  
  friend Vector3D operator*(float k, const Vector3D& a) {
    return Vector3D(k*a.x, k*a.y, k*a.z);
  }

  friend ostream& operator<<(ostream& os, const Vector3D& a) {
    os << "(" << a.x << ", " << a.y << ", " << a.z << ")";
    return os;
  }
};


#endif
