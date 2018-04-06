/**
 * @file CSRMatrix.hpp
 * @brief The CSRMatrix class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_CSRMATRIX_HPP
#define MATRIXINSPECTOR_CSRMATRIX_HPP




#include "SparseMatrix.hpp"
#include "Types.hpp"
#include <vector>




namespace MatrixInspector
{


class CSRMatrix :
  public SparseMatrix
{
  public:
    /**
    * @brief Create a new CSR matrix.
    *
    * @param numRows The number of rows.
    * @param numCols The number of columns.
    * @param numNonZeros The number of non-zeros.
    */
    CSRMatrix(
        dim_type numRows,
        dim_type numCols,
        index_type numNonZeros);


    /**
    * @brief Virtual destructor.
    */
    virtual ~CSRMatrix();


    /**
    * @brief Transpose the matrix.
    *
    * @param progress The progress indicator to update.
    * @param scale The fraction of the total progress to be updated.
    */
    void transpose(
        double * progress,
        double scale) override;


    /**
    * @brief Re-order the matrix.
    *
    * @param colPerm The column permutation.
    * @param rowPerm The row permutation.
    * @param progress The progress indicator to update.
    * @param scale The fraction of the total progress to be updated.
    */
    void reorder(
        dim_type const * rowPerm,
        dim_type const * colPerm,
        double * progress,
        double scale) override;


    /**
     * @brief Reduce the size of the matix down to the specified set of rows and
     * columns.
     *
     * @param rows The set of rows to reduce it to. Must be in ascending order.
     * @param numRows The number of rows in the set.
     * @param cols The set of columns to reduce it to. Must be in ascending
     * order.
     * @param numCols The number of columns in the set.
     * @param progress The progress indicator to update.
     * @param scale The fraction of the task to update.
     */
    void reduce(
        dim_type const * rows,
        dim_type numRows,
        dim_type const * cols,
        dim_type numCols,
        double * progress,
        double scale) override;


		/**
		* @brief Determine and set whether the matrix is symmetric.
    *
    * @param progress The progress of the task.
    * @param scale The fraction of the task this operation completes.
    */
    void computeSymmetry(
        double * progress,
        double scale) override;


    /**
    * @brief Get the row offsets.
    *
    * @return The row of offsets.
    */
    index_type const * getOffsets() const;

    /**
    * @brief Get the row offsets.
    *
    * @return The row of offsets.
    */
    index_type * getOffsets();


    /**
    * @brief Get the columns in the martix for each non-zero. 
    *
    * @return The columns.
    */
    dim_type const * getColumns() const;


    /**
    * @brief Get the columns in the martix for each non-zero. 
    *
    * @return The columns.
    */
    dim_type * getColumns();


    /**
    * @brief Get the non-zero values in the matrix.
    *
    * @return The values.
    */
    value_type const * getValues() const;

    /**
    * @brief Get the non-zero values in the matrix.
    *
    * @return The values.
    */
    value_type * getValues();


  protected:
    void updateNumNonZeros();


  private:
    std::vector<index_type> m_offsets;
    std::vector<dim_type> m_columns;
    std::vector<value_type> m_values;
    
};




}




#endif
