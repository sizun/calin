/* 

   calin/math/log_quadratic_spline_pdf_1d.hpp -- Stephen Fegan -- 2015-07-30

   PDF base on quadratic spline in log space.

*/

#pragma once

#include <string>

#include "pdf_1d.hpp"

namespace calin { namespace math { namespace pdf_1d {

enum class ParamZeroType { SLOPE, CURVATURE };
enum class ParamZeroLocation { LEFT, RIGHT };

class LogQuadraticSpline1DPDF: public Parameterizable1DPDF
{
public:
  constexpr static double inf = std::numeric_limits<double>::infinity();
  
  LogQuadraticSpline1DPDF(ConstVecRef xknots,
                          double xlo, double xhi, double bin_dx = 0.0,
                          ParamZeroType p0_type = ParamZeroType::SLOPE,
                          ParamZeroLocation p0_loc = ParamZeroLocation::LEFT,
                          bool normalize = true,
                          const std::string& yunits = "y-value units",
                          const std::string& xunits = "x-value units",
                          double error_up = 0.5);
      
  virtual ~LogQuadraticSpline1DPDF();
  
  // Reiterate functions from ParameterizableSingleAxisFunction

  unsigned num_parameters() override;
  std::vector<function::ParameterAxis> parameters() override;
  Eigen::VectorXd parameter_values() override;
  void set_parameter_values(ConstVecRef values) override;

  function::DomainAxis domain_axis() override;
  
  bool can_calculate_gradient() override;
  bool can_calculate_hessian() override;
  bool can_calculate_parameter_gradient() override;
  bool can_calculate_parameter_hessian() override;

  double value_1d(double x) override;
  double value_and_gradient_1d(double x,  double& dfdx) override;
  double value_gradient_and_hessian_1d(double x, double& dfdx,
                            double& d2fdx2) override;
  double value_and_parameter_gradient_1d(double x,
                                         VecRef gradient) override;
  double value_parameter_gradient_and_hessian_1d(double x, VecRef gradient,
                                                 MatRef hessian) override;

  double error_up() override;

  ConstVecRef a() const { return a_; }
  ConstVecRef b() const { return b_; }
  ConstVecRef xknot() const { return xknot_; }
  ConstVecRef yknot() const { return yknot_; }
  
  // Moments

  bool can_calculate_mean_and_variance() override;
  void mean_and_variance(double& mean, double& var) override;

 protected:
  unsigned find_segment(double x) const;
  void set_cache();
  void set_spline_coeffs_left_to_right();
  void set_spline_coeffs_right_to_left();
  static void integral(double a, double b, double c, double xl, double xr,
                       double& I, double& dI_da, double& dI_db);
  
  std::string yunits_       = "y-value units";
  std::string xunits_       = "x-value units";
  double error_up_          = 0.5;
  double xlo_               = 0.0;
  double xhi_               = inf;
  double bin_dx_            = 0.0;
  ParamZeroType p0_type_    = ParamZeroType::SLOPE;
  ParamZeroLocation p0_loc_ = ParamZeroLocation::LEFT;

  unsigned nknot_           = 0;
  double param0_            = 0;
  Eigen::VectorXd xknot_; // vector of size nknot
  Eigen::VectorXd yknot_; // vector of size nknot

  Eigen::VectorXd dx_;    // vector of size nknot-1
  Eigen::VectorXd dy_;    // vector of size nknot-1
  Eigen::VectorXd a_;     // vector of size nknot-1
  Eigen::VectorXd b_;     // vector of size nknot-1

  Eigen::MatrixXd a_gradient_; // matrix of size nknot+1 x nknot-1
  Eigen::MatrixXd b_gradient_; // matrix of size nknot+1 x nknot-1
  
  bool normalize_           = true;
  double norm_              = 1.0;
  Eigen::VectorXd norm_gradient_; // vector of size nknot_+1;
};

} } } // namespace calin::math::pdf_1d

