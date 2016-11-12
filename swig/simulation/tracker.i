//-*-mode:swig;-*-

/*

   calin/simulation/tracker.i -- Stephen Fegan -- 2016-10-06

   SWIG interface file for calin.simulation.tracker

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

%module (package="calin.simulation") tracker

%{
#include "simulation/tracker.hpp"
#include "simulation/straight_track_generator.hpp"
#include "simulation/air_cherenkov_tracker.hpp"
#include "simulation/iact_array_tracker.hpp"
#include "simulation/quadrature_iact_array_integration.hpp"
#include "simulation/vso_quadrature_iact_array_integration.hpp"
#define SWIG_FILE_WITH_INIT
  %}

%init %{
  import_array();
%}

%include "calin_typemaps.i"
%import "calin_global_definitions.i"

%import "simulation/atmosphere.i"
%import "simulation/detector_efficiency.i"
%import "simulation/vs_optics.i"
%import "simulation/tracker.pb.i"
//%newobject *::dump_as_proto() const;
//%newobject *::create_from_proto;

%include "simulation/tracker.hpp"
%include "simulation/straight_track_generator.hpp"
%include "simulation/air_cherenkov_tracker.hpp"
%include "simulation/iact_array_tracker.hpp"
%include "simulation/quadrature_iact_array_integration.hpp"
%include "simulation/vso_quadrature_iact_array_integration.hpp"