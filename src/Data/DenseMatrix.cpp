/**
 * @file DenseMatrix.cpp
 * @brief Implementation of the DenseMatrix class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include "DenseMatrix.hpp"




namespace MatrixInpsector
{


/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/


DenseMatrix::DenseMatrix(
    dim_type const numRows,
    dim_type const numCols) :
  Matrix(numRows,numCols,false),
  m_values(numRows*numCols,0)
{
  // do nothing
}


DenseMatrix::~DenseMatrix()
{
  // do nothing
}




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


void DenseMatrix::transpose(
    double * const progress,
    double scale)
{
  dim_type const numRows = getNumRows();
  dim_type const numCols = getNumColumns();

  if (isSymmetric() || (numRows == 1 || numCols == 1)) {
    // do nothing for these cases
  } else if (isSquare()) {
    dim_type const interval = numRows > 100 ? numRows / 100 : 1;
    double const increment = scale * 1.0 / 100.0;

    // we can transpose square matrices in place
    for (dim_type row = 0; row < numRows; ++row) {
      for (dim_type col = 0; col < row; ++col) {
        index_type const inIdx = (row*numCols) + col;
        index_type const outIdx = (col*numRows) + row;
        value_type const val = m_values[inIdx];
        m_values[inIdx] = m_values[outIdx];
        m_values[outIdx] = val;
      }

      if (progress != nullptr && row % interval == 0) {
        *progress += increment;
      }
    }
  } else {
    // transpose rectangular matrix this is a bit tricky -- chase down cycles
		std::vector<bool> visited(numRows*numCols,false);
		index_type const end = (numRows*numCols) - 1;
    // first and last positions don't move
		for (index_type cycle = 1; cycle < end; ++ cycle) {
			if (visited[cycle]) {
					continue;
			}
			index_type current = cycle;
			do  {
				current = current == end ? end : (numRows*current) % end;
        value_type const val = m_values[current];
        m_values[current] = m_values[cycle];
        m_values[cycle] = val;
				visited[current] = true;
			} while (current != cycle);
    }

    // not sure how to incrementally progress this yet
    if (progress != nullptr) {
      *progress += scale;
    }
  }

  // update dimensions
  setNumRows(numCols);
  setNumColumns(numRows);
}


void DenseMatrix::reorder(
    dim_type const * rowPerm,
    dim_type const * colPerm,
    double * const progress,
    double scale)
{
  dim_type const numRows = getNumRows();
  dim_type const numCols = getNumColumns();

  // create copy of matrix 
  std::vector<value_type> const oldValues(m_values);

  dim_type const interval = numRows > 100 ? numRows / 100 : 1;
  double const increment = scale * 1.0 / 100.0;

  // write entries
  for (dim_type row = 0; row < numRows; ++row) {
    dim_type const dstRow = rowPerm != nullptr ? rowPerm[row] : row;
    for (dim_type col = 0; col < numCols; ++col) {
      dim_type const dstCol = colPerm != nullptr ? colPerm[col] : col;
      index_type const srcIdx = (row*numCols) + col;
      index_type const dstIdx = (dstRow*numCols) + dstCol;
      m_values[dstIdx] = oldValues[srcIdx];
    }

    if (progress != nullptr && row % interval == 0) {
      *progress += increment;
    }
  }
}


void DenseMatrix::reduce(
    dim_type const * const rows,
    dim_type const numSampleRows,
    dim_type const * const cols,
    dim_type const numSampleCols,
    double * progress,
    double scale)
{
  dim_type const numRows = getNumRows();
  dim_type const numCols = getNumColumns();

  std::vector<value_type> oldValues(m_values);

  m_values.resize(numSampleRows*numSampleCols);

  dim_type const interval = numRows > 100 ? numRows / 100 : 1;
  double const increment = scale * 1.0 / 100.0;

  // copy in rows and columns
  dim_type rowIdx = 0;
  for (dim_type row = 0; row < numRows; ++row) {
    if (row == rows[rowIdx]) {
      dim_type colIdx = 0;
      for (dim_type col = 0; col < numCols; ++col) {
        if (col == cols[colIdx]) {
          index_type const globalIdx = (row*numCols)+row;
          index_type const sampleIdx = (rowIdx*numSampleCols)+colIdx;
          m_values[sampleIdx] = oldValues[globalIdx];
          ++colIdx;
        }
      }
      ++rowIdx;
    }

    if (progress != nullptr && row % interval == 0) {
      *progress += increment;
    }
  }

  // update dimensions
  setNumRows(numRows);
  setNumColumns(numCols);
}


void DenseMatrix::computeSymmetry()
{
  if (!isSquare()) {
    // easy call
    setSymmetry(false);
  } else {
    dim_type const numRows = getNumRows();
    dim_type const numCols = getNumColumns();

    for (dim_type row = 0; row < numRows; ++row) {
      for (dim_type col = 0; col < row; ++ col) {
        if (m_values[(row*numCols)+col] != m_values[(col*numRows)+row]) {
          // missing non-zero
          setSymmetry(false);
          return;
        }
      }
    }

    setSymmetry(true);
  }
}




}
