#include "Math.h"

template<> float Const<float>::pi = (float)PI;
template<> double Const<double>::pi = (double)PI;
template<> long double Const<long double>::pi = (long double)PI;

template<> float Const<float>::tol = 0.0001f;
template<> double Const<double>::tol = 0.0000001;
template<> long double Const<long double>::tol = 0.0000000001;
