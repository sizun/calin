/*

   calin/iact_data/zfits_acts_data_source.hpp -- Stephen Fegan -- 2016-05-04

   A supplier of ACTL telescope data types from CTA ACTL ZFits data files

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

#include <string>

#include <calin_global_definitions.hpp>
#include <calin_global_config.hpp>
#include <io/data_source.hpp>
#include <io/chained_data_source.hpp>
#include <iact_data/zfits_data_source.pb.h>

#ifdef CALIN_HAVE_CTA_CAMERASTOACTL

#include <ProtobufIFits.h>
#include <L0.pb.h>

namespace calin { namespace iact_data { namespace zfits_actl_data_source {

CALIN_TYPEALIAS(ACTLRandomAccessDataSource,
  calin::io::data_source::RandomAccessDataSource<DataModel::CameraEvent>);

} } } // namespace calin::iact_data::zfits_actl_data_source

#ifdef SWIG
%template(ACTLRandomAccessDataSource)
  calin::io::data_source::RandomAccessDataSource<DataModel::CameraEvent>;
#endif

namespace calin { namespace iact_data { namespace zfits_actl_data_source {

class ACTLRandomAccessDataSourceWithRunHeader:
  public calin::io::data_source::RandomAccessDataSource<DataModel::CameraEvent>
{
public:
  ACTLRandomAccessDataSourceWithRunHeader():
    calin::io::data_source::RandomAccessDataSource<DataModel::CameraEvent>() {
    /* nothing to see here */ }
  virtual ~ACTLRandomAccessDataSourceWithRunHeader();
  virtual DataModel::CameraRunHeader* get_run_header() = 0;
};

CALIN_TYPEALIAS(ACTLChainedRandomAccessDataSourceWithRunHeader,
  calin::io::data_source::BasicChainedRandomAccessDataSource<
    ACTLRandomAccessDataSourceWithRunHeader>);

CALIN_TYPEALIAS(ACTLRandomAccessDataSourceOpener,
  calin::io::data_source::DataSourceOpener<
    ACTLRandomAccessDataSourceWithRunHeader>);

} } } // namespace calin::iact_data::zfits_actl_data_source

#ifdef SWIG
%template(ACTLChainedRandomAccessDataSourceWithRunHeader)
  calin::io::data_source::BasicChainedRandomAccessDataSource<
    calin::iact_data::zfits_actl_data_source::
      ACTLRandomAccessDataSourceWithRunHeader>;
%template(ACTLRandomAccessDataSourceOpener)
  calin::io::data_source::DataSourceOpener<
    calin::iact_data::zfits_actl_data_source::
      ACTLRandomAccessDataSourceWithRunHeader>;
#endif

namespace calin { namespace iact_data { namespace zfits_actl_data_source {

class ZFITSSingleFileACTLDataSource:
  public ACTLRandomAccessDataSourceWithRunHeader
{
public:
  CALIN_TYPEALIAS(config_type,
    calin::ix::iact_data::zfits_data_source::ZFITSDataSourceConfig);

  ZFITSSingleFileACTLDataSource(const std::string& filename,
    config_type config = default_config());
  virtual ~ZFITSSingleFileACTLDataSource();

  DataModel::CameraEvent* borrow_next_event(uint64_t& seq_index_out);
  void release_borrowed_event(DataModel::CameraEvent* event);

  DataModel::CameraEvent* get_next(uint64_t& seq_index_out,
    google::protobuf::Arena** arena = nullptr) override;
  uint64_t size() override;
  void set_next_index(uint64_t next_index) override;

  DataModel::CameraRunHeader* get_run_header() override;

  static config_type default_config();

private:
  std::string filename_;
  ACTL::IO::ProtobufIFits* zfits_ = nullptr;
  uint64_t next_event_index_ = 0;
  DataModel::CameraRunHeader* run_header_ = nullptr;
};

class ZFITSACTLDataSource:
  public calin::io::data_source::BasicChainedRandomAccessDataSource<
    ACTLRandomAccessDataSourceWithRunHeader>
{
public:
  CALIN_TYPEALIAS(config_type,
    calin::ix::iact_data::zfits_data_source::ZFITSDataSourceConfig);

  ZFITSACTLDataSource(const std::string& filename,
    const config_type& config = default_config());
  virtual ~ZFITSACTLDataSource();

  DataModel::CameraRunHeader* get_run_header() override;

  static config_type default_config() {
    return ZFITSSingleFileACTLDataSource::default_config(); }
  const config_type& config() const { return config_; }

private:
  config_type config_;
  DataModel::CameraRunHeader* run_header_ = nullptr;
};

class ZFITSACTLDataSourceOpener:
  public calin::io::data_source::DataSourceOpener<
    ACTLRandomAccessDataSourceWithRunHeader>
{
public:
  CALIN_TYPEALIAS(data_source_type, ACTLRandomAccessDataSourceWithRunHeader);
  ZFITSACTLDataSourceOpener(std::string filename,
    const ZFITSACTLDataSource::config_type& config =
      ZFITSACTLDataSource::default_config());
  virtual ~ZFITSACTLDataSourceOpener();
  unsigned num_sources() override;
  ACTLRandomAccessDataSourceWithRunHeader* open(unsigned isource) override;
private:
  std::vector<std::string> filenames_;
  ZFITSACTLDataSource::config_type config_;
  bool has_opened_file_ = false;
};

} } } // namespace calin::iact_data::zfits_actl_data_source

#endif
