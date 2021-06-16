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
/// \file   TH1Reductor.cxx
/// \author Piotr Konopka
///

#include <TH1.h>
#include <TCanvas.h>  // LOKI
#include <TList.h>  // LOKI
#include "TPC/TH1ReductorTPC.h"
#include "TAxis.h"
#include <vector>

namespace o2::quality_control_modules::tpc
{

void* TH1ReductorTPC::getBranchAddress()
{
  return &mStats;
}

const char* TH1ReductorTPC::getBranchLeafList()
{
  return Form("mean[%i]/D:stddev[%i]:entries[%i]", NMAXSLICES, NMAXSLICES, NMAXSLICES); //"&mean:&stddev:&entries";
}

void TH1ReductorTPC::update(TObject* obj,std::vector<std::vector<float>> &axis, bool isCanvas)
{
  TList *padList = nullptr;   // LOKI: List of pads in case of TCanvas.
  Int_t nPads = 1; // LOKI: Number of pads in obj ("1" for TH1, > 1 for TCanvas)
  TH1 *histo = nullptr; // LOKI

  if (isCanvas) { // LOKI: Get the histograms from the canvas if isCanvas = true.
    printf("We have a canvas.\n");

    auto canvas = dynamic_cast<TCanvas*>(obj);  // LOKI
    padList = dynamic_cast<TList*>(canvas->GetListOfPrimitives()); // LOKI. Get the list of TPads
    nPads = padList->GetEntries();
  }
  printf("Number of pads: %d\n", nPads);

  for (Int_t iPad = 0; iPad < nPads; iPad++){
    if (isCanvas) {
      auto pad = dynamic_cast<TPad*>(padList->At(iPad));
      histo = dynamic_cast<TH1*>(pad->GetListOfPrimitives()->At(0));
    } else {  // LOKI: we don't receive an initial canvas but directly the TH1.
    histo = dynamic_cast<TH1*>(obj);
    }

    if( 1 == (int)axis.size()) 
    {

    // todo: use GetStats() instead?
    //auto histo = dynamic_cast<TH1*>(obj);
    if (histo) {
      for(int i=0; i<(int)axis.size(); i++)
  	  {
    		for(int j=0; j<(int)axis[i].size()-1; j++)
    		{	  
    		  histo->GetXaxis()->SetRangeUser(axis[i][j],axis[i][j+1]);
    		  mStats.entries[j] = histo->GetEntries();
    		  mStats.stddev[j] = histo->GetStdDev();
    		  mStats.mean[j] = histo->GetMean();

    		  //printf("Mean from %f to %f: %f \n", axis[i][j],axis[i][j+1],mStats.mean[j]);
          printf("Mean for pad %d: %f \n", iPad, mStats.mean[j]);

    		}
  	  }

     } // if (histo)
    } // if( 1 == (int)axis.size()) 

  }
}

} // namespace o2::quality_control_modules::common
