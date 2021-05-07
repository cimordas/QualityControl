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
/// \file   TH1ReductorTPC.h
/// \author Piotr Konopka
///
#ifndef QUALITYCONTROL_TH1REDUCTORTPC_H
#define QUALITYCONTROL_TH1REDUCTORTPC_H

#include "TPC/ReductorTPC.h"
#include <vector>

namespace o2::quality_control_modules::tpc
{

/// \brief A Reductor which obtains the most popular characteristics of TH1.
///
/// A Reductor which obtains the most popular characteristics of TH1.
/// It produces a branch in the format: "mean/D:stddev:entries"
class TH1ReductorTPC : public quality_control::postprocessing::ReductorTPC
{
 public:
  TH1ReductorTPC() = default;
  ~TH1ReductorTPC() = default;

  void* getBranchAddress() override;
  const char* getBranchLeafList() override;
  void update(TObject* obj,std::vector<std::vector<float>> &axis) override; 

  private:
  static constexpr int NMAXSLICES = 30;
  struct {
    Double_t mean[NMAXSLICES];
    Double_t stddev[NMAXSLICES];
    Double_t entries[NMAXSLICES];
  } mStats ;
};

} // namespace o2::quality_control_modules::tpc

#endif //QUALITYCONTROL_TH1REDUCTORTPC_H
