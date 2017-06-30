/**
 * @file CSRMatrix.hpp
 * @brief The CSRMatrix class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_CSRMATRIX_HPP
#define MATRIXINSPECTOR_CSRMATRIX_HPP




#include <vector>
#include "Types.hpp"
#include "Matrix.hpp"




namespace MatrixInspector
{


class CSRMatrix :
  public Matrix
{
  public:
    CSRMatrix(
        dim_type numRows,
        dim_type numCols,
        index_type numNonZeros);


    ~CSRMatrix();


    void transpose(
        double * progress,
        double scale) override;


    void reorder(
        dim_type const * rowPerm,
        dim_type const * colPerm,
        double * progress,
        double scale) override;


    /**
     * @brief Reduce the matrix to a subset of its rows and columns.
     *
     * @param rows The set of rows to reduce it to. Must be in ascending order.
     * @param numRows The number of rows in the set.
     * @param cols The set of columns to reduce it to. Must be in ascending
     * order.
     * @param numCols The number of columns in the set.
     * @param progress
     * @param scale
     */
    void reduce(
        dim_type const * rows,
        dim_type numRows,
        dim_type const * cols,
        dim_type numCols,
        double * progress,
        double scale) override;


    void computeSymmetry() override;


    index_type getNumNonZeros() const;


    index_type const * getOffsets() const;


    index_type * getOffsets();


    dim_type const * getColumns() const;


    dim_type * getColumns();


    value_type const * getValues() const;


    value_type * getValues();

  protected:
    void updateNumNonZeros();


  private:
    index_type m_numNonZeros;
    std::vector<index_type> m_offsets;
    std::vector<dim_type> m_columns;
    std::vector<value_type> m_values;
    
};




}




#endif
