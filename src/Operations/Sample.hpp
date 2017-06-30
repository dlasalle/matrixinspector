/**
 * @file Sample.hpp
 * @brief The Sample class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_OPERATIONS_SAMPLE_HPP
#define MATRIXINSPECTOR_OPERATIONS_SAMPLE_HPP




#include "Data/Matrix.hpp"




namespace MatrixInspector
{


class Sample
{
  public:
    static void random(
        Matrix * matrix,
        dim_type numRows,
        dim_type numCols,
        bool symmetric = false,
        double * progress = nullptr,
        double scale = 1.0f);


    static void thresholdRows(
        Matrix * matrix,
        dim_type minRowSize,
        dim_type maxRowSize,
        double * progress = nullptr,
        double scale = 1.0f);


    static void thresholdColumns(
        Matrix * matrix,
        dim_type minColSize,
        dim_type maxColSize,
        double * progress = nullptr,
        double scale = 1.0f);




};




}




#endif
