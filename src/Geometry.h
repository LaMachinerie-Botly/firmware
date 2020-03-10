#include <Arduino.h>

static class GMath{
    const double PI = 3.14159265358;
	const double degToRad = PI/180;
	const double radToDeg = 180/PI; 
};

template <class T>

class GVector2D{
public:

    T x, y;

    GVector2D(): x(0), y(0){}
    GVector2D(T _x, T _y) : x(_x), y(_y){}
    GVector2D(const GVector2D& v) : x(v.x), y(v.y) {}

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

    void set(T x, T y) {
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
		double theta = deg / 180.0 * GMath.PI;
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
		GVector2D cpy = this;
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

typedef GVector2D<float> GVector2d;
typedef GVector2D<double> GVector2f;


class GVector2z : public GVector2D{
public :
	GVector2z() : GVector2D(), theta(0){}
    GVector2z(T _x, T _y) : GVector2D(_x, _y), theta(0){}
	GVector2z(T _x, T _y, T theta) : GVector2D(_x, _y), theta(theta){}
    GVector2z(const GVector2D& v) : GVector2D(v), theta(0) {}
	GVector2z(const GVector2z& v) : GVector2D(v.x, v.y), theta(v.theta) {}

private :
	T theta;
}