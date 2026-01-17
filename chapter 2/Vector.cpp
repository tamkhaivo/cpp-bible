#include "Vector.h"

Vector::Vector(int size) : _size{size}, _elements{new double[size]} {}
double &Vector::operator[](int i) { return _elements[i]; }
int Vector::size() { return _size; }
Vector::~Vector() { delete[] _elements; }
