/*

   calin/math/function.hpp -- Stephen Fegan -- 2015-02-24

   Base classes for functions and general parameterizable objects that
   can be used with optimizers, root finders, the MCMC algorithm etc.

   Copyright 2015, Stephen Fegan <sfegan@llr.in2p3.fr>
   LLR, Ecole polytechnique, CNRS/IN2P3, Universite Paris-Saclay

   This file is part of "calin"

   "calin" is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License version 2 or
   later, as published by the Free Software Foundation.

   "calin" is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

*/

#pragma once

#include <string>
#include <vector>
#include <limits>
#include <numeric>

#include <Eigen/Core>

#include "calin_global_definitions.hpp"

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

CALIN_TYPEALIAS(DomainAxis, ParameterAxis);

// Variadic template.. what fun!

template<typename T>
void assign_parameters(const double* values, T& x)
{
  x = *values;
}

template<typename T>
void assign_parameters(ConstVecRef values, T& x)
{
  x = values[0];
}

template<typename T, typename... Params>
void assign_parameters(const double* values, T& x, Params & ... params)
{
  x = *values;
  assign_parameters(++values, params...);
}

template<typename T, typename... Params>
void assign_parameters(ConstVecRef values, T& x, Params & ... params)
{
  const double* values_data = values.data();
  x = *values_data;
  assign_parameters(++values_data, params...);
}

class Parameterizable
{
 public:
  virtual ~Parameterizable();
  virtual unsigned num_parameters() = 0;
  virtual std::vector<ParameterAxis> parameters() = 0;
  virtual Eigen::VectorXd parameter_values() = 0;
  virtual void set_parameter_values(ConstVecRef values) = 0;
  virtual bool can_calculate_parameter_gradient() = 0;
  virtual bool can_calculate_parameter_hessian() = 0;
};

class MultiAxisFunction
{
 public:
  virtual ~MultiAxisFunction();
  virtual unsigned num_domain_axes() = 0;
  virtual std::vector<DomainAxis> domain_axes() = 0;
  virtual double value(ConstVecRef x) = 0;
  virtual bool can_calculate_gradient() = 0;
  virtual double value_and_gradient(ConstVecRef x, VecRef gradient) = 0;
  virtual bool can_calculate_hessian() = 0;
  virtual double value_gradient_and_hessian(ConstVecRef x, VecRef gradient,
                                            MatRef hessian) = 0;
  virtual double error_up() = 0;
};

class SingleAxisFunction: virtual public MultiAxisFunction
{
 public:
  virtual ~SingleAxisFunction();
  virtual DomainAxis domain_axis() = 0;
  virtual double value_1d(double x) = 0;
  virtual double value_and_gradient_1d(double x,  double& dfdx) = 0;
  virtual double value_gradient_and_hessian_1d(double x, double& dfdx,
                                               double& d2fdx2) = 0;

  // Members from MultiAxisFunction that we override
  unsigned num_domain_axes() override;
  std::vector<DomainAxis> domain_axes() override;
  double value(ConstVecRef x) override;
  double value_and_gradient(ConstVecRef x, VecRef gradient) override;
  double value_gradient_and_hessian(ConstVecRef x, VecRef gradient,
                                    MatRef hessian) override;
};

class ParameterizableMultiAxisFunction: public Parameterizable,
                                        virtual public MultiAxisFunction
{
 public:
  virtual ~ParameterizableMultiAxisFunction();
  virtual double value_and_parameter_gradient(ConstVecRef x,
                                         VecRef gradient) = 0;
  virtual double value_parameter_gradient_and_hessian(ConstVecRef x,
                                         VecRef gradient, MatRef hessian) = 0;
};

class ParameterizableSingleAxisFunction:
      public ParameterizableMultiAxisFunction,
      public SingleAxisFunction
{
 public:
  virtual ~ParameterizableSingleAxisFunction();
  virtual double value_and_parameter_gradient_1d(double x,
                                                 VecRef gradient) = 0;
  virtual double value_parameter_gradient_and_hessian_1d(double x,
                                         VecRef gradient, MatRef hessian) = 0;

  // Members from ParameterizableMultiAxisFunction that we override
  double value_and_parameter_gradient(ConstVecRef x,
                                      VecRef gradient) override;
  double value_parameter_gradient_and_hessian(ConstVecRef x,
                                      VecRef gradient, MatRef hessian) override;
};

