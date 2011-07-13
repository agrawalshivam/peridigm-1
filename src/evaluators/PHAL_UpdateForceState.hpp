/*! \file PHAL_UpdateForceState.hpp */

//@HEADER
// ************************************************************************
//
//                             Peridigm
//                 Copyright (2011) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions?
// David J. Littlewood   djlittl@sandia.gov
// John A. Mitchell      jamitch@sandia.gov
// Michael L. Parks      mlparks@sandia.gov
// Stewart A. Silling    sasilli@sandia.gov
//
// ************************************************************************
//@HEADER
#ifndef PHAL_UPDATEFORCESTATE_HPP
#define PHAL_UPDATEFORCESTATE_HPP

#include <vector>

#include <Phalanx_ConfigDefs.hpp>
#include <Phalanx_Evaluator_WithBaseImpl.hpp>
#include <Phalanx_Evaluator_Derived.hpp>
#include <Phalanx_MDField.hpp>
#include "PHAL_ParameterEntry.hpp"
#include "PHAL_ParameterGet.hpp"

template<typename EvalT, typename Traits>
class UpdateForceState : public PHX::EvaluatorWithBaseImpl<Traits>,
						 public PHX::EvaluatorDerived<EvalT, Traits>,
						 public PHAL::ParameterGet<EvalT>
{
  typedef typename EvalT::ScalarT ScalarT;

public:

  UpdateForceState(const Teuchos::ParameterList& p);

  void postRegistrationSetup(typename Traits::SetupData d,
							 PHX::FieldManager<Traits>& vm);

  void evaluateFields(typename Traits::EvalData d);

  // getValue method is pure virtual in PHAL::ParameterGet
  virtual ScalarT & getValue(const std::string &n) {
	return parameter_accessible_via_getValue;
  }

private:

  ScalarT parameter_accessible_via_getValue;
  bool m_verbose;
 
  Teuchos::RCP<PHX::FieldTag> update_force_state_field_tag;

  std::size_t m_num_pt;
};

#include "PHAL_UpdateForceState_Def.hpp"

#endif // PHAL_UPDATEFORCESTATE_HPP
