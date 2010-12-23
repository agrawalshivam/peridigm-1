/*! \file Peridigm.hpp */

// ***********************************************************************
//
//                             Peridigm
//                 Copyright (2009) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// 
// Questions? 
// David J. Littlewood   djlittl@sandia.gov 
// John A. Mitchell      jamitch@sandia.gov
// Michael L. Parks      mlparks@sandia.gov
// Stewart A. Silling    sasilli@sandia.gov
//
// ***********************************************************************

#ifndef PERIDIGM_HPP
#define PERIDIGM_HPP

#include <vector>

#include <Epetra_MpiComm.h>
#include <Epetra_SerialComm.h>
#include <Teuchos_FancyOStream.hpp>
#include <Teuchos_ParameterList.hpp>
#include <Teuchos_RCP.hpp>

#include "contact/Peridigm_ContactModel.hpp"
#include "materials/Peridigm_Material.hpp"
#include "Peridigm_AbstractDiscretization.hpp"
#include "Peridigm_ModelEvaluator.hpp"
#include "Peridigm_DataManager.hpp"
#include "Peridigm_OutputManager.hpp"

namespace PeridigmNS {

  class Peridigm {

  public:

    //! Constructor
    Peridigm(const Teuchos::RCP<const Epetra_Comm>& comm,
             const Teuchos::RCP<Teuchos::ParameterList>& params);

    //! Destructor
    ~Peridigm(){};

    //! Instantiate material objects
    void instantiateMaterials();

    //! Initialize material objects
    void initializeMaterials();

    //! Initialize discretization and maps
    void initializeDiscretization(Teuchos::RCP<AbstractDiscretization> peridigmDisc);

    //! Apply boundary conditions
    void applyInitialVelocities();

    //! Initialize contact
    void initializeContact();

    //! Initialize the workset
    void initializeWorkset();

    //! Initialize the output manager
    void initializeOutputManager();

    //! Main routine to drive problem solution
    void execute();

    //! Rebalance the mesh
    void rebalance();

    //! Update contact neighborlist; do load rebalance
    void updateContactNeighborList();

    //! Accessor for three-dimensional map
    Teuchos::RCP<const Epetra_BlockMap> getThreeDimensionalMap() { return threeDimensionalMap; }

    //! Accessor for main solver-level vectors
    Teuchos::RCP<const Epetra_Vector> getX() { return x; }
    Teuchos::RCP<const Epetra_Vector> getU() { return u; }
    Teuchos::RCP<const Epetra_Vector> getY() { return y; }
    Teuchos::RCP<const Epetra_Vector> getV() { return v; }
    Teuchos::RCP<const Epetra_Vector> getA() { return a; }
    Teuchos::RCP<const Epetra_Vector> getForce() { return force; }

  private:

    //! Parameterlist of entire input deck
    Teuchos::RCP<Teuchos::ParameterList> peridigmParams;

    //! Epetra communicator established by Peridigm_Factory
    Teuchos::RCP<const Epetra_Comm> peridigmComm;

    //! Output stream
    Teuchos::RCP<Teuchos::FancyOStream> out;

    //! Maps for scalar, vector, and bond data
    Teuchos::RCP<const Epetra_BlockMap> oneDimensionalMap;
    Teuchos::RCP<const Epetra_BlockMap> oneDimensionalOverlapMap;
    Teuchos::RCP<const Epetra_BlockMap> threeDimensionalMap;
    Teuchos::RCP<const Epetra_BlockMap> threeDimensionalOverlapMap;
    Teuchos::RCP<const Epetra_BlockMap> bondMap;

    //! Importers and exporters from global to overlapped vectors
    Teuchos::RCP<const Epetra_Import> oneDimensionalMapToOneDimensionalOverlapMapImporter;
    Teuchos::RCP<const Epetra_Import> threeDimensionalMapToThreeDimensionalOverlapMapImporter;

    //! Contact flag
    bool computeContact;

    //! Contact search radius
    double contactSearchRadius;

    //! Contact search frequency
    int contactSearchFrequency;

    //! Material models
    //! \todo Use Teuchos::ArrayRCP to store materials?
    Teuchos::RCP< std::vector< Teuchos::RCP<const PeridigmNS::Material> > > materials;

    //! Contact models
    Teuchos::RCP< std::vector<Teuchos::RCP<const PeridigmNS::ContactModel> > > contactModels;

    //! Data manager
    Teuchos::RCP<PeridigmNS::DataManager> dataManager;

    //! Global vector for initial positions
    Teuchos::RCP<Epetra_Vector> x;

    //! Global vector for displacement
    Teuchos::RCP<Epetra_Vector> u;

    //! Global vector for current position
    Teuchos::RCP<Epetra_Vector> y;

    //! Global vector for velocity
    Teuchos::RCP<Epetra_Vector> v;

    //! Global vector for acceleration
    Teuchos::RCP<Epetra_Vector> a;

    //! Global vector for force
    Teuchos::RCP<Epetra_Vector> force;

    //! Force due to contact (vector includes ghosted dof)
    Teuchos::RCP<Epetra_Vector> contactForceOverlap;

    //! List of neighbors for all locally-owned nodes
    Teuchos::RCP<PeridigmNS::NeighborhoodData> neighborhoodData;

    //! List of potential contact neighbors for all locally-owned nodes
    Teuchos::RCP<PeridigmNS::NeighborhoodData> contactNeighborhoodData;

    //! Workset that is passed to the modelEvaluator
    Teuchos::RCP<PHAL::Workset> workset;

    //! The peridigm model evaluator
    Teuchos::RCP<PeridigmNS::ModelEvaluator> modelEvaluator;

    //! The peridigm output manager
    Teuchos::RCP<PeridigmNS::OutputManager> outputManager;
    //! Description of force state data used by output manager
    Teuchos::RCP<Teuchos::ParameterList> forceStateDesc;

  };
}

#endif // PERIDIGM_HPP
