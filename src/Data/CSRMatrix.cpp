/**
 * @file CSRMatrix.cpp
 * @brief Implementation of the CSRMatrix class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include "CSRMatrix.hpp"
#include "Utility/Debug.hpp"
#include "Utility/PrefixSum.hpp"
#include <limits>




namespace MatrixInspector
{


/******************************************************************************
* CONSTANTS *******************************************************************
******************************************************************************/


namespace
{

double const INCREMENT = 0.01;
value_type const EPSILON = std::numeric_limits<value_type>::epsilon();

}




/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/

CSRMatrix::CSRMatrix(
    dim_type const numRows,
    dim_type const numCols,
    index_type const numNonZeros) :
  Matrix(numRows,numCols),
  m_numNonZeros(numNonZeros),
  m_offsets(numRows+1),
  m_columns(numNonZeros),
  m_values(numNonZeros)
{
  // do nothing
}


CSRMatrix::~CSRMatrix()
{
  // do nothing
}




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


index_type CSRMatrix::getNumNonZeros() const
{
  return m_numNonZeros;
}


index_type const * CSRMatrix::getOffsets() const
{
  return m_offsets.data();
}


index_type * CSRMatrix::getOffsets()
{
  return m_offsets.data();
}


dim_type const * CSRMatrix::getColumns() const
{
  return m_columns.data();
}


dim_type * CSRMatrix::getColumns()
{
  return m_columns.data();
}


value_type const * CSRMatrix::getValues() const
{
  return m_values.data();
}


value_type * CSRMatrix::getValues()
{
  return m_values.data();
}


void CSRMatrix::transpose(
    double * const progress,
    double const scale)
{
  dim_type const numRows = getNumRows();
  dim_type const numCols = getNumColumns();

  if (isSymmetric() || numRows == 0 || numCols == 0) {
    // nothing to do
    if (progress != nullptr) {
      *progress += scale*1.0;
    }
  } else {
    assert(m_offsets.size() == numRows+1);
    std::vector<index_type> const oldOffsets(m_offsets); 
    std::vector<dim_type> const oldColumns(m_columns);
    std::vector<value_type> const oldValues(m_values);

    // determine rows per percent
    dim_type interval = numRows > 30 ? numRows / 30 : 1; 

    // count new rows
    m_offsets.assign(numCols+1,0);
    for (dim_type row = 0; row < numRows; ++row) {
      for (index_type nz = oldOffsets[row]; nz < oldOffsets[row+1]; ++nz) {
        assert(oldColumns[nz]+1 < m_offsets.size());
        ++m_offsets[oldColumns[nz]+1];
      }
      if (progress != nullptr && row % interval == 0) {
        *progress += scale*INCREMENT;
      }
    }

    // take prefix sum
    assert(m_offsets.size() > 1);
    assert(m_offsets.size() == numCols+1);
    PrefixSum::exclusive(m_offsets.data()+1,numCols);

    // determine rows per percent
    interval = numRows > 70 ? numRows / 70 : 1; 

    // copy values and columns indexes
    for (dim_type row = 0; row < numRows; ++row) {
      for (index_type nz = oldOffsets[row]; nz < oldOffsets[row+1]; ++nz) {
        index_type idx = m_offsets[oldColumns[nz]+1]++;
        assert(idx < m_columns.size());
        m_columns[idx] = row;
        m_values[idx] = oldValues[nz];
      }
      if (progress != nullptr && row % interval == 0) {
        *progress += scale*INCREMENT;
      }
    }

    assert(m_offsets[numCols] == oldOffsets[numRows]);
  }

  setNumColumns(numRows);
  setNumRows(numCols);

  invalidateStats();
}


