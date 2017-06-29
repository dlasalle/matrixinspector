/**
 * @file HeatMap.cpp
 * @brief Implementation of the HeatMap class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include "HeatMap.hpp"




namespace MatrixInpsector
{


/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/


HeatMap::HeatMap(
    dim_type const width,
    dim_type const height,
    value_type const def) :
  m_width(width),
  m_height(height),
  m_default(def),
  m_min(def),
  m_max(def),
  m_values(width*height,def)
{
  // do nothing
}


HeatMap::~HeatMap()
{
  // do nothing
}




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


dim_type HeatMap::getWidth() const noexcept
{
  return m_width;
}


dim_type HeatMap::getHeight() const noexcept
{
  return m_height;
}


std::vector<value_type> const * HeatMap::getValues() const noexcept
{
  return &m_values;
}


void HeatMap::resize(
    dim_type const width,
    dim_type const height)
{
  m_width = width;
  m_height = height;

  m_values.assign(width*height,m_default);

  m_min = m_default;
  m_max = m_default;
}


void HeatMap::normalize()
{
  size_t const numValues = m_values.size();
  const value_type range = m_max-m_min;

  if (range > 0) {
    for (size_t i = 0; i < numValues; ++i) {
      m_values[i] = (m_values[i] - m_min) / range;
    }
  } else {
    for (size_t i = 0; i < numValues; ++i) {
      m_values[i] = 0.5f;
    }
  }
}



}


