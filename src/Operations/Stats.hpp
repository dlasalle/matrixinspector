/**
 * @file Stats.hpp
 * @brief The Stats class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_OPERATIONS_STATS_HPP
#define MATRIXINSPECTOR_OPERATIONS_STATS_HPP




#include "Data/Matrix.hpp"




namespace MatrixInpsector
{


class Stats
{
  public:
    static void countRowNonZeros(
        Matrix * mat,
        dim_type * counts);


    static void countColumnNonZeros(
        Matrix * mat,
        dim_type * counts);




};




}




#endif
