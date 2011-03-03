// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/CBuilder.hpp"
#include "Common/OptionComponent.hpp"

#include "Solver/CTime.hpp"
#include "Solver/Actions/CCriterionTime.hpp"

namespace CF {
namespace Solver {
namespace Actions {

using namespace Common;

ComponentBuilder< CCriterionTime, CCriterion, LibActions > CCriterionTime_Builder;

////////////////////////////////////////////////////////////////////////////////

CCriterionTime::CCriterionTime( const std::string& name  ) :
  CCriterion ( name ),
  m_tolerance(1e-12)
{
  properties()["brief"] = std::string("Time Criterion object");
  std::string description = properties()["description"].value<std::string>()+
    "Returns true if a time is reached\n";
  properties()["description"] = description;
  
  properties().add_option(OptionComponent<CTime>::create("time","Time","Time tracking component",&m_time))
    ->mark_basic()
    ->add_tag("time");
}

////////////////////////////////////////////////////////////////////////////////

CCriterionTime::~CCriterionTime()
{
}

////////////////////////////////////////////////////////////////////////////////

bool CCriterionTime::operator()()
{
  if (m_time.expired()) throw SetupError(FromHere(),"Time option was not set in ["+full_path().path()+"]");
  CTime& t = *m_time.lock();
  
  return ( t.time() + m_tolerance > t.property("end_time").value<Real>() );
}

////////////////////////////////////////////////////////////////////////////////

} // Actions
} // Solver
} // CF