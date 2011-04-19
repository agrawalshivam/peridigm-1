/*! \file PHAL_EvaluateJacobian_Def.hpp */

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

#include <Teuchos_TestForException.hpp>
#include <Phalanx_DataLayout.hpp>

//**********************************************************************
template<typename EvalT, typename Traits>
EvaluateJacobian<EvalT, Traits>::EvaluateJacobian(Teuchos::ParameterList& p) :
  m_verbose(false),
  m_num_pt(0)
{
  if(p.isParameter("Verbose"))
	 m_verbose = p.get<bool>("Verbose");

  Teuchos::RCP<PHX::FieldTag> update_jacobian_state_data_field_tag = 
	Teuchos::rcp(new PHX::Tag<ScalarT>("UpdateJacobianState", p.get< Teuchos::RCP<PHX::DataLayout> >("Dummy Data Layout")));

  evaluate_jacobian_field_tag = 
    Teuchos::rcp(new PHX::Tag<ScalarT>("EvaluateJacobian",p.get< Teuchos::RCP<PHX::DataLayout> >("Dummy Data Layout")));

  this->addEvaluatedField(*evaluate_jacobian_field_tag);

  this->setName("EvaluateJacobian");
}

//**********************************************************************
template<typename EvalT, typename Traits>
void EvaluateJacobian<EvalT, Traits>::postRegistrationSetup(
                      typename Traits::SetupData d,
                      PHX::FieldManager<Traits>& fm)
{
}

//**********************************************************************
template<typename EvalT, typename Traits>
void EvaluateJacobian<EvalT, Traits>::evaluateFields(typename Traits::EvalData cellData)
{
  if(m_verbose)
	cout << "CHECK inside EvaluateJacobian::evaluateFields()\n" << endl;

  const double dt = *cellData.timeStep;
  const int numOwnedPoints = cellData.neighborhoodData->NumOwnedPoints();
  const int* ownedIDs = cellData.neighborhoodData->OwnedIDs();
  const int* neighborhoodList = cellData.neighborhoodData->NeighborhoodList();
  PeridigmNS::DataManager& dataManager = *cellData.dataManager;
  PeridigmNS::SerialMatrix& jacobian = *cellData.jacobian;

  // handling of material models needs work!
  Teuchos::RCP<const PeridigmNS::Material> material = (*cellData.materialModels)[0];

  material->computeJacobian(dt, 
                            numOwnedPoints,
                            ownedIDs,
                            neighborhoodList,
                            dataManager,
                            jacobian);
}
