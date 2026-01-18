/*
  Class Hierarchies = Family of related classes
  Lattice Structure with deep derivations
*/
#include <iostream>
#include <memory>
#include <vector>

namespace ClassHierarchies {

enum class Kind { circle, triangle, smiley };

class Point {
public:
  double x;
  double y;
};

std::istream &operator>>(std::istream &is, Point &p) {
  return is >> p.x >> p.y;
}

class Shape {
public:
  virtual Point center() const = 0;
  virtual void move(Point to) = 0;
  virtual void draw() const = 0;
  virtual void rotate(int angle) = 0;
  virtual ~Shape() {}
};

class Circle : public Shape {
public:
  Circle(Point center, int radius)
      : _center{center}, _radius{radius} {} // constructor
  Point center() const override { return _center; }
  void move(Point to) override { _center = to; }
  void draw() const override { std::cout << "Circle" << std::endl; }
  void rotate(int angle) override {}

private:
  Point _center;
  int _radius;
};

class Triangle : public Shape {
public:
  Triangle(Point p1, Point p2, Point p3) : _p1{p1}, _p2{p2}, _p3{p3} {}
  Point center() const override {
    return Point{(_p1.x + _p2.x + _p3.x) / 3.0, (_p1.y + _p2.y + _p3.y) / 3.0};
  }
  void move(Point to) override {
    Point c = center();
    Point delta{to.x - c.x, to.y - c.y};
    _p1.x += delta.x;
    _p1.y += delta.y;
    _p2.x += delta.x;
    _p2.y += delta.y;
    _p3.x += delta.x;
    _p3.y += delta.y;
  }
  void draw() const override { std::cout << "Triangle" << std::endl; }
  void rotate(int angle) override {}

private:
  Point _p1, _p2, _p3;
};

class Smiley : public Circle {
public:
  Smiley(Point center, int radius) : Circle{center, radius}, _mouth{nullptr} {}
  ~Smiley() {
    delete _mouth;
    for (auto eye : _eyes)
      delete eye;
  }

  void move(Point to) override;
  void draw() const override;
  void rotate(int angle) override;
  void add_eye(Shape *s) { _eyes.push_back(s); }
  void set_mouth(Shape *s) { _mouth = s; }
  void wink(int eye_number);

private:
  Shape *_mouth;
  std::vector<Shape *> _eyes;
};

void Smiley::draw() const {
  Circle::draw();
  for (auto eye : _eyes)
    eye->draw();
  _mouth->draw();
}
void Smiley::move(Point to) {
  Circle::move(to);
  // logic to move eyes and mouth would go here
}
void Smiley::rotate(int angle) {
  // logic to rotate would go here
}
void Smiley::wink(int eye_number) {
  // wink logic
  std::cout << "Winking eye " << eye_number << "\n";
}

void rotate_all(std::vector<Shape *> &shapes, int angle) {
  for (auto shape : shapes)
    shape->rotate(angle);
}

void draw_all(const std::vector<Shape *> &shapes) {
  for (auto shape : shapes)
    shape->draw();
}

Shape *read_shape(std::istream &is) {
  int k;
  is >> k;
  if (!is)
    return nullptr;
  Kind kind = static_cast<Kind>(k);
  switch (kind) {
  case Kind::circle: {
    Point center;
    int radius;
    is >> center >> radius;
    return new Circle{center, radius};
  }
  case Kind::triangle: {
    Point p1, p2, p3;
    is >> p1 >> p2 >> p3;
    return new Triangle{p1, p2, p3};
  }
  case Kind::smiley: {
    Point center;
    int radius;
    is >> center >> radius;
    Smiley *ps = new Smiley{center, radius};
    Shape *e1 = read_shape(is);
    Shape *e2 = read_shape(is);
    Shape *m = read_shape(is);
    ps->add_eye(e1);
    ps->add_eye(e2);
    ps->set_mouth(m);
    return ps;
  }
  default:
    return nullptr;
  }
}

void user() {
  std::vector<Shape *> v;
  while (std::cin) {
    Shape *p = read_shape(std::cin);
    if (p)
      v.push_back(p);
    else
      break; // Stop on error or EOF
  }
  draw_all(v);
  rotate_all(v, 45);
  for (auto p : v)
    delete p;
}

} // namespace ClassHierarchies

namespace UniquePtr {
using namespace ClassHierarchies;

std::unique_ptr<Shape> read_shape(std::istream &is) {
  int k;
  is >> k;
  if (!is)
    return nullptr;
  Kind kind = static_cast<Kind>(k);
  switch (kind) {
  case Kind::circle: {
    Point center;
    int radius;
    is >> center >> radius;
    return std::unique_ptr<Shape>{new Circle{center, radius}};
  }
  case Kind::triangle: {
    Point p1, p2, p3;
    is >> p1 >> p2 >> p3;
    return std::unique_ptr<Shape>{new Triangle{p1, p2, p3}};
  }
  case Kind::smiley: {
    Point center;
    int radius;
    is >> center >> radius;
    Smiley *ps = new Smiley{center, radius};
    auto e1 = read_shape(is);
    auto e2 = read_shape(is);
    auto m = read_shape(is);
    if (e1)
      ps->add_eye(e1.release());
    if (e2)
      ps->add_eye(e2.release());
    if (m)
      ps->set_mouth(m.release());
    return std::unique_ptr<Shape>{ps};
  }
  default:
    return nullptr;
  }
}

void user() {
  std::vector<std::unique_ptr<Shape>> v;
  while (std::cin) {
    auto p = read_shape(std::cin);
    if (p)
      v.push_back(std::move(p));
    else
      break;
  }
  for (const auto &p : v)
    p->draw();
  for (const auto &p : v)
    p->rotate(45);
}
} // namespace UniquePtr

int main() {
  std::cout << "--- Manual Test ---\n";
  using namespace ClassHierarchies;
  Smiley s{Point{0, 0}, 1};
  s.add_eye(new Circle{Point{.5, .5}, 1});
  s.add_eye(new Circle{Point{-.5, .5}, 1});
  s.set_mouth(new Circle{Point{0, 0}, 1});

  s.draw();
  s.wink(1);

  std::cout << "\n--- Input Test ---\n";
  // Verify UniquePtr user() with standard input
  UniquePtr::user();

  return 0;
}

