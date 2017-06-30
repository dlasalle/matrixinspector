/**
 * @file DenseMatrix.hpp
 * @brief The DenseMatrix class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_DENSEMATRIX_HPP
#define MATRIXINSPECTOR_DENSEMATRIX_HPP




#include <vector>
#include "Types.hpp"
#include "Matrix.hpp"




namespace MatrixInspector
{


class DenseMatrix :
  public Matrix
{
  public:
    DenseMatrix(
        dim_type numRows,
        dim_type numCols);


    ~DenseMatrix();


    void transpose(
        double * progress,
        double scale) override;


    void reorder(
        dim_type const * rowPerm,
        dim_type const * colPerm,
        double * progress,
        double scale) override;


    void reduce(
        dim_type const * rows,
        dim_type numRows,
        dim_type const * cols,
        dim_type numCols,
        double * progress,
        double scale) override;


    void computeSymmetry() override;


  private:
    std::vector<value_type> m_values;




};




}




#endif
