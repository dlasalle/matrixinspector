/**
 * @file Matrix.hpp
 * @brief The Matrix class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_MATRIX_HPP
#define MATRIXINSPECTOR_MATRIX_HPP




#include "Types.hpp"




namespace MatrixInspector
{


class Matrix
{
  public:
    /**
    * @brief Create a new matrix.
    *
    * @param numRows The number of rows.
    * @param numCols The number of columns.
    */
    Matrix(
        dim_type numRows,
        dim_type numCols);


    /**
    * @brief Create a new matrix.
    *
    * @param numRows The number of rows.
    * @param numCols The number of columns.
    * @param symmetry Whether or not the matrix is symmetric.
    */
    Matrix(
        dim_type numRows,
        dim_type numCols,
        bool symmetry);


    /**
    * @brief Virtual destructor.
    */
    virtual ~Matrix();


    /**
    * @brief The number of rows in the matrix.
    *
    * @return The number of rows.
    */
    dim_type getNumRows() const noexcept;


    /**
    * @brief The number of columns in the matrix.
    *
    * @return The number of columns.
    */
    dim_type getNumColumns() const noexcept;


    /**
    * @brief Check if the martix is symmetric.
    *
    * @return Whether or not the matrix is symmetric.
    */
    bool isSymmetric() const;


    /**
    * @brief Check if the matrix has had its symmetry determind.
    *
    * @return True if it has been set.
    */
    bool isSymmetrySet() const noexcept;


    /**
    * @brief Check if the matrix is square.
    *
    * @return True if its square.
    */
    bool isSquare() const noexcept;


    /**
    * @brief Check if the statistics have been computed.
    *
    * @return True if the statistics have been computed.
    */
    bool isStatsSet() const noexcept;


    /**
    * @brief Get the maximum number of non-zero per row.
    *
    * @return The maximum number of non-zeros.
    */
    dim_type getMaxRowSize() const;


    /**
    * @brief Get the maximum number of non-zero per column.
    *
    * @return The maximum number of non-zeros.
    */
    dim_type getMaxColumnSize() const;


    /**
    * @brief Get the number of empty rows in the matrix.
    *
    * @return The number of empty rows.
    */
    dim_type getNumEmptyRows() const;


    /**
    * @brief Get the number of empty columns in the matrix.
    *
    * @return The number of empty columns.
    */
    dim_type getNumEmptyColumns() const;


    /**
    * @brief Compute the statistics of the matrix.
    *
    * @param progress The progress counter to update.
    * @param scale The amount of progress this task contributes.
    */
    void computeStats(
        double * progress = nullptr,
        double scale = 1.0);


    /**
    * @brief Invalidate the current statistics for the matrix. 
    */
    void invalidateStats();


    /**
    * @brief Transpose the matrix.
    */
    void transpose();


    /**
    * @brief Transpose the matrix.
    *
    * @param progress The progress indicator to update.
    */
    void transpose(
        double * progress);


    /**
    * @brief Re-order the matrix.
    *
    * @param colPerm The column permutation.
    * @param rowPerm The row permutation.
    */
    void reorder(
        dim_type const * colPerm,
        dim_type const * rowPerm);


    /**
    * @brief Re-order the matrix.
    *
    * @param colPerm The column permutation.
    * @param rowPerm The row permutation.
    * @param progress The progress indicator to update.
    */
    void reorder(
        dim_type const * colPerm,
        dim_type const * rowPerm,
        double * progress);

    /**
    * @brief Re-order the matrix.
    *
    * @param colPerm The column permutation.
    * @param rowPerm The row permutation.
    * @param progress The progress indicator to update.
    * @param scale The fraction of the total progress to be updated.
    */
    virtual void reorder(
        dim_type const * colPerm,
        dim_type const * rowPerm,
        double * progress,
        double scale) = 0;


    /**
    * @brief Transpose the matrix.
    *
    * @param progress The progress indicator to update.
    * @param scale The fraction of the total progress to be updated.
    */
    virtual void transpose(
        double * progress,
        double scale) = 0;


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
    virtual void reduce(
        dim_type const * rows,
        dim_type numRows,
        dim_type const * cols,
        dim_type numCols,
        double * progress,
        double scale) = 0;


		/**
		* @brief Determine and set whether the matrix is symmetric.
    */
    void computeSymmetry();


		/**
		* @brief Determine and set whether the matrix is symmetric.
    *
    * @param progress The progress of the task.
    */
    void computeSymmetry(
        double * progress);


		/**
		* @brief Determine and set whether the matrix is symmetric.
    *
    * @param progress The progress of the task.
    * @param scale The fraction of the task this operation completes.
    */
    virtual void computeSymmetry(
        double * progress,
        double scale) = 0;


  protected:
    void setNumRows(
        dim_type numRows);


    void setNumColumns(
        dim_type numColumns);


    void setSymmetry(
        bool sym);


    void unsetSymmetry();



  private:
    dim_type m_numRows;
    dim_type m_numCols;
    bool m_symmetrySet;
    bool m_symmetry;
    bool m_statsSet;
    dim_type m_maxRowSize;
    dim_type m_maxColumnSize;
    dim_type m_numEmptyRows;
    dim_type m_numEmptyColumns;




};


}




#endif
