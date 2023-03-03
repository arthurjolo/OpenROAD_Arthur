/*
 * Copyright (c) 2022, The Regents of the University of California
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <functional>
#include <string>
#include <iostream>

#include "boostParser.h"
#include "db.h"
#include "lefLayerPropParser.h"
#include "lefin.h"

namespace odb {

lefTechLayerPitchRuleParser::lefTechLayerPitchRuleParser(lefin* l)
{
  lefin_ = l;
}

void lefTechLayerPitchRuleParser::parse(
    std::string s,
    odb::dbTechLayer* layer)
{
    odb::dbTechLayerPitchRule* rule = odb::dbTechLayerPitchRule::create(layer);

    qi::rule<std::string::iterator, space_type> PITCH
      = ((lit("PITCH") >> double_ >> double_)[boost::bind(
            &odb::lefTechLayerPitchRuleParser::setPitchXY,
            this,
            _1,
            rule,
            &odb::dbTechLayerPitchRule::setPitchX,
            &odb::dbTechLayerPitchRule::setPitchY)]
        | lit("PITCH") >> double_[boost::bind(
            &odb::lefTechLayerPitchRuleParser::setPitch,
            this,
            _1,
            rule,
            &odb::dbTechLayerPitchRule::setPitch)]);

    qi::rule<std::string::iterator, space_type> FIRST_LAST_PTICH
      = (PITCH >> (lit("FIRSTLASTPITCH") >> double_[boost::bind(
            &odb::lefTechLayerPitchRuleParser::setFirstLastPitch,
            this,
            _1,
            rule,
            &odb::dbTechLayerPitchRule::setFirstLastPitch)])
        >> lit(";"));
  auto first = s.begin();
  auto last = s.end();
  bool valid = qi::phrase_parse(first, last, FIRST_LAST_PTICH, space) && first == last;
  if (!valid) {
    lefin_->warning(281,
                  "parse mismatch in layer propery LEF58_Pitch for layer");
    odb::dbTechLayerPitchRule::destroy(rule);
  }
}

void lefTechLayerPitchRuleParser::setFirstLastPitch(double val,
              odb::dbTechLayerPitchRule* rule,
              void (odb::dbTechLayerPitchRule::*func)(int))
{
    (rule->*func)(lefin_->dbdist(val));
}

void lefTechLayerPitchRuleParser::setPitch(double val,
              odb::dbTechLayerPitchRule* rule,
              void (odb::dbTechLayerPitchRule::*func)(int))
{
    (rule->*func)(lefin_->dbdist(val));
}

void lefTechLayerPitchRuleParser::setPitchXY(boost::fusion::vector<double, double>& params,
                odb::dbTechLayerPitchRule* rule,
                void (odb::dbTechLayerPitchRule::*func1)(int),
                void (odb::dbTechLayerPitchRule::*func2)(int))
{
    (rule->*func1)(lefin_->dbdist(at_c<0>(params)));
    (rule->*func2)(lefin_->dbdist(at_c<1>(params)));
}

}  // namespace odb