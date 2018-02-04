/**
 * @file Matrix.cpp
 * @brief Implementation of the Matrix class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <stdexcept>
#include <vector>
#include <algorithm>
#include "Utility/Debug.hpp"
#include "Operations/Stats.hpp"
#include "Matrix.hpp"




namespace MatrixInspector
{



/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/

Matrix::Matrix(
    dim_type const numRows,
    dim_type const numCols) :
  Matrix(numRows, numCols, false)
{
  // do nothing
}


Matrix::Matrix(
    dim_type const numRows,
    dim_type const numCols,
    bool const sym) :
  m_numRows(numRows),
  m_numCols(numCols),
  m_symmetrySet(true),
  m_symmetry(sym),
  m_statsSet(false),
  m_maxRowSize(0),
  m_maxColumnSize(0),
  m_numEmptyRows(0),
  m_numEmptyColumns(0)
{
  // do nothing
}


Matrix::~Matrix()
{
  // do nothing
}


/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


dim_type Matrix::getNumRows() const noexcept
{
  return m_numRows;
}


dim_type Matrix::getNumColumns() const noexcept
{
  return m_numCols;
}


bool Matrix::isSymmetric() const
{
  if (!m_symmetrySet) {
    throw std::runtime_error("Symmetry has not been set.");
  }

  return m_symmetry;
}


bool Matrix::isSymmetrySet() const noexcept
{
  return m_symmetrySet;
}


bool Matrix::isSquare() const noexcept
{
  return m_numRows == m_numCols;
}


bool Matrix::isStatsSet() const noexcept 
{
  return m_statsSet;
}


dim_type Matrix::getMaxRowSize() const
{
  if (!isStatsSet()) {
    throw std::runtime_error("Stats have not been computed yet.");
  }

  return m_maxRowSize;
}


dim_type Matrix::getMaxColumnSize() const
{
  if (!isStatsSet()) {
    throw std::runtime_error("Stats have not been computed yet.");
  }

  return m_maxColumnSize;
}


dim_type Matrix::getNumEmptyRows() const
{
  if (!isStatsSet()) {
    throw std::runtime_error("Stats have not been computed yet.");
  }

  return m_numEmptyRows;
}


dim_type Matrix::getNumEmptyColumns() const
{
  if (!isStatsSet()) {
    throw std::runtime_error("Stats have not been computed yet.");
  }

  return m_numEmptyColumns;
}


void Matrix::computeStats(
    double * const progress,
    double scale)
{
  // set symmetry if neccessary
  if (!isSymmetrySet()) {
    computeSymmetry();
  }

  if (getNumRows() == 0 || getNumColumns() == 0) {
    // handle empty matrix
    m_maxRowSize = 0;
    m_maxColumnSize = 0;
    m_numEmptyRows = getNumRows();
    m_numEmptyColumns = getNumColumns();

    if (progress != nullptr) {
      *progress += scale;
    }
  } else {
    // compute row stats
    {
      std::vector<dim_type> rowCounts(getNumRows());
      Stats::countRowNonZeros(this,rowCounts.data());
      std::sort(rowCounts.begin(),rowCounts.end());

      m_maxRowSize = rowCounts.back();

      index_type i = 0;
      while (i < rowCounts.size() && rowCounts[i] == 0) {
        ++i;
      }
      m_numEmptyRows = i;
    }

    if (progress != nullptr) {
      *progress += scale*0.5;
    }

    // compute col stats
    {
      std::vector<dim_type> colCounts(getNumColumns());
      Stats::countColumnNonZeros(this,colCounts.data());
      std::sort(colCounts.begin(),colCounts.end());

      m_maxColumnSize = colCounts.back();

      index_type i;
      for (i = 0; i < colCounts.size() && colCounts[i] == 0; ++i);
      m_numEmptyColumns = i;
    }

    if (progress != nullptr) {
      *progress += scale*0.5;
    }
  }

  m_statsSet = true;

  assert(isStatsSet());
}


void Matrix::invalidateStats()
{
  m_statsSet = false;

  assert(!isStatsSet());
}


void Matrix::transpose()
{
  transpose(nullptr);
}


void Matrix::transpose(
    double * const progress)
{
  transpose(progress,1.0f);
}


void Matrix::reorder(
    dim_type const * const colPerm,
    dim_type const * const rowPerm)
{
  reorder(colPerm,rowPerm,nullptr);
}


void Matrix::reorder(
    dim_type const * const colPerm,
    dim_type const * const rowPerm,
    double * const progress)
{
  reorder(colPerm,rowPerm,progress,1.0f);
}


void Matrix::computeSymmetry()
{
  computeSymmetry(nullptr, 1.0);
}


void Matrix::computeSymmetry(
    double * const progress)
{
  computeSymmetry(progress, 1.0);
}

/******************************************************************************
* PROTECTED FUNCTIONS *********************************************************
******************************************************************************/


void Matrix::setNumRows(
    dim_type const numRows)
{
  m_numRows = numRows;
}


void Matrix::setNumColumns(
    dim_type const numCols)
{
  m_numCols = numCols;
}


void Matrix::setSymmetry(
    bool const sym)
{
  assert(!sym || isSquare());
  m_symmetry = sym;
  m_symmetrySet = true;
}


void Matrix::unsetSymmetry()
{
  m_symmetrySet = false;
  m_symmetry = false;
}



}
