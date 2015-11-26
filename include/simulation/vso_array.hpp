/* 

   calin/simulation/vso_array.hpp -- Stephen Fegan -- 2015-11-25

   Class for array of telescopes

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

//-*-mode:c++; mode:font-lock;-*-

/*! \file OSTelescopeArray.hpp
  Array class header file

  \author  Stephen Fegan               \n
           UCLA                        \n
	   sfegan@astro.ucla.edu       \n

  \author  Maciej Nicewicz             \n
           UCLA                        \n
	   nicewicz@physics.ucla.edu   \n

  \date    11/30/2004
  \version 0.2
  \note
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <math/vs_vec3d.hpp>
#include <math/vs_particle.hpp>
#include <math/rng.hpp>

#include <proto/simulation_vs_optics.pb.h>
#include <simulation/vso_telescope.hpp>

namespace calin { namespace simulation { namespace vs_optics {

class VSOArray
{
 public:
  VSOArray();
  virtual ~VSOArray();
    
  // ************************************************************************
  // Create a new array randomly using parameters
  // ************************************************************************
  void generateFromArrayParameters(
      const ix::simulation::vs_optics::IsotropicDCArrayParameters& param,
      math::rng::RNG& rng);
    
  // ************************************************************************
  // Other stuff
  // ************************************************************************
  bool pointTelescopesAzEl(const double az_rad, const double el_rad);
  bool pointTelescopes(const math::vs_physics::Vec3D& v);

  // ************************************************************************
  // Accessors
  // ************************************************************************
  double latitude() const { return fLatitude; }
  double longitude() const { return fLatitude; }
  double altitude() const { return fAltitude; }
#if 0
  double spacing() const { return fSpacing; }
  double arrayParity() const { return fArrayParity; }
#endif
  
  unsigned numTelescopes() const { return fTelescopes.size(); }
#if 0
  unsigned numTelescopeHexSites() const { return fTelescopesByHexID.size(); }
#endif
  
  inline const VSOTelescope* telescope(unsigned id) const;
#if 0
  inline const VSOTelescope* telescopeByHexID(unsigned hexID) const;
#endif
  
  inline VSOTelescope* telescope(unsigned id);
#if 0
  inline VSOTelescope* telescopeByHexID(unsigned hexID);
#endif
  
  // ************************************************************************
  // Dump
  // ************************************************************************

  void dump_to_proto(ix::simulation::vs_optics::VSOArrayData* d) const;
  static VSOArray*
  create_from_proto(const ix::simulation::vs_optics::VSOArrayData& d);

#if 0
  void dump(std::ostream& stream, unsigned l=0) const;
  void dumpShort(const std::string& filename) const;
  void dumpShort(std::ostream& stream) const;
  void writeToShortDump(const std::string& filename) const { dumpShort(filename); }
  void writeToShortDump(std::ostream& stream) const { dumpShort(stream); }
  bool readFromShortDump(std::istream& stream);
  bool readFromShortDump(const std::string& filename);
#endif
  
 private:
  double                     fLatitude;
  double                     fLongitude;
  double                     fAltitude;
#if 0
  double                     fSpacing;
  bool                       fArrayParity;
#endif
  
  std::vector<VSOTelescope*>  fTelescopes;
#if 0
  std::vector<VSOTelescope*>  fTelescopesByHexID;
#endif
};

inline const VSOTelescope* VSOArray::telescope(unsigned id) const
{
  if(id>=fTelescopes.size())return 0;
  else return fTelescopes[id];
}

#if 0
inline const VSOTelescope* VSOArray::telescopeByHexID(unsigned hexID) const
{
  if((hexID<1)||(hexID>fTelescopes.size()))return 0;
  else return fTelescopes[hexID-1];
}
#endif
  
inline VSOTelescope* VSOArray::telescope(unsigned id)
{
  if(id>=fTelescopes.size())return 0;
  else return fTelescopes[id];
}

#if 0
inline VSOTelescope* VSOArray::telescopeByHexID(unsigned hexID)
{
  if((hexID<1)||(hexID>fTelescopes.size()))return 0;
  else return fTelescopes[hexID-1];
}
#endif

} } } // namespace calin::simulation::vs_optics