void CSRMatrix::reorder(
    dim_type const * const rowPerm,
    dim_type const * const colPerm,
    double * const progress,
    double const scale)
{
  dim_type const numRows = getNumRows();
  dim_type const numCols = getNumColumns();

  assert(m_offsets.size() == numRows+1);

  if (rowPerm) {
    std::vector<index_type> const oldOffsets(m_offsets); 
    std::vector<dim_type> const oldColumns(m_columns);
    std::vector<value_type> const oldValues(m_values);

    // perform a row and possibly a column permutation
    m_offsets[0] = 0;
    for (dim_type row = 0; row < numRows; ++row) {
      dim_type const v = rowPerm[row];
      ASSERT_LESS(v,numRows);
      m_offsets[row+1] = m_offsets[row] + (oldOffsets[v+1] - oldOffsets[v]);
    }

    if (progress != nullptr) {
      *progress += scale*0.1;
    }
    
    index_type newIdx = 0;
    if (colPerm) {
      std::vector<dim_type> rename(numCols);
      // reverse the colperm
      for (dim_type col = 0; col < numCols; ++col) {
        dim_type const v = colPerm[col];
        ASSERT_LESS(v,numCols);
        rename[v] = col;
      }

      if (progress != nullptr) {
        *progress += scale*0.1;
      }

      // determine rows per percent
      dim_type const interval = numRows > 80 ? numRows / 80 : 1; 

      // apply the column perm while filling rowindex and rowvalue arrays
      for (dim_type row = 0; row < numRows; ++row) {
        dim_type const v = rowPerm[row];
        ASSERT_LESS(v,numRows);
        for (index_type idx = oldOffsets[v]; idx < oldOffsets[v+1]; ++idx) {
          m_columns[newIdx] = rename[oldColumns[idx]];
          m_values[newIdx] = oldValues[idx];
          ++newIdx;
        }
        if (progress != nullptr && row % interval == 0) {
          *progress += scale*INCREMENT;
        }
      }
    } else {
      // determine rows per percent
      dim_type const interval = numRows > 90 ? numRows / 90 : 1; 

      // fill rowindex and rowvalue arrays
      for (dim_type row = 0; row < numRows; ++row) {
        dim_type const v = rowPerm[row];
        ASSERT_LESS(v,numRows);
        for (index_type idx = oldOffsets[v]; idx < oldOffsets[v+1]; ++idx) {
          m_columns[newIdx] = oldColumns[idx];
          m_values[newIdx] = oldValues[idx];
          ++newIdx;
        }
        if (progress != nullptr && row % interval == 0) {
          *progress += scale*INCREMENT;
        }
      }
    }
  } else if (colPerm) {
    // perform only a column permutation
    std::vector<dim_type> const oldColumns(m_columns);
    std::vector<dim_type> rename(numCols);

    dim_type interval = numCols > 10 ? numRows / 10 : 1; 

    // reverse the colperm
    for (dim_type col = 0; col < numCols; ++col) {
      dim_type const v = colPerm[col];
      ASSERT_LESS(v,numCols);
      rename[v] = col;

      if (progress != nullptr && col % interval == 0) {
        *progress += scale*INCREMENT;
      }
    }

    // determine rows per percent
    interval = numRows > 90 ? numRows / 90 : 1; 

    // apply the colperm while filling rowindex and rowvalue arrays
    for (dim_type row = 0; row < numRows; ++row) {
      for (index_type idx = m_offsets[row]; idx < m_offsets[row+1]; ++idx) {
        m_columns[idx] = rename[oldColumns[idx]];
      }
      if (progress != nullptr && row % interval == 0) {
        *progress += scale*INCREMENT;
      }
    }
  }

  invalidateStats();
}


