class Vector {
public:
  Vector(int size);
  double &operator[](int index);
  int size();
  ~Vector();

private:
  double *_elements;
  int _size;
};
