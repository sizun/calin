/* 

   calin/math/function.cpp -- Stephen Fegan -- 2015-02-24

*/

#include "math/function.hpp"

// namespace calin { namespace math { namespace function {

using namespace calin::math::function;

Parameterizable::~Parameterizable()
{
  // nothing to see here
}

MultiAxisFunction::~MultiAxisFunction()
{
  // nothing to see here
}

SingleAxisFunction::~SingleAxisFunction()
{
  // nothing to see here
}

std::vector<DomainAxis> SingleAxisFunction::domain_axes()
{
  return { domain_axis() };
}

double SingleAxisFunction::value(const double* x)
{
  return value(x[0]);
}

double SingleAxisFunction::value_and_gradient(const double* x,
                                            double* gradient) 
{
  return value_and_deriv(*x,*gradient);
}

double SingleAxisFunction::
value_gradient_and_hessian(const double* x, double* gradient, double* hessian) 
{
  return value_gradient_and_hessian(*x,*gradient,*hessian);
}

#if 0
DoubleAxisFunction::~DoubleAxisFunction()
{
  // nothing to see here
}
#endif

// } } } // namespace calin::math::function
