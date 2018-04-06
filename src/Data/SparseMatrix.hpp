/**
* @file SparseMatrix.hpp
* @brief The SparseMatrix class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-04-06
*/




#ifndef MATRIXINSPECTOR_SPARSEMATRIX_HPP
#define MATRIXINSPECTOR_SPARSEMATRIX_HPP



#include "Matrix.hpp"
#include "Types.hpp"

namespace MatrixInspector
{

class SparseMatrix :
  public Matrix
{
  public:
    /**
    * @brief Create a new CSR matrix.
    *
    * @param numRows The number of rows.
    * @param numCols The number of columns.
    * @param numNonZeros The number of non-zeros.
    */
    SparseMatrix(
        dim_type numRows,
        dim_type numCols,
        index_type numNonZeros);


    /**
    * @brief Virtual destructor.
    */
    virtual ~SparseMatrix();


    /**
    * @brief Get the number of non-zeros in the matrix.
    *
    * @return The number of non-zeros.
    */
    index_type getNumNonZeros() const;


    /**
    * @brief Check if this matrix is structurally symmetric.
    *
    * @return True if it is symmetric.
    */
    bool isStructurallySymmetric() const;

  protected:
    /**
    * @brief Set the number of non-zeros in the sparse matrix.
    *
    * @param numNonZeros The number of non-zeros.
    */
    void setNumNonZeros(
        index_type numNonZeros);

    /**
    * @brief Set the structural symmetry of the matrix.
    *
    * @param structuralSymmetry The structural symmetry.
    */
    void setStructuralSymmetry(
        bool structuralSymmetry);

  private:
    index_type m_numNonZeros;
    bool m_structuralSymmetry;
    bool m_structuralSymmetrySet;
    
};

}

#endif
