// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file    TrendingTPC.h
/// \author  Cindy Mordasini
/// \author  Marcel Lesch
/// Structure of the initial task taken from /Framework/.../TrendingTask.*
///

#ifndef QC_MODULE_TPC_TRENDING_H
#define QC_MODULE_TPC_TRENDING_H

#include "QualityControl/PostProcessingInterface.h"
#include "QualityControl/TrendingTaskConfig.h"
#include "QualityControl/Reductor.h"

#include <memory>
#include <unordered_map>
#include <TTree.h>

namespace o2::quality_control::repository
{
class DatabaseInterface;
}

using namespace o2::quality_control::postprocessing;

namespace o2::quality_control_modules::tpc
{

/// \brief  A post-processing task which trends values, stores them in a TTree and produces plots.
///
/// A post-processing task which trends objects inside QC database (QCDB). It extracts some values of one or multiple
/// objects using the Reductor classes, then stores them inside a TTree. One can generate plots out the TTree - the
/// class exposes the TTree::Draw interface to the user. The TTree and plots are stored in the QCDB. The class is
/// configured with configuration files, see Framework/postprocessing.json as an example.
///
/// \author Piotr Konopka
class TrendingTPC : public PostProcessingInterface
{
 public:
  TrendingTPC() = default;
  ~TrendingTPC() override = default;

  void configure(std::string name, o2::configuration::ConfigurationInterface& config) override;
  void initialize(Trigger, framework::ServiceRegistry&) override;
  void update(Trigger, framework::ServiceRegistry&) override;
  void finalize(Trigger, framework::ServiceRegistry&) override;

 private:
  struct MetaData {
    Int_t runNumber = 0;
  };

  void trendValues();
  void storePlots();
  void storeTrend();

  TrendingTaskConfig mConfig;
  MetaData mMetaData;
  UInt_t mTime;
  std::unique_ptr<TTree> mTrend;
  std::unordered_map<std::string, std::unique_ptr<Reductor>> mReductors;
  o2::quality_control::repository::DatabaseInterface* mDatabase = nullptr;
};

} // namespace o2::quality_control_modules::tpc

#endif //QC_MODULE_TPC_TRENDING_H