template<typename ParameterizableBaseType>
class ReducedSpaceParameterizable: public ParameterizableBaseType
{
public:
  template<typename ... Args> ReducedSpaceParameterizable(Args && ... args):
    ParameterizableBaseType(std::forward<Args>(args)...),
    subspace_params_(ParameterizableBaseType::num_parameters()),
    removed_params_(),
    removed_param_values_(ParameterizableBaseType::num_parameters())
  {
    std::iota(subspace_params_.begin(), subspace_params_.end(), 0);
  }
  //ReducedSpaceParameterizable(ParameterizableBaseType* p, bool adopt_p);
  virtual ~ReducedSpaceParameterizable();
  unsigned num_parameters() override;
  virtual std::vector<ParameterAxis> parameters() override;
  virtual Eigen::VectorXd parameter_values() override;
  virtual void set_parameter_values(ConstVecRef values) override;
  //virtual bool can_calculate_parameter_gradient() override;
  //virtual bool can_calculate_parameter_hessian() override;

  bool remove_parameter_from_subspace(unsigned iparam, double value);
  bool replace_parameter(unsigned iparam);
  const std::vector<unsigned>& subspace_parameters() const { return subspace_params_; }
  const std::vector<unsigned>& removed_parameters() const { return removed_params_; }
  unsigned index_of_subspace_parameter(unsigned iparam) const {
    return subspace_params_.at(iparam); }
  unsigned index_of_removed_parameter(unsigned iparam) const {
    return removed_params_.at(iparam); }

  Eigen::VectorXd subspace_param_vec_to_original(ConstVecRef values);
  Eigen::VectorXd original_param_vec_to_subspace(ConstVecRef values);
  Eigen::VectorXd original_param_grad_to_subspace(ConstVecRef grad);
  Eigen::MatrixXd original_param_hess_to_subspace(ConstMatRef hess);

protected:
  std::vector<unsigned> subspace_params_;
  std::vector<unsigned> removed_params_;
  Eigen::VectorXd removed_param_values_;
};

template<typename ParameterizableBaseType>
class ReducedSpaceParameterizableMultiAxisFunction:
  public ReducedSpaceParameterizable<ParameterizableBaseType>
{
public:
  using ReducedSpaceParameterizable<ParameterizableBaseType>::ReducedSpaceParameterizable;

  virtual ~ReducedSpaceParameterizableMultiAxisFunction();
  double value_and_parameter_gradient(ConstVecRef x, VecRef gradient) override;
  double value_parameter_gradient_and_hessian(ConstVecRef x,
    VecRef gradient, MatRef hessian) override;
};

template<typename ParameterizableBaseType>
class ReducedSpaceParameterizableSingleAxisFunction:
  public ReducedSpaceParameterizable<ParameterizableBaseType>
{
public:
  using ReducedSpaceParameterizable<ParameterizableBaseType>::ReducedSpaceParameterizable;

  virtual ~ReducedSpaceParameterizableSingleAxisFunction();

  double value_and_parameter_gradient_1d(double x, VecRef gradient) override;
  double value_parameter_gradient_and_hessian_1d(double x,
    VecRef gradient, MatRef hessian) override;
};

class FreezeThawFunction: public ParameterizableMultiAxisFunction
{
 public:
  FreezeThawFunction(MultiAxisFunction* fcn, bool adopt_fcn=false);
  virtual ~FreezeThawFunction();

  MultiAxisFunction* function() { return fcn_; }
  bool freeze(unsigned index, double value);
  bool thaw(unsigned index);
  const std::vector<unsigned>& free_axes() const { return free_axes_; }
  const std::vector<unsigned>& frozen_axes() const { return frozen_axes_; }
  unsigned index_of_free_axis(unsigned iparam) const {
    return free_axes_.at(iparam); }
  unsigned index_of_frozen_axis(unsigned iparam) const {
    return frozen_axes_.at(iparam); }

