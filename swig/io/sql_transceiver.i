//-*-mode:swig;-*-

/*

   calin/io/sql_transceiver.i -- Stephen Fegan -- 2015-12-15

   SWIG interface file for calin.io.sql_transceiver

   Copyright 2015, Stephen Fegan <sfegan@llr.in2p3.fr>
   LLR, Ecole Polytechnique, CNRS/IN2P3

   This file is part of "calin"

   "calin" is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License version 2 or
   later, as published by the Free Software Foundation.

   "calin" is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

*/

%module (package="calin.io") sql_transceiver
%feature(autodoc,2);

%{
#include "io/sql_transceiver.hpp"
#include "io/sqlite3_transceiver.hpp"
#define SWIG_FILE_WITH_INIT
  %}

%init %{
  import_array();
%}

%include "calin_typemaps.i"
%import "calin_global_definitions.i"

%ignore insert(const std::string& table_name,
               const google::protobuf::Message* m_data,
               const google::protobuf::Message* m_key = nullptr);

%ignore create_tables_and_insert(const std::string& table_name,
              const google::protobuf::Message* m_data,
              const google::protobuf::Message* m_key = nullptr,
              const std::string& instance_desc = "");

%apply uint64_t &OUTPUT { uint64_t& oid };

%include "io/sql_statement.hpp"
%include "io/sql_transceiver.hpp"
%include "io/sqlite3_statement.hpp"
%include "io/sqlite3_transceiver.hpp"
