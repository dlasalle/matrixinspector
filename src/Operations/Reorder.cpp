/**
 * @file Reorder.cpp
 * @brief Implementation of the Reorder class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <vector>
#include "Operations/Reorder.hpp"
#include "Operations/Stats.hpp"
#include "Data/CSRMatrix.hpp"
#include "Utility/Sort.hpp"
#include "Utility/Random.hpp"
#include "Utility/Debug.hpp"




namespace MatrixInspector
{



/******************************************************************************
* HELPER FUNCTIONS ************************************************************
******************************************************************************/

namespace
{

template< typename K>
void reorderViaKeys(
    Matrix * const matrix,
    K const * const rowKeys,
    K const * const colKeys,
    bool const ascending,
    double * const progress,
    double const scale)
{
  dim_type const numRows = matrix->getNumRows();
  dim_type const numCols = matrix->getNumColumns();

  CSRMatrix * const csr = dynamic_cast<CSRMatrix*>(matrix); 

  dim_type const * rowPermPtr = nullptr;
  std::vector<dim_type> rowPerm;

  dim_type const * colPermPtr = nullptr;
  std::vector<dim_type> colPerm;

  if (rowKeys != nullptr) {
    rowPerm.resize(numRows);
    for (dim_type row = 0; row < numRows; ++row) {
      rowPerm[row] = row;
    }
    Sort::keyValue(rowKeys,rowPerm.data(),numRows,ascending);

    rowPermPtr = rowPerm.data();
  }

  if (progress != nullptr) {
    *progress += scale*0.05;
  }

  if (colKeys != nullptr) {
    colPerm.resize(numCols);
    for (dim_type col = 0; col < numCols; ++col) {
      colPerm[col] = col;
    }
    Sort::keyValue(colKeys,colPerm.data(),numCols,ascending);

    colPermPtr = colPerm.data();
  }

  if (progress != nullptr) {
    *progress += scale*0.05;
  }

  csr->reorder(rowPermPtr,colPermPtr,progress,scale*0.9);
}

}


/******************************************************************************
* PUBLIC STATIC FUNCTIONS *****************************************************
******************************************************************************/


void Reorder::random(
    Matrix * const matrix,
    bool const rows,
    bool const columns,
    uint32_t const seed,
    bool const symmetric,
    double * const progress,
    double const scale)
{
  dim_type const numRows = matrix->getNumRows();
  dim_type const numCols = matrix->getNumColumns();

  Random::setSeed(seed);

  dim_type const * permRowPtr = nullptr;
  dim_type const * permColPtr = nullptr;
  std::vector<dim_type> permRow;
  std::vector<dim_type> permCol;

  if (rows && columns && symmetric) {
    ASSERT_EQUAL(numRows,numCols);
    permRow.resize(numRows);
    Random::permutation(permRow.data(),numRows);

    permRowPtr = permRow.data();
    permColPtr = permRow.data();
  } else if (rows && columns) {
    assert(!symmetric);
    permRow.resize(numRows);
    Random::permutation(permRow.data(),numRows);

    permCol.resize(numCols);
    Random::permutation(permCol.data(),numCols);

    permRowPtr = permRow.data();
    permColPtr = permCol.data();
  } else if (rows) {
    assert(!symmetric);
    permRow.resize(numRows);
    Random::permutation(permRow.data(),numRows);

    permRowPtr = permRow.data();
  } else {
    assert(!symmetric);
    assert(columns);
    permCol.resize(numCols);
    Random::permutation(permCol.data(),numCols);

    permColPtr = permCol.data();
  }

  if (progress != nullptr) {
    *progress += scale*0.1;
  }

  matrix->reorder(permRowPtr,permColPtr,progress,scale*0.9);
}


void Reorder::density(
    Matrix * const matrix,
    bool const rows,
    bool const columns,
    bool const descending,
    double * const progress,
    double const scale)
{
  dim_type const numRows = matrix->getNumRows();
  dim_type const numCols = matrix->getNumColumns();

  CSRMatrix * const csr = dynamic_cast<CSRMatrix*>(matrix); 

  dim_type const * rowKeysPtr = nullptr;
  std::vector<dim_type> rowKeys;

  dim_type const * colKeysPtr = nullptr;
  std::vector<dim_type> colKeys;

  if (rows) {
    rowKeys.resize(numRows);
    Stats::countRowNonZeros(csr,rowKeys.data());

    rowKeysPtr = rowKeys.data();
  }

  if (progress != nullptr) {
    *progress += scale*0.05;
  }

  if (columns) {
    colKeys.resize(numCols);
    Stats::countColumnNonZeros(csr,colKeys.data());

    colKeysPtr = colKeys.data();
  }

  if (progress != nullptr) {
    *progress += scale*0.05;
  }

  Reorder::keys(matrix,rowKeysPtr,colKeysPtr,!descending,progress,scale*0.9);
}


void Reorder::keys(
    Matrix * const matrix,
    value_type const * const rowKeys,
    value_type const * const colKeys,
    bool const ascending,
    double * const progress,
    double const scale)
{
  reorderViaKeys(matrix,rowKeys,colKeys,ascending,progress,scale);
}


void Reorder::keys(
    Matrix * const matrix,
    dim_type const * const rowKeys,
    dim_type const * const colKeys,
    bool const ascending,
    double * const progress,
    double const scale)
{
  reorderViaKeys(matrix,rowKeys,colKeys,ascending,progress,scale);
}




}