  Eigen::VectorXd x_in2out(ConstVecRef x);

  // Function interface
  unsigned num_domain_axes() override;
  std::vector<DomainAxis> domain_axes() override;
  double value(ConstVecRef x) override;
  bool can_calculate_gradient() override;
  double value_and_gradient(ConstVecRef x, VecRef gradient) override;
  bool can_calculate_hessian() override;
  double value_gradient_and_hessian(ConstVecRef x, VecRef gradient,
                                    MatRef hessian) override;
  double error_up() override;

  // Parameterizable interface
  unsigned num_parameters() override;
  std::vector<ParameterAxis> parameters() override;
  Eigen::VectorXd parameter_values() override;
  void set_parameter_values(ConstVecRef values) override;
  bool can_calculate_parameter_gradient() override;
  bool can_calculate_parameter_hessian() override;

  // ParameterizableMultiAxisFunction interface
  double value_and_parameter_gradient(ConstVecRef x,
                                      VecRef gradient) override;
  double value_parameter_gradient_and_hessian(ConstVecRef x,
                                     VecRef gradient, MatRef hessian) override;

 protected:
  Eigen::VectorXd gradient_out2in(ConstVecRef gradient);
  Eigen::MatrixXd hessian_out2in(ConstMatRef hessian);

  Eigen::VectorXd par_gradient_out2in(ConstVecRef gradient);
  Eigen::MatrixXd par_hessian_out2in(ConstMatRef hessian);

  MultiAxisFunction* fcn_;
  bool adopt_fcn_ = false;
  std::vector<unsigned> free_axes_;
  std::vector<unsigned> frozen_axes_;
  Eigen::VectorXd xfrozen_;
};

class PMAFReverser: public ParameterizableMultiAxisFunction
{
 public:
  PMAFReverser(ParameterizableMultiAxisFunction* fcn_deligate,
               bool adopt_fcn_deligate = false, double error_up = 0.5);
  virtual ~PMAFReverser();

  // Parameterizable interface
  unsigned num_parameters() override;
  std::vector<ParameterAxis> parameters() override;
  Eigen::VectorXd parameter_values() override;
  void set_parameter_values(ConstVecRef values) override;
  bool can_calculate_parameter_gradient() override;
  bool can_calculate_parameter_hessian() override;

  // MultiAxisFunction interface
  unsigned num_domain_axes() override;
  std::vector<DomainAxis> domain_axes() override;
  double value(ConstVecRef x) override;
  bool can_calculate_gradient() override;
  double value_and_gradient(ConstVecRef x, VecRef gradient) override;
  bool can_calculate_hessian() override;
  double value_gradient_and_hessian(ConstVecRef x, VecRef gradient,
                                    MatRef hessian) override;
  double error_up() override;

  // ParameterizableMultiAxisFunction interface
  double value_and_parameter_gradient(ConstVecRef x,
                                      VecRef gradient) override;
  double value_parameter_gradient_and_hessian(ConstVecRef x,
                                      VecRef gradient, MatRef hessian) override;

 protected:
  ParameterizableMultiAxisFunction* fcn_deligate_;
  bool adopt_fcn_deligate_ = false;
  double error_up_ = 0.5;
  Eigen::VectorXd x_;
};

template<typename ParamType>
class MultiParameterSet: virtual public Parameterizable
{
 public:
  virtual ~MultiParameterSet() { /* nothing to see here */ }
  virtual std::vector<ParameterAxis> parameters() override;
  virtual Eigen::VectorXd parameter_values() override;
  virtual void set_parameter_values(ConstVecRef values) override;
 private:
  unsigned find_parameter_set(unsigned iparam);
  std::vector<std::pair<unsigned, ParamType*> > parameter_sets_;
};

