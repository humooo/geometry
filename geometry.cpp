#include "geometry.hpp"

Vector::Vector() : x_(0), y_(0) {}

Vector::Vector(int64_t xxx, int64_t yyy) : x_(xxx), y_(yyy) {}

Vector::Vector(const Point& lhs, const Point& rhs) {
  x_ = rhs.GetX() - lhs.GetX();
  y_ = rhs.GetY() - lhs.GetY();
}

int64_t Vector::operator*(const Vector& rhs) const {
  int64_t res = x_ * rhs.x_ + y_ * rhs.y_;
  return res;
}

int64_t Vector::operator^(const Vector& rhs) const {
  int64_t res = x_ * rhs.y_ - y_ * rhs.x_;
  return res;
}

Vector Vector::operator-() const {
  Point lhs = Point(0, 0);
  Point new_rhs = Point(-x_, -y_);
  Vector res = Vector(lhs, new_rhs);
  return res;
}

Vector Vector::operator+(const Vector& rhs) const {
  Point lhs = Point(0, 0);
  Point rhss = Point(x_ + rhs.x_, y_ + rhs.y_);
  Vector res = Vector(lhs, rhss);
  return res;
}

double Vector::Length() const { return sqrt(x_ * x_ + y_ * y_); }

int64_t Vector::GetX() const { return x_; }

int64_t Vector::GetY() const { return y_; }

Vector operator-(const Point& lhs, const Point& rhs) {
  return Vector(lhs.x - rhs.x, lhs.y - rhs.y);
}

Point::Point() : x(0), y(0) {}

Point::Point(int64_t xxx, int64_t yyy) : x(xxx), y(yyy) {}

Point Circle::GetCentre() const { return centre_; }

int64_t Circle::GetRadius() const { return radius_; }

void Point::Move(const Vector& vector) {
  x += vector.GetX();
  y += vector.GetY();
}

bool Point::ContainsPoint(const Point& point) const {
  return x == point.x and y == point.y;
}

IShape* Point::Clone() const {
  Point* res = new Point(x, y);
  return res;
}

bool Point::CrossSegment(const Segment& segment) const {
  Point temp(x, y);
  return segment.ContainsPoint(temp);
}

int64_t Point::GetX() const { return x; }

int64_t Point::GetY() const { return y; }

Segment::Segment(const Point& point_a, const Point& point_b)
    : a_(point_a), b_(point_b) {}

double Segment::FindDistWithPoint(const Point& point) const {
  Vector acc(a_, point);
  Vector abb(a_, b_);
  Vector bcc(b_, point);
  Line temp(a_, b_);
  if (acc * abb >= 0 and bcc * (-abb) >= 0) {
    return temp.FindDistWithPoint(point);
  }
  return std::min(acc.Length(), bcc.Length());
}

bool Segment::CrossSegment(const Segment& segment) const {
  bool crosses = false;
  Vector abb(a_, b_);
  Vector acc(a_, segment.a_);
  Vector add(a_, segment.b_);
  Vector cdd(segment.a_, segment.b_);
  Vector cbb(segment.a_, b_);
  Vector dbb(segment.b_, b_);
  int64_t ab_ac = abb ^ acc;
  int64_t ab_ad = abb ^ add;
  int64_t cd_ca = -acc ^ cdd;
  int64_t cd_cb = cbb ^ cdd;
  return (ab_ac * ab_ad < 0 and cd_ca * cd_cb < 0) or
      (ab_ac == 0 and -acc * cbb <= 0) or (ab_ad == 0 and -add * dbb <= 0) or
      (cd_ca == 0 and acc * add <= 0) or (cd_cb == 0 and -cbb * -dbb <= 0);
}

bool Segment::ContainsPoint(const Point& point) const {
  Vector acc = Vector(a_, point);
  Vector abb = Vector(a_, b_);
  if ((abb ^ acc) == 0) {
    return point.GetX() <= std::max(a_.GetX(), b_.GetX()) and
        point.GetX() >= std::min(a_.GetX(), b_.GetX()) and
        point.GetY() <= std::max(a_.GetY(), b_.GetY()) and
        point.GetY() >= std::min(a_.GetY(), b_.GetY());
  }
  return false;
}

void Segment::Move(const Vector& vector) {
  a_.Move(vector);
  b_.Move(vector);
}

IShape* Segment::Clone() const {
  Segment* res = new Segment(a_, b_);
  return res;
}

Point Segment::GetA() const { return a_; }

Point Segment::GetB() const { return b_; }

