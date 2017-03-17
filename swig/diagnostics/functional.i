/*

   calin/diagnostics/waveform.i -- Stephen Fegan -- 2016-03-23

   SWIG interface file for calin waveform diagnostics

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

%module (package="calin.diagnostics") functional

%{
#include "diagnostics/functional.hpp"
#define SWIG_FILE_WITH_INIT
  %}

%init %{
  import_array();
%}

%include "calin_typemaps.i"
%import "calin_global_definitions.i"

%import "iact_data/event_dispatcher.i"
%import "iact_data/functional_event_visitor.i"
%import "diagnostics/value_capture.i"
%import "diagnostics/functional.pb.i"
%include "diagnostics/functional.hpp"

%template(FunctionalIntStatsVisitor)
calin::diagnostics::functional::FunctionalStatsVisitor<
  calin::iact_data::functional_event_visitor::
    DualGainInt32FunctionalTelescopeEventVisitor,
  calin::ix::diagnostics::functional::CameraIntFunctionalRawStats>;

%template(FunctionalDoubleStatsVisitor)
calin::diagnostics::functional::FunctionalStatsVisitor<
  calin::iact_data::functional_event_visitor::
    DualGainDoubleFunctionalTelescopeEventVisitor,
  calin::ix::diagnostics::functional::CameraDoubleFunctionalRawStats>;

%template(channel_mean_int)
calin::diagnostics::functional::channel_mean<
  calin::ix::diagnostics::functional::OneGainIntFunctionalRawStats>;
%template(channel_var_int)
calin::diagnostics::functional::channel_var<
  calin::ix::diagnostics::functional::OneGainIntFunctionalRawStats>;
%template(channel_cov_int)
calin::diagnostics::functional::channel_cov<
  calin::ix::diagnostics::functional::OneGainIntFunctionalRawStats>;
%template(channel_cov_frac_int)
calin::diagnostics::functional::channel_cov_frac<
  calin::ix::diagnostics::functional::OneGainIntFunctionalRawStats>;

%template(channel_mean_double)
calin::diagnostics::functional::channel_mean<
  calin::ix::diagnostics::functional::OneGainDoubleFunctionalRawStats>;
%template(channel_var_double)
calin::diagnostics::functional::channel_var<
  calin::ix::diagnostics::functional::OneGainDoubleFunctionalRawStats>;
%template(channel_cov_double)
calin::diagnostics::functional::channel_cov<
  calin::ix::diagnostics::functional::OneGainDoubleFunctionalRawStats>;
%template(channel_cov_frac_double)
calin::diagnostics::functional::channel_cov_frac<
  calin::ix::diagnostics::functional::OneGainDoubleFunctionalRawStats>;
