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
/// \file   TrendingTaskConfigTPC.cxx
/// \author Based on the work of Piotr Konopka
///

#include "TPC/TrendingTaskConfigTPC.h"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <typeinfo> 
#include <vector>
//#include <boost/foreach.hpp>

//using boost::property::ptree;

namespace o2::quality_control::postprocessing
{

TrendingTaskConfigTPC::TrendingTaskConfigTPC(std::string name, const boost::property_tree::ptree& config)
  : PostProcessingConfig(name, config)
{
  for (const auto& plotConfig : config.get_child("qc.postprocessing." + name + ".plots")) {
    plots.push_back({ plotConfig.second.get<std::string>("name"),
                      plotConfig.second.get<std::string>("title", ""),
                      plotConfig.second.get<std::string>("varexp"),
                      plotConfig.second.get<std::string>("selection", ""),
                      plotConfig.second.get<std::string>("option", ""),
                      plotConfig.second.get<std::string>("graphErrors", "") });
  }
  for (const auto& dataSourceConfig : config.get_child("qc.postprocessing." + name + ".dataSources")) {

  std::vector<std::vector<float>> axisBoundaries; 
  std::vector<float> singleAxis;

    if (const auto& sourceNames = dataSourceConfig.second.get_child_optional("names"); sourceNames.has_value()) {	
      if (const auto& multiAxisValues = dataSourceConfig.second.get_child_optional("axisDivision"); multiAxisValues.has_value()) {
 	
	for(const auto& multiAxisValue : multiAxisValues.value() ) 
	{
		for (const auto& axis : multiAxisValue.second ) 
		{
			singleAxis.push_back(std::stof(axis.second.data()));
		}

		axisBoundaries.push_back(singleAxis);
		singleAxis.clear();
	}
      } 

      for (const auto& sourceName : sourceNames.value()) {
        dataSources.push_back({ dataSourceConfig.second.get<std::string>("type", "repository"),
                                dataSourceConfig.second.get<std::string>("path"),
                                sourceName.second.data(),
                                dataSourceConfig.second.get<std::string>("reductorName"),
				axisBoundaries,
                                dataSourceConfig.second.get<std::string>("moduleName") });
      }

      axisBoundaries.clear();

    } else if (!dataSourceConfig.second.get<std::string>("name").empty()) {
      // "name" : [ "something" ] would return an empty string here
      dataSources.push_back({ dataSourceConfig.second.get<std::string>("type", "repository"),
                              dataSourceConfig.second.get<std::string>("path"),
                              dataSourceConfig.second.get<std::string>("name"),
                              dataSourceConfig.second.get<std::string>("reductorName"),
			      axisBoundaries,
                              dataSourceConfig.second.get<std::string>("moduleName") });
    } else {
      throw std::runtime_error("No 'name' value or a 'names' vector in the path 'qc.postprocessing." + name + ".dataSources'");
    }
  }
}

} // namespace o2::quality_control::postprocessing
