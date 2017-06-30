/**
 * @file DataStorage.hpp
 * @brief The DataStorage class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_DATASTORAGE_HPP
#define MATRIXINSPECTOR_DATASTORAGE_HPP




#include <memory>
#include <vector>
#include "Matrix.hpp"




namespace MatrixInspector
{


class DataStorage
{
  public:
    static std::vector<value_type> loadVector(
        char const * path);


    DataStorage();


    ~DataStorage();


    void loadDataset(
        char const * path,
        double * progress);


    void saveDataset(
        char const * path,
        double * progress);


    Matrix const * getMatrix() const; 


    Matrix * getMatrix(); 


  private:
    std::unique_ptr<Matrix> m_matrix;


};




}




#endif
