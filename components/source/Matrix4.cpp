#include <iostream>
#include <sstream>
#include <cmath>

#include "Utility.h"
#include "Matrix4.h"

const Matrix4 Matrix4::matrix_identity(Matrix4::MatrixType::tIdentity);

Matrix4::Matrix4row::Matrix4row(Matrix4* owner, UINT8 row)
{
    this->owner = owner;
    this->row = row;
}

Matrix4::Matrix4row& Matrix4::Matrix4row::operator=(const Vector4& vector)
{
    (*owner)[row][0] = vector[0];
    (*owner)[row][1] = vector[1];
    (*owner)[row][2] = vector[2];
    return *this;
}

double Matrix4::Matrix4row::operator[](UINT8 column) const
{
    return (*owner).m[row][column];
}

double& Matrix4::Matrix4row::operator[](UINT8 column)
{
    return (*owner).m[row][column];
}

Matrix4::Matrix4()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            m[i][j] = 0;
        }
    }
}

Matrix4::Matrix4(MatrixType type) : Matrix4()
{
    if (type == MatrixType::tIdentity)
    {
        this->identity();
    }
}

Matrix4::Matrix4(MatrixType type, const Vector3& v) : Matrix4()
{
    if (type == MatrixType::tTranslate)
        this->makeTranslate(v[0], v[1], v[2]);
}

Matrix4::Matrix4(const Matrix4& m)
{
    (*this) = m;
}

Matrix4* Matrix4::clone()
{
    return new Matrix4(*this);
}


Matrix4& Matrix4::operator=(const Matrix4& m2)
{
    if (this != &m2)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                m[i][j] = m2.m[i][j];
            }
        }
    }
    return *this;
}

Matrix4& Matrix4::operator=(GLdouble *matrix)
{
    UINT8 k = 0;
    for (UINT8 j = 0; j < 4; ++j)
    {
        for (UINT8 i = 0; i < 4; ++i, ++k)
        {
            m[i][j] = matrix[k];
        }
    }
    return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& m) const
{
    Matrix4 result;

    for (register int i = 0; i < 4; ++i)
    {
        for (register int j = 0; j < 4; ++j)
        {
            for (register int k = 0; k < 4; ++k)
            {
                result.m[i][j] += this->m[i][k] * m.m[k][j];
            }
        }
    }

    return result;
}

Matrix4& Matrix4::operator*=(const Matrix4 &m)
{
    (*this) = (*this) * m;
    return *this;
}

Vector4 Matrix4::operator*(const Vector4& v) const
{
    Vector4 result;
    result[3] = 0;

    for (register int i = 0; i < 4; ++i)
    {
        for (register int j = 0; j < 4; ++j)
        {
            result[i] += this->m[i][j] * v[j];
        }
    }

    return result;
}

Matrix4 Matrix4::operator*(double s) const
{
    Matrix4 result = (*this);
    result *= s;
    return result;
}

Matrix4& Matrix4::operator*=(double s)
{
    for (UINT8 i = 0; i < 4; ++i)
    {
        for (UINT8 j = 0; j < 4; ++j)
        {
            m[i][j] *= s;
        }
    }
    return (*this);
}

Matrix4 Matrix4::operator-() const
{
    return (*this) * -1;
}

Matrix4::Matrix4row Matrix4::operator[](UINT8 row)
{
    return Matrix4row(this, row);
}

void Matrix4::setColumn(UINT8 column, const Vector4& v)
{
    m[0][column] = v[0];
    m[1][column] = v[1];
    m[2][column] = v[2];
}

// return pointer to matrix elements
const double* Matrix4::getPointer() const
{
    return &m[0][0];
}

// set matrix to identity matrix
void Matrix4::identity()
{
    for (register UINT8 i = 0; i < 4; ++i)
    {
        for (register UINT8 j = 0; j < 4; ++j)
        {
            if (i == j)
                m[i][j] = 1.0;
            else
                m[i][j] = 0.0;
        }
    }
}