bool gradient_check_eps(MultiAxisFunction& fcn, ConstVecRef x, VecRef good,
                        double max_good = 0.5, double eps_factor = 10.0);
bool gradient_check(MultiAxisFunction& fcn, ConstVecRef x, ConstVecRef dx,
                    VecRef good, double max_good = 0.5);
bool hessian_check(MultiAxisFunction& fcn, ConstVecRef x, ConstVecRef dx,
                   MatRef good, double max_good = 0.5);

#ifndef SWIG

template<typename ParameterizableType> using ValGetter =
    std::function<double(ParameterizableType*)>;
template<typename ParameterizableType> using GradGetter =
    std::function<double(ParameterizableType*, VecRef)>;
template<typename ParameterizableType> using HessGetter =
    std::function<double(ParameterizableType*, VecRef, MatRef)>;

template<typename ParameterizableType>
class ParameterizableToMultiAxisFunctionAdapter: public MultiAxisFunction
{
 public:
  using PTValGetter = ValGetter<ParameterizableType>;
  using PTGradGetter = GradGetter<ParameterizableType>;
  using PTHessGetter = HessGetter<ParameterizableType>;

  ParameterizableToMultiAxisFunctionAdapter(ParameterizableType* par,
                                            PTValGetter val_getter,
                                            PTGradGetter grad_getter,
                                            PTHessGetter hess_getter,
                                            double error_up = 0.5):
      MultiAxisFunction(), par_(par), val_getter_(val_getter),
      grad_getter_(grad_getter), hess_getter_(hess_getter),
      error_up_(error_up) { }
  ~ParameterizableToMultiAxisFunctionAdapter() { }
  unsigned num_domain_axes() override { return par_->num_parameters(); }
  std::vector<DomainAxis> domain_axes() override { return par_->parameters(); }
  double value(ConstVecRef x) override {
    par_->set_parameter_values(x);
    return val_getter_(par_); }
  bool can_calculate_gradient() override {
    return par_->can_calculate_parameter_gradient(); }
  double value_and_gradient(ConstVecRef x, VecRef gradient) override {
    par_->set_parameter_values(x);
    return grad_getter_(par_, gradient); }
  bool can_calculate_hessian() override {
    return par_->can_calculate_parameter_hessian(); }
  double value_gradient_and_hessian(ConstVecRef x, VecRef gradient,
                                    MatRef hessian) override {
    par_->set_parameter_values(x);
    return hess_getter_(par_, gradient, hessian); }
  double error_up() override { return error_up_; }
 protected:
  ParameterizableType* par_;
  PTValGetter val_getter_;
  PTGradGetter grad_getter_;
  PTHessGetter hess_getter_;
  double error_up_;
};

template<typename ParameterizableType>
bool gradient_check_par(ParameterizableType& par_fcn,
                        ConstVecRef p, ConstVecRef dp, VecRef good,
                        ValGetter<ParameterizableType> val_getter,
                        GradGetter<ParameterizableType> grad_getter,
                        HessGetter<ParameterizableType> hess_getter,
                        double max_good = 0.5, double error_up = 0.5)
{
  function::ParameterizableToMultiAxisFunctionAdapter<ParameterizableType>
      fcn(&par_fcn, val_getter, grad_getter, hess_getter, error_up);
  return function::gradient_check(fcn, p, dp, good, max_good);
}

template<typename ParameterizableType>
bool hessian_check_par(ParameterizableType& par_fcn,
                       ConstVecRef p, ConstVecRef dp, MatRef good,
                       ValGetter<ParameterizableType> val_getter,
                       GradGetter<ParameterizableType> grad_getter,
                       HessGetter<ParameterizableType> hess_getter,
                       double max_good = 0.5, double error_up = 0.5)
{
  function::ParameterizableToMultiAxisFunctionAdapter<ParameterizableType>
      fcn(&par_fcn, val_getter, grad_getter, hess_getter, error_up);
  return function::hessian_check(fcn, p, dp, good, max_good);
}

#endif // ifndef SWIG

} } } // namespace calin::math::function

#include "function_impl.hpp"
