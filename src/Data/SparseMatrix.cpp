/**
* @file SparseMatrix.cpp
* @brief Implementation of the SparseMatrix class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-04-06
*/




#include "SparseMatrix.hpp"
#include <stdexcept>




namespace MatrixInspector
{


/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/


SparseMatrix::SparseMatrix(
    dim_type const numRows,
    dim_type const numCols,
    index_type const numNonZeros) :
  Matrix(numRows,numCols),
  m_numNonZeros(numNonZeros),
  m_structuralSymmetry(false),
  m_structuralSymmetrySet(false)
{
  // do nothing
}


SparseMatrix::~SparseMatrix()
{
  // do nothing
}



/******************************************************************************
* PUBLIC METHODS **************************************************************
******************************************************************************/

index_type SparseMatrix::getNumNonZeros() const
{
  return m_numNonZeros;
}

bool SparseMatrix::isStructurallySymmetric() const
{
  if (!m_structuralSymmetrySet) {
    throw std::runtime_error("Structural symmetry has not been set.");
  }

  return m_structuralSymmetry;
}

/******************************************************************************
* PROTECTED METHODS ***********************************************************
******************************************************************************/

void SparseMatrix::setNumNonZeros(
    index_type const nnz)
{
  m_numNonZeros = nnz;
}

void SparseMatrix::setStructuralSymmetry(
    bool const sym)
{
  m_structuralSymmetry = sym;
  m_structuralSymmetrySet = true;
}

}
