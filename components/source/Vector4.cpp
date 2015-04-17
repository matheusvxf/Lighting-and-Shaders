#include "Vector4.h"

#include <sstream>
#include <iostream>

#define X v[0]
#define Y v[1]
#define Z v[2]
#define W w

Vector4::Vector4() : Vector4(0, 0, 0, 0) {}

Vector4::Vector4(float *v, double w) : Vector3(v), w(w) {}

Vector4::Vector4(double x, double y, double z) : Vector4(x, y, z, 0) {}

Vector4::Vector4(double x, double y, double z, double w) : Vector3(x, y, z), w(w) {}

Vector4::Vector4(const Vector3& a) : Vector3(a), w(0) {}

Vector4::~Vector4() {}

Vector4& Vector4::operator=(const Vector4& v)
{
    Vector3::operator=(v);
    W = v.w;
    return (*this);
}

double Vector4::operator[](int index) const
{
    return index < 3 ? v[index] : W;
}

double& Vector4::operator[](int index)
{
    return index < 3 ? v[index] : W;
}

void Vector4::dehomogenize(){
    if (W >= 0.01 || W <= -0.01)
    {
        X /= W;
        Y /= W;
        Z /= W;
        W /= W;
    }
}

void Vector4::set(double x, double y, double z, double w)
{
    X = x;
    Y = y;
    Z = z;
    W = w;
}

std::string Vector4::toString() const{
	std::stringstream buffer;
	buffer.precision(2);
	buffer << "[" << X << "," << Y << "," << Z << "," << W << "]" << std::fixed;
	return buffer.str();
}
