/* 

   calin/math/function.hpp -- Stephen Fegan -- 2015-02-24

   Base classes for functions and general parameterizable objects that
   can be used with optimizers, root finders, the MCMC algorithm etc.

*/

#pragma once

#include <string>
#include <vector>
#include <limits>

#include <Eigen/Core>

namespace calin { namespace math { namespace function {

struct ParameterAxis
{
  ParameterAxis(const std::string& _name = { },
                const std::string& _units = { },
                bool _has_lo_bound = false,
                double _lo_bound = -std::numeric_limits<double>::infinity(),
                bool _has_hi_bound  = false,
                double _hi_bound = std::numeric_limits<double>::infinity(),
                double _scale = 1.0, double _initial_value = 0):
      name(_name), units(_units),
      has_lo_bound(_has_lo_bound), lo_bound(_lo_bound),
      has_hi_bound(_has_hi_bound), hi_bound(_hi_bound),
      scale(_scale), initial_value(_initial_value)
  { /* nothing to see here */ } 

  std::string name;
  std::string units;
  bool has_lo_bound;
  double lo_bound;
  bool has_hi_bound;
  double hi_bound;
  double scale;
  double initial_value;
};

using DomainAxis = ParameterAxis;

// Variadic template.. what fun!

template<typename T>
void assign_parameters(const double* values, T& x)
{
  x = *values;
}

template<typename T, typename... Params>
void assign_parameters(const double* values, T& x, Params & ... params)
{
  x = *values;
  assign_parameters(++values, params...);
}

class Parameterizable
{
 public:
  virtual ~Parameterizable();
  virtual std::vector<ParameterAxis> parameters() = 0;
  virtual std::vector<double> parameter_values() = 0;
  virtual void set_parameter_values(const double* values) = 0;
  virtual bool can_calculate_parameter_derivs() = 0;
  virtual bool can_calculate_parameter_hessian() = 0;
};

class MultiAxisFunction
{
 public:
  virtual ~MultiAxisFunction();
  virtual std::vector<DomainAxis> domain_axes() = 0;
  virtual double value(const double* x) = 0;
  virtual bool can_calculate_derivs() = 0;
  virtual double value_and_derivs(const double* x, double* derivs) = 0;
  virtual bool can_calculate_hessian() = 0;
  virtual double value_derivs_and_hessian(const double* x, double* derivs,
                                          double* hessian) = 0;
  virtual double error_up() = 0;
  
  // Utility functions
  double value_and_derivs(const std::vector<double>& x,
                          std::vector<double>& derivs)
  {
    unsigned npar = domain_axes().size();
    derivs.resize(npar);
    return value_and_derivs(&x.front(), &derivs.front());
  }

  double value_and_derivs(const std::vector<double>& x,
                          Eigen::VectorXd& derivs)
  {
    unsigned npar = domain_axes().size();
    derivs.resize(npar);
    return value_and_derivs(&x.front(), derivs.data());
  }

  double value_derivs_and_hessian(const std::vector<double>& x,
                                  std::vector<double>& derivs,
                                  std::vector<double>& hessian)
  {
    unsigned npar = domain_axes().size();
    derivs.resize(npar);
    hessian.resize(npar*npar);
    return value_derivs_and_hessian(&x.front(), &derivs.front(),
                                    &hessian.front());
  }
  
  double value_derivs_and_hessian(const std::vector<double>& x,
                                  std::vector<double>& derivs,
                                  Eigen::MatrixXd& hessian)
  {
    unsigned npar = domain_axes().size();
    derivs.resize(npar);
    hessian.resize(npar,npar);
    return value_derivs_and_hessian(&x.front(), &derivs.front(),
                                    hessian.data());
  }    

  double value_derivs_and_hessian(const std::vector<double>& x,
                                  Eigen::VectorXd& derivs,
                                  Eigen::MatrixXd& hessian)
  {
    unsigned npar = domain_axes().size();
    derivs.resize(npar);
    hessian.resize(npar,npar);
    return value_derivs_and_hessian(&x.front(), derivs.data(), hessian.data());
  }    
};

class SingleAxisFunction: virtual public MultiAxisFunction
{
 public:
  virtual ~SingleAxisFunction();
  virtual DomainAxis domain_axis() = 0;
  virtual double value(double x) = 0;
  virtual double value_and_deriv(double x,  double& dfdx) = 0;
  virtual double value_derivs_and_hessian(double x, double& dfdx,
                                          double& d2fdx2) = 0;

  // Members from MultiAxisFunction that we override
  std::vector<DomainAxis> domain_axes() override;
  double value(const double* x) override;
  double value_and_derivs(const double* x, double* derivs) override;
  double value_derivs_and_hessian(const double* x, double* derivs,
                                  double* hessian) override;
};

class Optimizable: virtual public MultiAxisFunction
{
 public:
  virtual ~Optimizable();
  virtual std::vector<double> initialPoint() const = 0;
  virtual std::vector<double> stepSize() const = 0;
};

template<typename ParamType>
class MultiParameterSet: virtual public Parameterizable
{
 public:
  virtual ~MultiParameterSet() { /* nothing to see here */ }
  virtual std::vector<ParameterAxis> parameters() override;
  virtual std::vector<double> parameter_values() override;
  virtual void set_parameter_values(const double* values) override;
 private:
  unsigned find_parameter_set(unsigned iparam);
  std::vector<std::pair<unsigned, ParamType*> > parameter_sets_;
};

} // namespace function

using function::ParameterAxis;
using function::DomainAxis;
using function::Parameterizable;
using function::MultiAxisFunction;
using function::SingleAxisFunction;
using function::Optimizable;

} } // namespace calin::math