void CSRMatrix::reduce(
    dim_type const * const rows,
    dim_type const numSampleRows,
    dim_type const * const cols,
    dim_type const numSampleCols,
    double * const progress,
    double const scale)
{
  dim_type const numRows = getNumRows();
  dim_type const numCols = getNumColumns();

  // copy matrix
  std::vector<index_type> const oldOffsets(m_offsets); 
  std::vector<dim_type> const oldColumns(m_columns);
  std::vector<value_type> const oldValues(m_values);

  dim_type newRows;
  if (rows != nullptr) {
    m_offsets.resize(numSampleRows+1);
    newRows = numSampleRows;
  } else {
    newRows = numRows;
  }

  // set mapping for columns
  dim_type newCols;
  std::vector<dim_type> colMap(numCols,NULL_DIM);
  if (cols != nullptr) {
    for (dim_type colIdx = 0; colIdx < numSampleCols; ++colIdx) {
      colMap[cols[colIdx]] = colIdx;
    }
    newCols = numSampleCols;
  } else {
    newCols = numCols;
  }

  dim_type const interval = numCols > 100 ? numRows / 100 : 1; 
  double const increment = scale*INCREMENT;

  // build new matrix
  index_type nnz = 0;
  dim_type sampleRowIdx = 0;
  for (dim_type row = 0; row < numRows; ++row) {
    if (rows == nullptr || row == rows[sampleRowIdx]) {
      for (index_type colIdx = oldOffsets[row]; colIdx < oldOffsets[row+1]; \
          ++colIdx) {
        dim_type const col = oldColumns[colIdx];
        if (cols != nullptr) {
          if (colMap[col] != NULL_DIM) {
            m_columns[nnz] = colMap[col];
            m_values[nnz] = oldValues[colIdx];
            ++nnz;
          }
        } else {
          m_columns[nnz] = col;
          m_values[nnz] = oldValues[colIdx];
          ++nnz;
        }
      }
      m_offsets[++sampleRowIdx] = nnz;
    }

    if (progress != nullptr && row % interval == 0) {
      *progress += increment;
    }
  }
  ASSERT_EQUAL(sampleRowIdx,newRows);

  // set new dimensions
  setNumRows(newRows);
  setNumColumns(newCols);
  updateNumNonZeros();

  invalidateStats();
}


void CSRMatrix::computeSymmetry(
    double * const progress,
    double const scale)
{
  if (!isSquare()) {
    // easy call
    setSymmetry(false);
  } else {
    dim_type const numRows = getNumRows();

    // this method only works for ordered columns
    #if 0
    std::vector<index_type> steps(m_offsets);

    for (dim_type row = 0; row < numRows; ++row) {
      for (index_type idx = m_offsets[row]; idx < m_offsets[row+1]; ++idx) {
        dim_type const col = m_columns[idx];
        value_type const val = m_values[idx];
        index_type const stepIdx = steps[col];
        if (m_columns[stepIdx] != row && m_values[stepIdx] != 0) {

          std::cout << "missing value at: " << row << "," << col << std::endl;

          // missing non-zero
          setSymmetry(false);
          return;
        } else if (m_values[stepIdx] != val) {

          std::cout << "mismatch values at: " << row << "," << col << \
              " : " << val << "," << m_values[stepIdx] << std::endl;

          // not matching values 
          setSymmetry(false);
          return;
        }
        ++steps[col];
      }
    }
    #else

    // determine rows per percent
    dim_type interval = numRows > 30 ? numRows / 30 : 1; 

    for (dim_type row = 0; row < numRows; ++row) {
      for (index_type idx = m_offsets[row]; idx < m_offsets[row+1]; ++idx) {
        dim_type const col = m_columns[idx];
        value_type const val = m_values[idx];
        index_type idx2;
        for (idx2 = m_offsets[col]; idx < m_offsets[col+1]; \
            ++idx2) {
          if (m_columns[idx2] == row) {
            break;
          }
        }
        value_type const tolerance = std::max(EPSILON, \
            static_cast<value_type>(val*1e-8));
        if (idx2 == m_offsets[col+1] || \
            std::abs(m_values[idx2] - val) > tolerance) {
          // not matching values 
          setSymmetry(false);
          if (progress != nullptr && row % interval == 0) {
            *progress += ((numRows - row)/30) * scale * INCREMENT;
          }
          return;
        }
      }

      if (progress != nullptr && row % interval == 0) {
        *progress += scale*INCREMENT;
      }
    }
    #endif

    // we made it this far -- must be symmetric
    setSymmetry(true);
  }
}



/******************************************************************************
* PROTECTED FUNCTIONS *********************************************************
******************************************************************************/


void CSRMatrix::updateNumNonZeros()
{
  ASSERT_EQUAL(getNumRows()+1,m_offsets.size());
  m_numNonZeros = m_offsets[getNumRows()];
}


}

