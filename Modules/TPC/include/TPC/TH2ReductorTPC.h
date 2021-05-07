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
/// \file   TH2Reductor.h
/// \author Piotr Konopka
///
#ifndef QUALITYCONTROL_TH2REDUCTORTPC_H
#define QUALITYCONTROL_TH2REDUCTORTPC_H

#include "TPC/ReductorTPC.h"
#include <vector>

namespace o2::quality_control_modules::tpc
{

/// \brief A Reductor which obtains the most popular characteristics of TH2.
///
/// A Reductor which obtains the most popular characteristics of TH2.
/// It produces a branch in the format: "sumw/D:sumw2:sumwx:sumwx2:sumwy:sumwy2:sumwxy:entries"
class TH2ReductorTPC : public quality_control::postprocessing::ReductorTPC
{
 public:
  TH2ReductorTPC() = default;
  ~TH2ReductorTPC() = default;

  void* getBranchAddress() override;
  const char* getBranchLeafList() override;
  void update(TObject* obj,std::vector<std::vector<float>> &axis) override;

 private:
 static constexpr int NMAXSLICES = 30;
  struct {
    Double_t sumw[NMAXSLICES];
    Double_t sumw2[NMAXSLICES];
    Double_t sumwx[NMAXSLICES];
    Double_t sumwx2[NMAXSLICES];
    Double_t sumwy[NMAXSLICES];
    Double_t sumwy2[NMAXSLICES];
    Double_t sumwxy[NMAXSLICES];
    Double_t entries[NMAXSLICES]; // is sumw == entries always? maybe not for values which land into the edge bins?
  } mStats;
};

} // namespace o2::quality_control_modules::tpc

#endif //QUALITYCONTROL_TH2REDUCTORTPC_H
