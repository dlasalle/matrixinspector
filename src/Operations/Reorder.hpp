/**
 * @file Reorder.hpp
 * @brief The Reorder class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_OPERATIONS_REORDER_HPP
#define MATRIXINSPECTOR_OPERATIONS_REORDER_HPP




#include "Data/Matrix.hpp"




namespace MatrixInpsector
{



class Reorder
{
  public:
    static void random(
        Matrix * matrix,
        bool rows,
        bool columns,
        uint32_t seed,
        bool symmetric = false,
        double * progress = nullptr,
        double scale = 1.0f);


    static void density(
        Matrix * matrix,
        bool rows,
        bool columns,
        bool descending = true,
        double * progress = nullptr,
        double scale = 1.0f);


    static void keys(
        Matrix * matrix,
        value_type const * rowKeys,
        value_type const * colKeys,
        bool ascending = true,
        double * progress = nullptr,
        double scale = 1.0f);


    static void keys(
        Matrix * matrix,
        dim_type const * rowKeys,
        dim_type const * colKeys,
        bool ascending = true,
        double * progress = nullptr,
        double scale = 1.0f);




};




}




#endif