Line::Line(const Point& point_a, const Point& point_b) {
  a_ = point_b.GetY() - point_a.GetY();
  b_ = point_a.GetX() - point_b.GetX();
  c_ = point_a.GetY() * (point_b.GetX() - point_a.GetX()) -
      point_a.GetX() * (point_b.GetY() - point_a.GetY());
  point_a_ = point_a;
  point_b_ = point_b;
}

bool Line::ContainsPoint(const Point& point) const {
  int64_t res = a_ * point.GetX() + b_ * point.GetY() + c_;
  return res == 0;
}

double Line::FindDistWithPoint(const Point& point) const {
  return static_cast<double>(
      std::abs(a_ * point.GetX() + b_ * point.GetY() + c_)) /
      sqrt(static_cast<double>(a_ * a_ + b_ * b_));
}

void Line::Move(const Vector& vector) {
  c_ = c_ - a_ * vector.GetX() - b_ * vector.GetY();
  point_a_.x += vector.GetX();
  point_a_.y += vector.GetY();
  point_b_.x += vector.GetX();
  point_b_.y += vector.GetY();
}

IShape* Line::Clone() const {
  Line* res = new Line(point_a_, point_b_);
  return res;
}
bool Line::CrossSegment(const Segment& segment) const {
  Vector abb = Vector(point_a_, point_b_);
  Vector acc = Vector(point_a_, segment.GetA());
  Vector add = Vector(point_a_, segment.GetB());
  return (abb ^ acc) * (abb ^ add) <= 0;
}

int64_t Line::GetA() const { return a_; }

int64_t Line::GetB() const { return b_; }

int64_t Line::GetC() const { return c_; }

Ray::Ray(const Point& point_a, const Point& point_b)
    : a_(point_a), b_(point_b) {}

bool Ray::ContainsPoint(const Point& point) const {
  Vector lhs = Vector(a_, b_);
  Vector rhs = Vector(a_, point);
  int64_t scal_mul = lhs * rhs;
  return (lhs ^ rhs) == 0 and scal_mul >= 0;
}

double Ray::FindDistWithPoint(Point& point) const {
  Vector abb = Vector(a_, b_);
  Vector acc = Vector(a_, point);
  Line temp = Line(a_, b_);
  if (abb * acc >= 0) {
    return temp.FindDistWithPoint(point);
  }
  return acc.Length();
}

void Ray::Move(const Vector& vector) {
  a_.Move(vector);
  b_.Move(vector);
}

bool Ray::CrossSegment(const Segment& segment) const {
  Vector ray_vec = Vector(a_, b_);
  Vector segment_vec = Vector(segment.GetA(), segment.GetB());
  Vector from_segment_to_ray = Vector(segment.GetA(), a_);
  if ((ray_vec ^ segment_vec) == 0) {
    return ContainsPoint(segment.GetA()) or ContainsPoint(segment.GetB());
  }
  int64_t rss = ray_vec ^ segment_vec;
  int64_t rsr = ray_vec ^ from_segment_to_ray;
  int64_t srs = segment_vec ^ from_segment_to_ray;
  if (rss < 0) {
    rss *= -1;
    rsr *= -1;
    srs *= -1;
  }
  return rsr >= 0 and rsr <= rss and srs >= 0;
}

IShape* Ray::Clone() const {
  Ray* res = new Ray(a_, b_);
  return res;
}

Point Ray::GetA() const { return a_; }

Vector Ray::GetVector() const { return Vector(a_, b_); }

Circle::Circle(const Point& centre, int64_t radius)
    : centre_(centre), radius_(radius) {}

void Circle::Move(const Vector& vector) { centre_.Move(vector); }

bool Circle::ContainsPoint(const Point& point) const {
  int64_t dist =
      (centre_.GetX() - point.GetX()) * (centre_.GetX() - point.GetX()) +
          (centre_.GetY() - point.GetY()) * (centre_.GetY() - point.GetY());
  return dist <= radius_ * radius_;
}

bool Circle::CrossSegment(const Segment& segment) const {
  double dist = segment.FindDistWithPoint(centre_);
  int64_t dist1 = (centre_.GetX() - segment.GetA().GetX()) *
      (centre_.GetX() - segment.GetA().GetX()) +
      (centre_.GetY() - segment.GetA().GetY()) *
          (centre_.GetY() - segment.GetA().GetY());
  int64_t dist2 = (centre_.GetX() - segment.GetB().GetX()) *
      (centre_.GetX() - segment.GetB().GetX()) +
      (centre_.GetY() - segment.GetB().GetY()) *
          (centre_.GetY() - segment.GetB().GetY());

  return dist <= static_cast<double>(radius_) and
      (dist1 >= radius_ * radius_ or dist2 >= radius_ * radius_);
}

IShape* Circle::Clone() const {
  Circle* res = new Circle(centre_, radius_);
  return res;
}
