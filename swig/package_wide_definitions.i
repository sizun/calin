//-*-mode:swig;-*-

/* 

   calin/package_wide_definitions.i -- Stephen Fegan -- 2015-04-21

*/

%module (package="calin") package_wide_definitions

%{
#include <iostream>
#include "package_wide_definitions.hpp"
#define SWIG_FILE_WITH_INIT
  %}

%include "numpy.i"

%init %{
  import_array();
%}

%import "package_wide_definitions.hpp"

#ifdef CALIN_USE_EIGEN_REF

%typemap(in,
         fragment="NumPy_Fragments")
   calin::ConstVecRef
(PyArrayObject* array=NULL, int is_new_object=0,
 Eigen::Map<const Eigen::VectorXd>* eigenmap=0)
{
  npy_intp size[1] = { -1 };
  array = obj_to_array_contiguous_allow_conversion($input,
                                                   NPY_DOUBLE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 1) ||
      !require_size(array, size, 1)) SWIG_fail;
  eigenmap =
      new Eigen::Map<const Eigen::VectorXd>((const double*)array_data(array),
                                            array_size(array,0));
  $1 = new $*1_ltype(*eigenmap);
}

%typemap(freearg) calin::ConstVecRef
{
  delete arg$argnum;
  delete eigenmap$argnum;
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}

%typemap(in,
         fragment="NumPy_Fragments")
   calin::VecRef
(PyArrayObject* array=NULL, int is_new_object=0,
 Eigen::Map<Eigen::VectorXd>* eigenmap=0)
{
  npy_intp size[1] = { -1 };
  array = obj_to_array_contiguous_allow_conversion($input,
                                                   NPY_DOUBLE,
                                                   &is_new_object);
  if (!array || !require_dimensions(array, 1) ||
      !require_size(array, size, 1)) SWIG_fail;
  eigenmap =
      new Eigen::Map<const Eigen::VectorXd>((const double*)array_data(array),
                                            array_size(array,0));
  $1 = new $*1_ltype(*eigenmap);
}

%typemap(freearg) calin::VecRef
{
  delete arg$argnum;
  delete eigenmap$argnum;
  if (is_new_object$argnum && array$argnum)
    { Py_DECREF(array$argnum); }
}


#else // ifdef CALIN_USE_EIGEN_REF

%fragment("Calin_Python_to_Eigen",
          "header",
          fragment="NumPy_Array_Requirements",
          fragment="NumPy_Backward_Compatibility",
          fragment="NumPy_Macros",
          fragment="NumPy_Utilities")
{
  static bool calin_python_to_eigen_vec(PyObject* input, Eigen::VectorXd& vec)
  {
    const int typecode = NPY_DOUBLE;

    if(!is_array(input))
      {
        const char* desired_type = typecode_string(typecode);
        const char* actual_type  = pytype_string(input);
        PyErr_Format(PyExc_TypeError,
                     "Array of type '%s' required.  A '%s' was given",
                     desired_type,
                     actual_type);
        return false;
      }
    
    PyArrayObject* in_array = (PyArrayObject*) input;

    npy_intp size[1] = { -1 };
    if(!require_dimensions(in_array, 1) or 
       !require_size(in_array, size, 1)) return false;        

    size[0] = array_size(in_array,0);
    vec.resize(size[0]);

    PyArrayObject* out_array = (PyArrayObject*)
        PyArray_SimpleNewFromData(1, size, typecode, vec.data());
    if(out_array == nullptr)return false;
        
    if(PyArray_CopyInto(out_array, in_array) != 0)
      {
        Py_DECREF(out_array);
        return false;
      }

    Py_DECREF(out_array);
    return true;
  }

  static bool calin_eigen_vec_to_python(Eigen::VectorXd& vec,
                                        PyObject* output)
  {
    const int typecode = NPY_DOUBLE;

    if(!is_array(output))
      {
        const char* desired_type = typecode_string(typecode);
        const char* actual_type  = pytype_string(output);
        PyErr_Format(PyExc_TypeError,
                     "Array of type '%s' required.  A '%s' was given",
                     desired_type,
                     actual_type);
        return false;
      }

    npy_intp size[1] = { vec.size() };
    PyArrayObject* in_array = (PyArrayObject*)
        PyArray_SimpleNewFromData(1, size, typecode, vec.data());
    if(in_array == nullptr)
      {
        return false;
      }

    PyArrayObject* out_array = (PyArrayObject*) output;

    std::cout << "IN:" << output->ob_refcnt << '\n';

    PyArray_Dims dims = { size, 1 };
    if(PyArray_Resize(out_array, &dims, 1, NPY_ANYORDER) == nullptr)
      {
        // Do we need to call Py_DECREF on returned val??
        Py_DECREF(in_array);
        return false;  
      }

    std::cout << "OUT:" << output->ob_refcnt << '\n';
    
    if(PyArray_CopyInto(out_array, in_array) != 0)
      {
        Py_DECREF(in_array);
        return false;
      }

    Py_DECREF(in_array);
    return true;
  }

}

%typemap(in,
         fragment="Calin_Python_to_Eigen")
   calin::ConstVecRef
{
  $1 = new Eigen::VectorXd();
  if(!calin_python_to_eigen_vec($input, *$1))SWIG_fail;
}

%typemap(freearg) calin::ConstVecRef
{
  delete arg$argnum;
}

%typemap(in,
         fragment="Calin_Python_to_Eigen")
   calin::VecRef
{
  $1 = new Eigen::VectorXd();
  if(!calin_python_to_eigen_vec($input, *$1))SWIG_fail;
}

%typemap(argout,
         fragment="Calin_Python_to_Eigen")
   calin::VecRef
{
  if(!calin_eigen_vec_to_python(*$1, $input))SWIG_fail;
}

%typemap(freearg) calin::VecRef
{
  delete arg$argnum;
}

#endif
