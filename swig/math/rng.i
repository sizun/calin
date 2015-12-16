//-*-mode:swig;-*-

/* 

   calin/math/function.i -- Stephen Fegan -- 2015-04-15

   SWIG interface file for calin.math.rng

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

%module (package="calin.math") rng

%{
#include "math/rng.hpp"
#define SWIG_FILE_WITH_INIT
  %}

%init %{
  import_array();
%}

%include "numpy.i"
%include "calin_typemaps.i"
%import "package_wide_definitions.i"

%import "math/rng.pb.i"
%include "math/rng.hpp"
