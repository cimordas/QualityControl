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

void TH1ReductorTPC::update(TObject* obj,std::vector<std::vector<float>> &axis)
{ 
  if( 1 == (int)axis.size()) 
  {

  // todo: use GetStats() instead?
  auto histo = dynamic_cast<TH1*>(obj);
  if (histo) {

	   for(int i=0; i<(int)axis.size(); i++)
	   {
		for(int j=0; j<(int)axis[i].size()-1; j++)
		{	  
		  histo->GetXaxis()->SetRangeUser(axis[i][j],axis[i][j+1]);
		  mStats.entries[j] = histo->GetEntries();
		  mStats.stddev[j] = histo->GetStdDev();
		  mStats.mean[j] = histo->GetMean();

		  printf("Mean from %f to %f: %f \n", axis[i][j],axis[i][j+1],mStats.mean[j]);

		}
	   }

   } // if (histo)
  } // if( 1 == (int)axis.size()) 


} 

} // namespace o2::quality_control_modules::common
