/**
 * @file CollisionTest.cpp
 * @brief Unit tests for the Collision class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include "Test/UnitTest.hpp"
#include "Operations/Reorder.hpp"
#include "Data/CSRMatrix.hpp"




using namespace MatrixInspector;




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

  // sort ascending
  Reorder::density(&mat,true,false,false);

  // check row widths
  for (dim_type row = 1; row < mat.getNumRows(); ++row) {
    const dim_type prevWidth = offsets[row] - offsets[row-1];
    const dim_type width = offsets[row+1] - offsets[row];
    testLessThanOrEqual(prevWidth,width);
  }

  // check columns
  testEquals(columns[0],1);
  testEquals(columns[1],2);
  testEquals(columns[2],0);
  testEquals(columns[3],3);
  testEquals(columns[4],0);
  testEquals(columns[5],1);

  // check values
  testEquals(values[0],1.0);
  testEquals(values[1],6.0);
  testEquals(values[2],2.0);
  testEquals(values[3],3.0);
  testEquals(values[4],4.0);
  testEquals(values[5],5.0);
}



}