// transpose the matrix (mirror at diagonal)
Matrix4& Matrix4::transpose()
{
    Matrix4 temp;
    for (register UINT8 i = 0; i < 4; ++i)
    {
        for (register UINT8 j = 0; j < 4; ++j)
        {
            temp.m[j][i] = m[i][j];
        }
    }
    *this = temp;  // copy temporary values to this matrix
    return (*this);
}

Matrix4& Matrix4::make_skew(const Vector3& v)
{
    identity();
    m[0][0] = 0; m[0][1] = -v[2]; m[0][2] = v[1];
    m[1][0] = v[2]; m[1][1] = 0; m[1][2] = -v[0];
    m[2][0] = -v[1]; m[2][1] = v[0]; m[2][2] = 0;
    return (*this);
}

Matrix4& Matrix4::makeRotateX(double angle)
{
    angle = degree2rad(angle);
    identity();
    m[1][1] = cos(angle);
    m[1][2] = -sin(angle);
    m[2][1] = sin(angle);
    m[2][2] = cos(angle);
    return (*this);
}

// Creates a rotation matrix which rotates about the y axis.
// angle is expected in degrees
Matrix4& Matrix4::makeRotateY(double angle)
{
    angle = degree2rad(angle);
    identity();
    m[0][0] = cos(angle);
    m[0][2] = sin(angle);
    m[2][0] = -sin(angle);
    m[2][2] = cos(angle);
    return (*this);
}

Matrix4& Matrix4::makeRotateZ(double angle)
{
    angle = degree2rad(angle);
    identity();
    m[0][0] = cos(angle);
    m[0][1] = -sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
    return (*this);
}

Matrix4& Matrix4::makeRotate(double angle, const Vector3& axis)
{
    Vector4 u, v, w;
    Matrix4 m2xyz, m2uvw, m_rotation;
   
    w = axis;
    
    if (w[0] != 0)
    {
        u[0] = -(w[1] + w[2]) / w[0];
        u[1] = 1;
        u[2] = 1;
    }
    else if (w[1] != 0)
    {
        u[0] = 1;
        u[1] = -(w[0] + w[2]) / w[1];
        u[2] = 1;
    }
    else if (w[2] != 0)
    {
        u[0] = 1;
        u[1] = 1;
        u[2] = -(w[0] + w[1]) / w[2];
    }

    v = w.cross(u);
    u.normalize();
    v.normalize();
    w.normalize();
    identity();
    m2xyz.identity();
    m2uvw.identity();
    m2xyz[0] = u;
    m2xyz[1] = v;
    m2xyz[2] = w;
    m2uvw.setColumn(0, u);
    m2uvw.setColumn(1, v);
    m2uvw.setColumn(2, w);
    m_rotation.makeRotateZ(angle);
    (*this) = m2uvw * m_rotation * m2xyz;
    return (*this);
}

Matrix4& Matrix4::makeScale(double sx, double sy, double sz)
{
    identity();
    m[0][0] = sx;
    m[1][1] = sy;
    m[2][2] = sz;
    return (*this);
}

Matrix4& Matrix4::makeScale(double s)
{
    return makeScale(s, s, s);
}

Matrix4& Matrix4::makeTranslate(double sx, double sy, double sz)
{
    identity();
    m[0][3] = sx;
    m[1][3] = sy;
    m[2][3] = sz;
    return (*this);
}

void Matrix4::print(const std::string& s) const
{
    std::cout << s << " : " << std::endl << toString();
}

std::string Matrix4::toString() const
{
    std::stringstream buffer;
    buffer.precision(2);
    for (register UINT8 i = 0; i < 4; ++i)
    {
        buffer << "|";
        for (register UINT8 j = 0; j < 3; ++j)
        {
            buffer << std::fixed << m[i][j] << ",";
        }
        buffer << m[i][3] << "|" << std::endl << std::fixed;
    }
    return buffer.str();
}