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
/// \file    TrendingTaskTPC.h
/// \author  Based on the work of Piotr Konopka
///

#ifndef QUALITYCONTROL_TRENDINGTASKTPC_H
#define QUALITYCONTROL_TRENDINGTASKTPC_H

#include "QualityControl/PostProcessingInterface.h"
#include "TPC/TrendingTaskConfigTPC.h"
#include "TPC/ReductorTPC.h"

#include <memory>
#include <unordered_map>
#include <TTree.h>

namespace o2::quality_control::repository
{
class DatabaseInterface;
}

namespace o2::quality_control::postprocessing
{

/// \brief  A post-processing task which trends values, stores them in a TTree and produces plots.
///
/// A post-processing task which trends objects inside QC database (QCDB). It extracts some values of one or multiple
/// objects using the Reductor classes, then stores them inside a TTree. One can generate plots out the TTree - the
/// class exposes the TTree::Draw interface to the user. The TTree and plots are stored in the QCDB. The class is
/// configured with configuration files, see Framework/postprocessing.json as an example.
///
/// \author based on work of Piotr Konopka
class TrendingTaskTPC : public PostProcessingInterface
{
 public:
  TrendingTaskTPC() = default;
  ~TrendingTaskTPC() override = default;

  void configure(std::string name, const boost::property_tree::ptree& config) override;
  void initialize(Trigger, framework::ServiceRegistry&) override;
  void update(Trigger, framework::ServiceRegistry&) override;
  void finalize(Trigger, framework::ServiceRegistry&) override;

 private:
  struct MetaData {
    Int_t runNumber = 0;
  };

  void trendValues(uint64_t timestamp, repository::DatabaseInterface&);
  void generatePlots();

  TrendingTaskConfigTPC mConfig;
  MetaData mMetaData;
  UInt_t mTime;
  std::unique_ptr<TTree> mTrend;
  std::map<std::string, TObject*> mPlots;
  std::unordered_map<std::string, std::unique_ptr<ReductorTPC>> mReductors;
};

} // namespace o2::quality_control::postprocessing

#endif //QUALITYCONTROL_TRENDINGTASKTPC_H
