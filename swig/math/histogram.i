//-*-mode:swig;-*-

/* 

   calin/math/histogram.i -- Stephen Fegan -- 2015-04-23

*/

%module (package="calin.math") histogram

%{
#include "math/histogram.hpp"
#define SWIG_FILE_WITH_INIT
  %}

%init %{
  import_array();
%}

%include "package_wide_definitions.i"

%include "math/histogram.hpp"