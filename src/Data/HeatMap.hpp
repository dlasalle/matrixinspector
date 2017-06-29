/**
 * @file HeatMap.hpp
 * @brief The HeatMap class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_HEATMAP_HPP
#define MATRIXINSPECTOR_HEATMAP_HPP




#include <cassert>
#include <vector>
#include "Types.hpp"




namespace MatrixInpsector
{


class HeatMap
{
  public:
    static inline uint32_t floatToRGBA(
        float const val)
    {
      assert(val <= 1.0);
      assert(val >= 0.0);

      uint32_t R = 0;
      uint32_t G = 0;
      uint32_t B = 0;

      float const mag = 255.0*4.0;

      if (val < 0.25) {
        B = static_cast<uint32_t>(val*mag);
      } else if (val < 0.5) {
        G = static_cast<uint32_t>((val - 0.25)*mag);
        B = static_cast<uint32_t>((0.5 - val)*mag);
      } else if (val < 0.75) {
        R = static_cast<uint32_t>((val - 0.5)*mag);
        G = static_cast<uint32_t>((0.75 - val)*mag);
      } else {
        R = 255;
        G = static_cast<uint32_t>((val - 0.75)*mag);
        B = static_cast<uint32_t>((val - 0.75)*mag); 
      }

      assert(R < 256);
      assert(G < 256);
      assert(B < 256);

      return (B << 16) | (G << 8) | R << 0;
    }


    HeatMap(
        dim_type width = 1,
        dim_type height = 1,
        value_type const def = 0.0);


    ~HeatMap();


    dim_type getWidth() const noexcept;


    dim_type getHeight() const noexcept;


    std::vector<value_type> const * getValues() const noexcept;


    void resize(
        dim_type width,
        dim_type height);


    void normalize();


    inline void add(
        dim_type const x,
        dim_type const y,
        value_type const val = 1.0)
    {
      index_type const idx = (y*m_width)+x;
      m_values[idx] += val;

      if (m_values[idx] > m_max) {
        m_max = m_values[idx];
      }
      if (m_values[idx] < m_min) {
        m_min = m_values[idx];
      }
    }


  private:
    dim_type m_width;
    dim_type m_height;
    value_type m_default;
    value_type m_min;
    value_type m_max;
    std::vector<value_type> m_values;

};


}




#endif
