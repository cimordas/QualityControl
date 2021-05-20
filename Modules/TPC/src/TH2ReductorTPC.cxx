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
/// \file   TH2Reductor.cxx
/// \author Piotr Konopka
///

#include <TH2.h>
#include "TPC/TH2ReductorTPC.h"
#include "TAxis.h"
#include <vector>

namespace o2::quality_control_modules::tpc
{

void* TH2ReductorTPC::getBranchAddress()
{
  return &mStats;
}

const char* TH2ReductorTPC::getBranchLeafList()
{
  return Form("sumw[%i]/D:sumw2[%i]:sumwx[%i]:sumwx2[%i]:sumwy[%i]:sumwy2[%i]:sumwxy[%i]:entries[%i]", NMAXSLICES, NMAXSLICES, NMAXSLICES, NMAXSLICES, NMAXSLICES, NMAXSLICES, NMAXSLICES, NMAXSLICES);
}

void TH2ReductorTPC::update(TObject* obj,std::vector<std::vector<float>> &axis)
{
  if( 1 == (int)axis.size()) //for now we just slice along the x axis
  {
  auto histo = dynamic_cast<TH2*>(obj);
  if (histo) {
 
  Double_t stats[7];

	   for(int i=0; i<(int)axis.size(); i++)
	   {
		for(int j=0; j<(int)axis[i].size()-1; j++)
		{		  
		  histo->GetXaxis()->SetRangeUser(axis[i][j],axis[i][j+1]);
		  histo->GetStats(stats);

		  mStats.sumw[j] = stats[0] ;
    		  mStats.sumw2[j] = stats[1];
    		  mStats.sumwx[j] = stats[2];
    	  	  mStats.sumwx2[j]= stats[3];
    	          mStats.sumwy[j] = stats[4];
    	          mStats.sumwy2[j] = stats[5];
    	 	  mStats.sumwxy[j] = stats[6];

		  mStats.entries[j] = histo->GetEntries();
		}
	   }

   } // if (histo)
  } // if( 1 == (int)axis.size()

}

} // namespace o2::quality_control_modules::common
