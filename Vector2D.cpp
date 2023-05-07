#include "Vector2D.h"
#include "math.h"

Vector2D::Vector2D()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D::~Vector2D() {}

double Vector2D::len() const
{
    return sqrt(x * x + y * y);
}

double Vector2D::getAngleRadians()
{
    if (len() == 0) return 1;
    double sin = y / len();
    return asin(sin);
}

double Vector2D::getAngleDegrees360()
{
    if (len() == 0) return 1;
    double tempy = -y;
    if (x*y < 0) tempy = -y;
    double sin = tempy / len();
    return asin(sin) * 180 / M_PI;
}
Vector2D Vector2D::Abs()
{
    Vector2D res = *this;
    res.x = abs(res.x);
    res.y = abs(res.y);
    return res;
}
double Vector2D::getAngleDegrees()
{
    return getAngleRadians() * 180 / M_PI;
}

void Vector2D::normalize()
{
    float oldX = x, oldY = y;
    if (x == 0 && y == 0) return;
    x /= sqrt(oldX * oldX + oldY * oldY);
    y /= sqrt(oldX * oldX + oldY * oldY);
}

Vector2D &Vector2D::Add(const Vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;

    return *this;
}
Vector2D &Vector2D::Subtract(const Vector2D &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;

    return *this;
}
Vector2D &Vector2D::Multiply(const Vector2D &vec)
{
    this->x *= vec.x;
    this->y *= vec.y;

    return *this;
}
Vector2D &Vector2D::Divide(const Vector2D &vec)
{
    this->x /= vec.x;
    this->y /= vec.y;

    return *this;
}

Vector2D &operator+(Vector2D &v1, const Vector2D &v2)
{
    return v1.Add(v2);
}
Vector2D &operator-(Vector2D &v1, const Vector2D &v2)
{
    return v1.Subtract(v2);
}
Vector2D &operator*(Vector2D &v1, const Vector2D &v2)
{
    return v1.Multiply(v2);
}
Vector2D &operator/(Vector2D &v1, const Vector2D &v2)
{
    return v1.Divide(v2);
}

Vector2D &Vector2D::operator+=(const Vector2D &vec)
{
    return this->Add(vec);
}
Vector2D &Vector2D::operator-=(const Vector2D &vec)
{
    return this->Subtract(vec);
}
Vector2D &Vector2D::operator*=(const Vector2D &vec)
{
    return this->Multiply(vec);
}
Vector2D &Vector2D::operator/=(const Vector2D &vec)
{
    return this->Divide(vec);
}

Vector2D &Vector2D::operator*(const int &i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

Vector2D &Vector2D::Zero()
{
    this->x = 0;
    this->y = 0;
    return *this;
}

std::ostream &operator<<(std::ostream &stream, const Vector2D &vec)
{
    stream << "(" << vec.x << ", " << vec.y << ")";
    return stream;
}