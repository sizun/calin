/* 

   calin/tools/swig_generator.cpp -- Stephen Fegan -- 2015-12-03

   Code generator for SWIG interface files

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

#include <iostream>
#include <sstream>
#include <numeric>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "swig_generator.hpp"

using std::string;
using namespace calin::tools::protoc_gen_swig;
using google::protobuf::io::Printer;

SwigGenerator::~SwigGenerator()
{

}

namespace {

std::vector<std::string>& 
split(const std::string &s, char delim, std::vector<std::string> &elems)
{
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) { elems.push_back(item); }
  return elems;
}

std::vector<std::string> 
split(const std::string &s, char delim)
{
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

std::string join(std::vector<std::string>::const_iterator begin,
                 std::vector<std::string>::const_iterator end,
                 const std::string& sep)
{
  std::string result;
  auto it = begin;
  if (it != end) {
    result.append(*it);
    for(++it ; it!=end; ++it)result.append(sep).append(*it); }
  return result;
}

std::string join(const std::vector<std::string>& vec, const std::string& sep)
{
  return join(vec.cbegin(), vec.cend(), sep);
}

string pb_to_gen_filename(string pb_filename, string extension = ".pb.i")
{
  return pb_filename.substr(0,pb_filename.find_last_of('.')) + extension;
}

void print_includes(Printer* I, const google::protobuf::FileDescriptor * file,
                    string directive, string extension, bool include_self)
{
  if(include_self)
    I->Print("$directive$<$file$>\n", "directive", directive, "file",
             pb_to_gen_filename(file->name(),extension));
  for(int i=0; i<file->dependency_count(); i++)
    I->Print("$directive$<$file$>\n","directive", directive, "file",
             pb_to_gen_filename(file->dependency(i)->name(),extension));
  for(int i=0; i<file->public_dependency_count(); i++)
    I->Print("$directive$<$file$>\n","directive", directive, "file",
             pb_to_gen_filename(file->public_dependency(i)->name(),extension));
  for(int i=0; i<file->weak_dependency_count(); i++) 
    I->Print("$directive$<$file$>\n","directive", directive, "file",
             pb_to_gen_filename(file->weak_dependency(i)->name(),extension));
}

// Make the name of the class - handling nested types
std::string class_name(const google::protobuf::Descriptor* d)
{
  std::string class_name = d->name();
  while((d = d->containing_type()))class_name = d->name() + "_" + class_name;
  return class_name;
}

// Make the full type for a message for use as an argument
std::string class_type(const google::protobuf::Descriptor* d,
                       const google::protobuf::Descriptor* d_referrer = nullptr)
{
  std::string class_type = d->name();
  const google::protobuf::Descriptor* d_sub = d;
  while((d_sub = d_sub->containing_type()))
  {
    if(d_sub == d_referrer)return class_type;
    class_type = d_sub->name() + "_" + class_type;
  }
  if(d_referrer and d_referrer->file()->package() == d->file()->package())
    return class_type;
  else return join(split(d->file()->package(),'.'), "::")+"::"+class_type;
}

void print_fwd_decl(Printer* I, const google::protobuf::Descriptor* d)
{
  for(int i=0; i<d->nested_type_count(); i++)
    print_fwd_decl(I, d->nested_type(i));
  std::string the_class_name = class_name(d);
  I->Print("class $class_name$;\n", "class_name", the_class_name);
}

void print_message(Printer* I, const google::protobuf::Descriptor* d)
{
  for(int i=0; i<d->nested_type_count(); i++)
    print_message(I, d->nested_type(i));
  
  std::string the_class_name = class_name(d);
  I->Print("\n"
           "class $class_name$ : public google::protobuf::Message \n"
           "{\n"
           " public:\n"
           "  $class_name$();\n"
           "  ~$class_name$();\n"
           "  $class_name$(const $class_name$& other);\n"
           "  void Swap($class_name$* other);\n"
           "\n"
           "  static const google::protobuf::Descriptor* descriptor();\n"
           "  static const $class_name$& default_instance();\n",
           //"\n"
           //"  google::protobuf::Message* New() const override;\n"
           //"  void CopyFrom(const google::protobuf::Message & from) override;\n"
           //"  void MergeFrom(const google::protobuf:: Message & from) override;\n"
           //"  int SpaceUsed() const override;\n",
           "class_name", the_class_name);
  I->Indent();

  // Typedefs for nested types
  if(d->nested_type_count())
  {
    I->Print("\n");
    for(int i=0; i<d->nested_type_count(); i++)
      I->Print("typedef $local$ $full$;\n", "local", d->nested_type(i)->name(),
               "full", class_name(d->nested_type(i)));
  }

  // Enums

  // Oneof constants

  // Fields
  for(int i=0;i<d->field_count();i++)
  {
    auto* f = d->field(i);
    I->Print("\n");
    std::map<string, string> vars;
    vars["name"]   = f->name();
    vars["type"]   = f->cpp_type_name();
    vars["index"]  = "";
    vars["class_name"] = the_class_name;
    
    if(f->is_map())
    {
      // This is handled in the "%extend" section later
      continue;
    }

    if(f->containing_oneof())
      I->Print(vars, "bool has_$name$() const;\n");

    if(f->is_repeated())
    {
      I->Print(vars, "int $name$_size() const;\n");
      vars["index"]  = "int index";
    }

    if(f->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
    {
      vars["type"]   = class_type(f->message_type(), d);
      I->Print(vars, "%rename(const_$name$) $name$($index$) const;\n");
      I->Print(vars, "const $type$& $name$($index$) const;\n");
      I->Print(vars, "$type$* mutable_$name$($index$);\n");
      if(f->is_repeated())I->Print(vars, "$type$* add_$name$();\n");
    }
    else
    {
      if(f->cpp_type_name() == std::string("string"))
        vars["type"] = "const std::string&";

      I->Print(vars, "$type$ $name$($index$) const;\n");
      if(f->is_repeated())
      {
        I->Print(vars, "void set_$name$($index$, $type$ x);\n");
        I->Print(vars, "void add_$name$($type$ x);\n");
      }        
      else
        I->Print(vars, "void set_$name$($type$ x);\n");
    }

    I->Print(vars, "void clear_$name$();\n");
  }
  
  I->Outdent();
  I->Print("};\n");
}

} // anonymous namespace

bool SwigGenerator::
Generate(const google::protobuf::FileDescriptor * file,
         const string & parameter,
         google::protobuf::compiler::GeneratorContext *context,
         string * error) const
{
  auto I_stream = context->Open(pb_to_gen_filename(file->name(),".pb.i"));
  Printer* I = new Printer(I_stream,'$');

  I->Print("// Auto-generated from \"$file$\". "
           "Do not edit by hand.\n\n","file",file->name());
  
  std::map<string,string> vars;
  if(file->package().find('.') != string::npos)
  {
    auto index = file->package().find_last_of('.');
    I->Print("%module (package=\"$package$\") $module$\n",
             "package", file->package().substr(0,index),
             "module", file->package().substr(index+1));
  }
  else
  {
    I->Print("%module $module$\n",
             "module", file->package());
  }

  I->Print("\n%{\n");
  I->Indent();
  I->Print("#include<cstdint>\n");
  I->Print("#include<string>\n");
  I->Print("#include<vector>\n");
  I->Print("#include<map>\n");
  I->Print("#include<google/protobuf/message.h>\n");
  I->Print("#include<google/protobuf/descriptor.h>\n");
  print_includes(I, file, "#include", ".pb.h", true);
  I->Print("#define SWIG_FILE_WITH_INIT\n");
  I->Outdent();
  I->Print("%}\n\n");

  I->Print("%init %{\n"
           "  import_array();\n"
           "%}\n\n");

  I->Print("%include<numpy.i>\n");
  I->Print("%include<stdint.i>\n");
  I->Print("%include<std_string.i>\n");
  //I->Print("%include<std_vector.i>\n");
  //I->Print("%include<std_map.i>\n");

  I->Print("%import<package_wide_definitions.i>\n");
  I->Print("%import<google_protobuf.i>\n");
  print_includes(I, file, "%import", ".pb.i", false);

  I->Print("\n"
           "#define int32 int32_t\n"
           "#define uint32 uint32_t\n"
           "#define int64 int64_t\n"
           "#define uint64 uint64_t\n");

  std::vector<string> package_bits = split(file->package(), '.');
  if(!package_bits.empty())
  {
    I->Print("\n");
    for(auto ibit : package_bits)I->Print("namespace $bit$ { ","bit",ibit);
    I->Print("\n");
  }

  // Print forward declaration for all classes
  for(int i=0;i<file->message_type_count();i++)
  {
    if(i==0)I->Print("\n");
    print_fwd_decl(I, file->message_type(i));
  }
  
  // Print classes for all messages
  for(int i=0;i<file->message_type_count();i++)
    print_message(I, file->message_type(i));

  if(!package_bits.empty())
  {
    I->Print("\n");
    for(auto ibit : package_bits)I->Print("} ");
    I->Print("// namespace $ns$\n","ns",join(package_bits,"::"));
  }
  
  delete I;
  delete I_stream;
  return true;
}
