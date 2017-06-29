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




namespace MatrixInpsector
{


class Matrix
{
  public:
    struct stats_struct {
      dim_type maxRowSize;
      dim_type maxColSize;
      dim_type numEmptyRows;
      dim_type numEmptyCols;
    };


    Matrix(
        dim_type numRows,
        dim_type numCols);


    Matrix(
        dim_type numRows,
        dim_type numCols,
        bool symmetry);


    virtual ~Matrix();


    dim_type getNumRows() const noexcept;


    dim_type getNumColumns() const noexcept;


    bool isSymmetric() const;


    bool isSymmetrySet() const noexcept;


    bool isSquare() const noexcept;


    bool isStatsSet() const noexcept;


    stats_struct getStats() const;


    stats_struct getStats();


    void computeStats(
        double * progress = nullptr,
        double scale = 1.0);


    void invalidateStats();


    void transpose();


    void transpose(
        double * progress);


    void reorder(
        dim_type const * colPerm,
        dim_type const * rowPerm);


    void reorder(
        dim_type const * colPerm,
        dim_type const * rowPerm,
        double * progress);


    virtual void reorder(
        dim_type const * colPerm,
        dim_type const * rowPerm,
        double * progress,
        double scale) = 0;


    virtual void transpose(
        double * progress,
        double scale) = 0;


    virtual void reduce(
        dim_type const * rows,
        dim_type numRows,
        dim_type const * cols,
        dim_type numCols,
        double * progress,
        double scale) = 0;



    virtual void computeSymmetry() = 0;




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
    stats_struct m_stats;




};


}




#endif
