/**
 * @file Sample.cpp
 * @brief Implementation of the Sample class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <vector>
#include <algorithm>
#include "Sample.hpp"
#include "Operations/Stats.hpp"
#include "Data/CSRMatrix.hpp"
#include "Utility/Random.hpp"
#include "Utility/Linear.hpp"
#include "Utility/Debug.hpp"




namespace MatrixInspector
{


/******************************************************************************
* PUBLIC STATIC FUNCTIONS *****************************************************
******************************************************************************/


void Sample::random(
    Matrix * const matrix,
    dim_type const numSampleRows,
    dim_type const numSampleCols,
    bool const symmetric,
    double * const progress,
    double const scale)
{
  dim_type const numRows = matrix->getNumRows();
  dim_type const numCols = matrix->getNumColumns();

  ASSERT_LESSEQUAL(numSampleRows,numRows);
  ASSERT_LESSEQUAL(numSampleCols,numCols);

  std::vector<dim_type> rows;
  std::vector<dim_type> cols;

  dim_type const * rowPtr = nullptr;
  dim_type const * colPtr = nullptr;

  if (symmetric) {
    ASSERT_EQUAL(numSampleRows,numSampleCols);

    // make sure the matrix is square
    if (!matrix->isSquare()) {
      throw std::runtime_error("Cannot do symmetric sampling on " \
          "non-square matrix");
    }

    rows.resize(numRows);

    rowPtr = rows.data();
    colPtr = rows.data();

    Linear::step(rows.data(),numRows);

    if (progress != nullptr) {
      *progress += scale*0.1;
    }

    Random::sample(rows.data(),numRows,numSampleRows);

    std::sort(rows.data(),rows.data()+numSampleRows);

    if (progress != nullptr) {
      *progress += scale*0.1;
    }
  } else {
    rows.resize(numRows);
    cols.resize(numCols);

    rowPtr = rows.data();
    colPtr = cols.data();

    Linear::step(rows.data(),numRows);
    Linear::step(cols.data(),numCols);

    if (progress != nullptr) {
      *progress += scale*0.1;
    }

    Random::sample(rows.data(),numRows,numSampleRows);
    Random::sample(cols.data(),numCols,numSampleCols);

    std::sort(rows.data(),rows.data()+numSampleRows);
    std::sort(cols.data(),cols.data()+numSampleCols);

    if (progress != nullptr) {
      *progress += scale*0.1;
    }
  }

  matrix->reduce(rowPtr,numSampleRows,colPtr,numSampleCols,progress,scale*0.8);
}


void Sample::thresholdRows(
    Matrix * const matrix,
    dim_type const minRowSize,
    dim_type const maxRowSize,
    double * const progress,
    double const scale)
{
  CSRMatrix * const csr = dynamic_cast<CSRMatrix*>(matrix);

  ASSERT_NOTNULL(csr);

  dim_type const numRows = csr->getNumRows();
  dim_type const numCols = csr->getNumColumns();

  std::vector<dim_type> sampleRows;
  sampleRows.reserve(numRows);

  index_type const * const offsets = csr->getOffsets();

  dim_type const interval = numCols > 10 ? numRows / 10 : 1; 
  double const increment = scale*0.01;

  for (dim_type row = 0; row < numRows; ++row) {
    dim_type size = offsets[row+1] - offsets[row];
    if (size >= minRowSize && size <= maxRowSize) {
      sampleRows.emplace_back(row);
    }

    if (progress != nullptr && row % interval == 0) {
      *progress += increment;
    }
  }

  csr->reduce(sampleRows.data(),sampleRows.size(),nullptr,NULL_DIM,progress, \
      scale*0.9);
}


void Sample::thresholdColumns(
    Matrix * const matrix,
    dim_type const minColSize,
    dim_type const maxColSize,
    double * const progress,
    double const scale)
{
  CSRMatrix * const csr = dynamic_cast<CSRMatrix*>(matrix);

  ASSERT_NOTNULL(csr);

  dim_type const numRows = matrix->getNumRows();
  dim_type const numCols = matrix->getNumColumns();

  std::vector<dim_type> sampleCols;
  sampleCols.reserve(numCols);

  // count columns
  std::vector<dim_type> colCounts(numCols,0);
  Stats::countColumnNonZeros(csr,colCounts.data());

  if (progress != nullptr) {
    *progress += scale*0.1;
  }

  dim_type const interval = numCols > 5 ? numRows / 5 : 1; 
  double const increment = scale*0.01;

  // add only columns in range
  for (dim_type col = 0; col < numCols; ++col) {
    if (colCounts[col] >= minColSize && colCounts[col] <= maxColSize) {
      sampleCols.emplace_back(col);
    }

    if (progress != nullptr && col % interval == 0) {
      *progress += increment;
    }
  }

  csr->reduce(nullptr,NULL_DIM,sampleCols.data(),sampleCols.size(),progress, \
      scale*0.85);
}




}
