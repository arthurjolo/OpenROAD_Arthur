// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2019-2025, The OpenROAD Authors

#pragma once

#include <cmath>
#include <limits>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Clock.h"
#include "CtsOptions.h"
#include "TreeBuilder.h"
#include "Util.h"

#include "odb/db.h"

namespace utl {
class Logger;
}  // namespace utl

namespace sta {
class dbSta;
class dbNetwork;
class LibertyCell;
class Vertex;
class Graph;
}  // namespace sta

namespace cts {

using utl::Logger;

struct GraphNode
{
  GraphNode(int id,
       std::string name,
       int parentId)
    : id(id),
      name(name),
      parentId(parentId)
    {
    }

  int id;
  std::string name;
  int parentId;
  std::vector<int> childrenIds;
  float delay = 0.0;
  float nBuffInsert = 0;
};

class LatenciesBalancer
{
 public:
 LatenciesBalancer(TreeBuilder* root,
                const CtsOptions* options,
                Logger* logger,
                odb::dbDatabase* db,
                rsz::Resizer* resizer,
                sta::dbNetwork* network,
                sta::dbSta* sta,
                double scalingUnit)
      : root_(root),
        options_(options),
        logger_(logger),
        db_(db),
        resizer_(resizer),
        network_(network),
        openSta_(sta),
        wireSegmentUnit_(scalingUnit)
  {
  }

  void run();

 private:
  void initSta();
  void findAllBuilders(TreeBuilder* builder);
  void expandBuilderGraph(TreeBuilder* builder);
  int getNodeIdByName(std::string name);
  odb::dbITerm* getFirstInput(odb::dbInst* inst) const;
  float getVertexClkArrival(sta::Vertex* sinkVertex,
                                     odb::dbNet* topNet,
                                     odb::dbITerm* iterm);
  float computeAveSinkArrivals(TreeBuilder* builder);
  void computeSinkArrivalRecur(odb::dbNet* topClokcNet,
                                        odb::dbITerm* iterm,
                                        float& sumArrivals,
                                        unsigned& numSinks);
  void computeLeafsNumBufferToInsert(int nodeId);
  bool propagateClock(odb::dbITerm* input);
  bool isSink(odb::dbITerm* iterm);

  TreeBuilder* root_ = nullptr;
  const CtsOptions* options_ = nullptr;
  Logger* logger_ = nullptr;
  odb::dbDatabase* db_ = nullptr;
  rsz::Resizer* resizer_ = nullptr;
  sta::dbNetwork* network_ = nullptr;
  sta::dbSta* openSta_ = nullptr;
  sta::Graph* timingGraph_ = nullptr;
  double wireSegmentUnit_;
  float worseDelay_ = std::numeric_limits<float>::min();
  std::vector<GraphNode> graph_;
};

}  // namespace cts
