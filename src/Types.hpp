/**
 * @file Types.hpp
 * @brief Basic types.
 * @author Dominique LaSalle <dominique@domnet.org>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_TYPES_HPP
#define MATRIXINSPECTOR_TYPES_HPP




#include <cstdint>
#include <cstddef>




namespace MatrixInspector
{


typedef WILDRIVER_DIMENSION_TYPE dim_type;
typedef WILDRIVER_INDEX_TYPE index_type;
typedef WILDRIVER_VALUE_TYPE value_type;


static dim_type const NULL_DIM = static_cast<dim_type>(-1);
static index_type const NULL_INDEX = static_cast<index_type>(-1);


}




#endif
