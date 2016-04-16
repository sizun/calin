  /*

   calin/diagnostics/waveform.hpp -- Stephen Fegan -- 2016-02-10

   Waveform diagnostics visitor

   Copyright 2016, Stephen Fegan <sfegan@llr.in2p3.fr>
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

#include <fftw3.h>

#include <iact_data/event_visitor.hpp>
#include <iact_data/functional_event_visitor.hpp>
#include <diagnostics/waveform.pb.h>

namespace calin { namespace diagnostics { namespace waveform {

class WaveformStatsVisitor:
  public iact_data::event_visitor::TelescopeEventVisitor
{
public:
  WaveformStatsVisitor(bool calculate_covariance = true);

  virtual ~WaveformStatsVisitor();

  bool demand_waveforms() override;
  bool is_parallelizable() override;
  WaveformStatsVisitor* new_sub_visitor(
    const std::map<calin::iact_data::event_visitor::TelescopeEventVisitor*,
        calin::iact_data::event_visitor::TelescopeEventVisitor*>&
      antecedent_visitors) override;

  bool visit_telescope_run(
    const calin::ix::iact_data::telescope_run_configuration::
      TelescopeRunConfiguration* run_config) override;
  bool leave_telescope_run() override;

  bool visit_telescope_event(
    calin::ix::iact_data::telescope_event::TelescopeEvent* event) override;

  bool visit_waveform(unsigned ichan,
    calin::ix::iact_data::telescope_event::ChannelWaveform* high_gain,
    calin::ix::iact_data::telescope_event::ChannelWaveform* low_gain) override;

  bool merge_results() override;

  calin::ix::diagnostics::waveform::CameraWaveformRawStats results()
  {
    return results_;
  }

  static Eigen::VectorXd waveform_mean(
    const ix::diagnostics::waveform::WaveformRawStats* stat);
  static Eigen::VectorXd waveform_var(
    const ix::diagnostics::waveform::WaveformRawStats* stat);
  static Eigen::MatrixXd waveform_cov(
    const ix::diagnostics::waveform::WaveformRawStats* stat);

protected:
  void process_one_waveform(
    const calin::ix::iact_data::telescope_event::ChannelWaveform* wf,
    ix::diagnostics::waveform::PartialWaveformRawStats* p_stat,
    ix::diagnostics::waveform::WaveformRawStats* r_stat);

  void merge_one_gain(
    const ix::diagnostics::waveform::WaveformRawStats* from,
    ix::diagnostics::waveform::WaveformRawStats* to);

  void merge_partial(
    ix::diagnostics::waveform::PartialWaveformRawStats* p_stat,
    ix::diagnostics::waveform::WaveformRawStats* r_stat);

  WaveformStatsVisitor* parent_ = nullptr;
  calin::ix::diagnostics::waveform::CameraWaveformRawStats results_;
  calin::ix::diagnostics::waveform::PartialCameraWaveformRawStats partial_;
  unsigned partial_max_num_entries_ = 256;
  const ix::iact_data::telescope_run_configuration::TelescopeRunConfiguration*
    run_config_ = nullptr;
  bool calculate_covariance_ = false;
};

class WaveformPSDVisitor:
  public iact_data::event_visitor::TelescopeEventVisitor
{
public:
  WaveformPSDVisitor();

  virtual ~WaveformPSDVisitor();

  bool demand_waveforms() override;
  bool is_parallelizable() override;
  WaveformPSDVisitor* new_sub_visitor(
    const std::map<calin::iact_data::event_visitor::TelescopeEventVisitor*,
        calin::iact_data::event_visitor::TelescopeEventVisitor*>&
      antecedent_visitors) override;

  bool visit_telescope_run(
    const calin::ix::iact_data::telescope_run_configuration::
      TelescopeRunConfiguration* run_config) override;
  bool leave_telescope_run() override;

  bool visit_telescope_event(
    calin::ix::iact_data::telescope_event::TelescopeEvent* event) override;

  bool visit_waveform(unsigned ichan,
    calin::ix::iact_data::telescope_event::ChannelWaveform* high_gain,
    calin::ix::iact_data::telescope_event::ChannelWaveform* low_gain) override;

  bool merge_results() override;

  calin::ix::diagnostics::waveform::CameraWaveformRawPSD results()
  {
    return results_;
  }

  static Eigen::VectorXd psd_mean(
    const ix::diagnostics::waveform::WaveformRawPSD* stat);
  static Eigen::VectorXd psd_var(
    const ix::diagnostics::waveform::WaveformRawPSD* stat);

  static Eigen::VectorXd corr_mean(
    const ix::diagnostics::waveform::WaveformRawPSD* psd_stat,
    const ix::diagnostics::waveform::WaveformRawStats* trace_stat);
  static Eigen::VectorXd corr_mean_centered(
    const ix::diagnostics::waveform::WaveformRawPSD* stat,
    const ix::diagnostics::waveform::WaveformRawStats* trace_stat,
    Eigen::VectorXd& h);
  static Eigen::VectorXd corr_var(
    const ix::diagnostics::waveform::WaveformRawPSD* stat,
    const ix::diagnostics::waveform::WaveformRawStats* trace_stat);

protected:
  void process_one_waveform(
    const calin::ix::iact_data::telescope_event::ChannelWaveform* wf,
    ix::diagnostics::waveform::WaveformRawPSD* psd);

  void merge_one_gain(
    const ix::diagnostics::waveform::WaveformRawPSD* from,
    ix::diagnostics::waveform::WaveformRawPSD* to);

  WaveformPSDVisitor* parent_ = nullptr;
  calin::ix::diagnostics::waveform::CameraWaveformRawPSD results_;
  const ix::iact_data::telescope_run_configuration::TelescopeRunConfiguration*
    run_config_ = nullptr;
  double* fftw_data_ = nullptr;
  fftw_plan fftw_plan_fwd_ = nullptr;
  fftw_plan fftw_plan_bwd_ = nullptr;
};

class FunctionalWaveformStatsVisitor:
  public iact_data::event_visitor::TelescopeEventVisitor
{
public:
  FunctionalWaveformStatsVisitor(
    calin::iact_data::functional_event_visitor::
      DualValueInt32FunctionalTelescopeEventVisitor* value_supplier,
    const calin::ix::diagnostics::waveform::
      FunctionalWaveformStatsVisitorConfig& config = default_config());

  virtual ~FunctionalWaveformStatsVisitor();

  bool demand_waveforms() override;
  bool is_parallelizable() override;
  FunctionalWaveformStatsVisitor* new_sub_visitor(
    const std::map<calin::iact_data::event_visitor::TelescopeEventVisitor*,
        calin::iact_data::event_visitor::TelescopeEventVisitor*>&
      antecedent_visitors) override;

  bool visit_telescope_run(
    const calin::ix::iact_data::telescope_run_configuration::
      TelescopeRunConfiguration* run_config) override;
  bool leave_telescope_run() override;

  bool visit_telescope_event(
    calin::ix::iact_data::telescope_event::TelescopeEvent* event) override;
  bool leave_telescope_event() override;

  bool visit_waveform(unsigned ichan,
    calin::ix::iact_data::telescope_event::ChannelWaveform* high_gain,
    calin::ix::iact_data::telescope_event::ChannelWaveform* low_gain) override;

  bool merge_results() override;

  calin::ix::diagnostics::waveform::CameraIntFunctionalWaveformRawStats results()
  {
    return results_;
  }

  static Eigen::VectorXd channel_mean(
    const ix::diagnostics::waveform::OneGainIntFunctionalWaveformRawStats* stat);

  static Eigen::VectorXd channel_var(
    const ix::diagnostics::waveform::OneGainIntFunctionalWaveformRawStats* stat);

  static Eigen::MatrixXd channel_cov(
    const ix::diagnostics::waveform::OneGainIntFunctionalWaveformRawStats* stat);

  static calin::ix::diagnostics::waveform::FunctionalWaveformStatsVisitorConfig
  default_config()
  {
    calin::ix::diagnostics::waveform::FunctionalWaveformStatsVisitorConfig cfg;
    cfg.set_calculate_covariance(true);
    return cfg;
  }

protected:
  void visit_one_waveform(
    const calin::ix::iact_data::telescope_event::ChannelWaveform* wf,
    unsigned index, std::vector<int>& mask, std::vector<int32_t>& signal);

  void process_one_gain(const std::vector<int>& mask,
    const std::vector<int32_t>& signal,
    calin::ix::diagnostics::waveform::OneGainIntFunctionalWaveformRawStats* stats);

  void merge_one_gain(
    const ix::diagnostics::waveform::OneGainIntFunctionalWaveformRawStats* from,
    ix::diagnostics::waveform::OneGainIntFunctionalWaveformRawStats* to);

  calin::iact_data::functional_event_visitor::
    DualValueInt32FunctionalTelescopeEventVisitor* value_supplier_;
  calin::ix::diagnostics::waveform::
    FunctionalWaveformStatsVisitorConfig config_;

  std::vector<int> high_gain_mask_;
  std::vector<int32_t> high_gain_signal_;
  std::vector<int> low_gain_mask_;
  std::vector<int32_t> low_gain_signal_;

  FunctionalWaveformStatsVisitor* parent_ = nullptr;
  calin::ix::diagnostics::waveform::CameraIntFunctionalWaveformRawStats results_;
  const ix::iact_data::telescope_run_configuration::TelescopeRunConfiguration*
    run_config_ = nullptr;
  bool calculate_covariance_ = false;
};


} } } // namespace calin::diagnostics::waveform_diagnostics
