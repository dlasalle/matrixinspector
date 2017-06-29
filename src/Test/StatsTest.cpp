/**
 * @file StatsTest.cpp
 * @brief Unit tests for the Collision class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include "Test/UnitTest.hpp"
#include "Operations/Stats.hpp"
#include "Data/CSRMatrix.hpp"




using namespace MatrixInpsector;




namespace Test
{


TEST
{
  CSRMatrix mat(5,4,6);

  index_type * const offsets = mat.getOffsets();
  offsets[0] = 0;
  offsets[1] = 1;
  offsets[2] = 3;
  offsets[3] = 3;
  offsets[4] = 5;
  offsets[5] = 6;

  dim_type * const columns = mat.getColumns();
  columns[0] = 1;
  columns[1] = 0;
  columns[2] = 3;
  columns[3] = 0;
  columns[4] = 1;
  columns[5] = 2;

  value_type * const values = mat.getValues();
  values[0] = 1.0;
  values[1] = 2.0;
  values[2] = 3.0;
  values[3] = 4.0;
  values[4] = 5.0;
  values[5] = 6.0;

  // check rows
  std::vector<dim_type> rowCounts(mat.getNumRows());
  Stats::countRowNonZeros(&mat,rowCounts.data());

  for (dim_type row = 0; row < mat.getNumRows(); ++row) {
    testEquals(rowCounts[row],offsets[row+1]-offsets[row]);
  }

  // check columns
  std::vector<dim_type> colCounts(mat.getNumColumns());
  Stats::countColumnNonZeros(&mat,colCounts.data());

  testEquals(colCounts[0],2);
  testEquals(colCounts[1],2);
  testEquals(colCounts[2],1);
  testEquals(colCounts[3],1);
}



}


