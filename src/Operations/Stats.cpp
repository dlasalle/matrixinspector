/**
 * @file Stats.cpp
 * @brief Implementation of the stats class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <stdexcept>
#include "Data/CSRMatrix.hpp"
#include "Stats.hpp"




namespace MatrixInpsector
{


/******************************************************************************
* STATIC PUBLIC FUNCTIONS *****************************************************
******************************************************************************/


void Stats::countRowNonZeros(
    Matrix * const matrix,
    dim_type * const counts)
{
  CSRMatrix const * const csr = dynamic_cast<CSRMatrix const *>(matrix);

  if (csr == nullptr) {
    throw std::runtime_error("Cannot perform row count on non-csr matrix.");
  }

  dim_type const numRows = csr->getNumRows();
  index_type const * const offsets = csr->getOffsets();

  for (dim_type row = 0; row < numRows; ++row) {
    counts[row] = offsets[row+1] - offsets[row];
  }
}


void Stats::countColumnNonZeros(
    Matrix * const matrix,
    dim_type * const counts)
{
  dim_type const numRows = matrix->getNumRows();
  dim_type const numCols = matrix->getNumColumns();

  CSRMatrix const * const csr = dynamic_cast<CSRMatrix const *>(matrix);

  if (csr == nullptr) {
    throw std::runtime_error("Cannot perform row count on non-csr matrix.");
  }

  index_type const * const offsets = csr->getOffsets();
  dim_type const * const columns = csr->getColumns();

  // initialize counts
  for (dim_type col = 0; col < numCols; ++col) {
    counts[col] = 0;
  }

  // actually count
  for (dim_type row = 0; row < numRows; ++row) {
    for (index_type idx = offsets[row]; idx < offsets[row+1]; ++idx) {
      ++counts[columns[idx]];
    }
  }
}




}
