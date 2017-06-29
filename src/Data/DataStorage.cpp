/**
 * @file DataStorage.cpp
 * @brief Implementation of the DataStorage class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */


#include <iostream>
#include <stdexcept>
#include <wildriver.h>
#include "Types.hpp"
#include "DataStorage.hpp"
#include "Data/CSRMatrix.hpp"
#include "Utility/Timer.hpp"



namespace MatrixInpsector
{


/******************************************************************************
* PUBLIC STATIC FUNCTIONS *****************************************************
******************************************************************************/


std::vector<value_type> DataStorage::loadVector(
    char const * const path)
{
  wildriver_vector_handle * handle = wildriver_open_vector(path,WILDRIVER_IN);

  if (handle == nullptr) {
    throw std::runtime_error(std::string("Failed to open ") + path);
  }

  std::vector<value_type> vec(handle->size);
  int rv = wildriver_load_vector(handle,vec.data(),nullptr);
  if (rv != 1) {
    throw std::runtime_error(std::string("Error while reading ") + path);
  }

  wildriver_close_vector(handle);

  return vec;
}




/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/


DataStorage::DataStorage() :
  m_matrix(nullptr)
{
  // do nothing
}


DataStorage::~DataStorage()
{
  // do nothing
}




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


void DataStorage::loadDataset(
    char const * const path,
    double * const progress)
{
  Timer tmr;

  tmr.start();

  wildriver_matrix_handle * const handle = \
      wildriver_open_matrix(path,WILDRIVER_IN);

  if (handle == nullptr) {
    throw std::runtime_error("Failed to open dataset.");
  }

  m_matrix.reset(new CSRMatrix(handle->nrows,handle->ncols,handle->nnz));
  CSRMatrix * mat = dynamic_cast<CSRMatrix*>(m_matrix.get());

  if (wildriver_load_matrix(handle,mat->getOffsets(), mat->getColumns(), \
        mat->getValues(),progress)) {
    wildriver_close_matrix(handle);
  } else {
    wildriver_close_matrix(handle);
    throw std::runtime_error("Failed to load dataset.");
  }

  tmr.stop();

  std::cout << "Loading took: " << tmr.poll() << "s" << std::endl;
}


void DataStorage::saveDataset(
    char const * const path,
    double * const progress)
{
  CSRMatrix const * csr;
  if ((csr = dynamic_cast<CSRMatrix const *>(m_matrix.get())) != nullptr) {
    wildriver_matrix_handle * handle = \
        wildriver_open_matrix(path,WILDRIVER_OUT);

    if (handle == nullptr) {
      throw std::runtime_error(std::string("Unable to open ") +
          std::string(path) + std::string(" for writing."));
    }

    handle->nrows = csr->getNumRows();
    handle->ncols = csr->getNumColumns();
    handle->nnz = csr->getNumNonZeros();

    int rv = wildriver_save_matrix(handle, csr->getOffsets(), \
        csr->getColumns(), csr->getValues(), progress);

    if (rv != 1) {
      throw std::runtime_error("Failed to save dataset.");
    }

    wildriver_close_matrix(handle);
  } else {
    throw std::runtime_error("Saving dense matrices is not implemented yet.");
  }
}


Matrix const * DataStorage::getMatrix() const
{
  return m_matrix.get();
}


Matrix * DataStorage::getMatrix()
{
  return m_matrix.get();
}





}
