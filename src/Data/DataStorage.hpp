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
    /**
    * @brief Load a vector from disk.
    *
    * @param path The location of the vector.
    *
    * @return The loaded vector.
    */
    static std::vector<value_type> loadVector(
        char const * path);


    /**
    * @brief Create a new data storage object.
    */
    DataStorage();


    /**
    * @brief Destructor.
    */
    ~DataStorage();


    /**
    * @brief Load a new dataset into memory.
    *
    * @param path The path of the dataset.
    * @param progress The progess variable.
    */
    void loadDataset(
        char const * path,
        double * progress);


    /**
    * @brief Save a dataset to memory.
    *
    * @param path The path of the dataset to write.
    * @param progress The progress variable.
    */
    void saveDataset(
        char const * path,
        double * progress);


    /**
    * @brief Get the matrix in this storage.
    *
    * @return The loaded matrix.
    */
    Matrix const * getMatrix() const;


    /**
    * @brief Get the matrix in this storage.
    *
    * @return The loaded matrix.
    */
    Matrix * getMatrix();


  private:
    std::unique_ptr<Matrix> m_matrix;


};




}




#endif
