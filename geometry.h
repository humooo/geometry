#include <cmath>
#include <iostream>
#include <vector>

class Point;
class Segment;

class Vector {
  public:
  Vector();
  Vector(int64_t xxx, int64_t yyy);
  Vector(const Point& lhs, const Point& rhs);
  Vector operator+(const Vector& rhs) const;
  int64_t operator*(const Vector& rhs) const;
  int64_t operator^(const Vector& rhs) const;
  double Length() const;
  Vector operator-() const;
  int64_t GetX() const;
  int64_t GetY() const;

  private:
  int64_t x_;
  int64_t y_;
};

Vector operator-(const Point& lhs, const Point& rhs);

class IShape {
  public:
  virtual void Move(const Vector& vector) = 0;
  virtual bool ContainsPoint(const Point& point) const = 0;
  virtual bool CrossSegment(const Segment& segment) const = 0;
  virtual IShape* Clone() const = 0;
  virtual ~IShape() = default;
};

class Point : public IShape {
  public:
  Point();
  Point(int64_t xxx, int64_t yyy);
  void Move(const Vector& vector);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& segment) const;
  IShape* Clone() const;
  int64_t GetX() const;
  int64_t GetY() const;
  int64_t x;
  int64_t y;
  ~Point() = default;
};

class Segment : public IShape {
  public:
  Segment(const Point& point_a, const Point& point_b);
  Point GetA() const;
  Point GetB() const;
  double FindDistWithPoint(const Point& point) const;
  void Move(const Vector& vector);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& segment) const;
  IShape* Clone() const;
  ~Segment() = default;

  private:
  Point a_;
  Point b_;
};

class Line : public IShape {
  public:
  Line(const Point& point_a, const Point& point_b);
  void Move(const Vector& vector);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& segment) const;
  IShape* Clone() const;
  double FindDistWithPoint(const Point& point) const;
  int64_t GetA() const;
  int64_t GetB() const;
  int64_t GetC() const;
  ~Line() = default;

  private:
  int64_t a_ = 0;
  int64_t b_ = 0;
  int64_t c_ = 0;
  Point point_a_;
  Point point_b_;
};

class Ray : public IShape {
  public:
  Ray(const Point& point_a, const Point& point_b);
  double FindDistWithPoint(Point& point) const;
  void Move(const Vector& vector);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& segment) const;
  IShape* Clone() const;
  Point GetA() const;
  Vector GetVector() const;
  ~Ray() = default;

  private:
  Point a_;
  Point b_;
};

class Circle : public IShape {
  public:
  explicit Circle(const Point& centre, int64_t radius);
  void Move(const Vector& vector);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& segment) const;
  IShape* Clone() const;
  Point GetCentre() const;
  int64_t GetRadius() const;
  ~Circle() = default;

  private:
  Point centre_;
  int64_t radius_ = 0;
};
