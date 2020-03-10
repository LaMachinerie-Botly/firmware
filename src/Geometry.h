#include <Arduino.h>

class GVector2D{
public:

    float x, y, z;

    GVector2D(): x(0), y(0), z(0){}
    GVector2D(float _x, float _y) : x(_x), y(_y), z(0){}
    GVector2D(float _x, float _y, float _z) : x(_x), y(_y),z(_z){}
    GVector2D(const GVector2D& v) : x(v.x), y(v.y), z(v.z) {}

    GVector2D& operator=(const GVector2D& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	
	GVector2D operator+(GVector2D& v) {
		return GVector2D(x + v.x, y + v.y);
	}
	GVector2D operator-(GVector2D& v) {
		return GVector2D(x - v.x, y - v.y);
	}
	
	GVector2D& operator+=(GVector2D& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	GVector2D& operator-=(GVector2D& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	GVector2D operator+(double s) {
		return GVector2D(x + s, y + s);
	}
	GVector2D operator-(double s) {
		return GVector2D(x - s, y - s);
	}
	GVector2D operator*(double s) {
		return GVector2D(x * s, y * s);
	}
	GVector2D operator/(double s) {
		return GVector2D(x / s, y / s);
	}
		
	GVector2D& operator+=(double s) {
		x += s;
		y += s;
		return *this;
	}
	GVector2D& operator-=(double s) {
		x -= s;
		y -= s;
		return *this;
	}
	GVector2D& operator*=(double s) {
		x *= s;
		y *= s;
		return *this;
	}
	GVector2D& operator/=(double s) {
		x /= s;
		y /= s;
		return *this;
	}

    void set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	
	void rotate(double theta) {
		double c = cos(theta);
		double s = sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
	}

    void rotateDegree(double deg) {
		double theta = deg / 180.0 * PI;
		double c = cos(theta);
		double s = sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
	}
	
	GVector2D& normalize() {
		if (length() == 0) return *this;
		*this *= (1.0 / length());
		return *this;
	}
	
	float dist(GVector2D v) const {
		GVector2D d(v.x - x, v.y - y);
		return d.length();
	}

	float length() const {
		return sqrt(x * x + y * y);
	}

	float heading() const {
		GVector2D cpy = *this;
		cpy.normalize();
		return acos(x);
	}

	void truncate(double length) {
		double angle = atan2f(y, x);
		x = length * cos(angle);
		y = length * sin(angle);
	}
	
	GVector2D ortho() const {
		return GVector2D(y, -x);
	}
	
	static float dot(GVector2D v1, GVector2D v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static float cross(GVector2D v1, GVector2D v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}
};
