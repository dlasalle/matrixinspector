/**
 * @file HeatMapView.hpp
 * @brief The HeatMapView class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */





#ifndef MATRIXINSPECTOR_VIEW_HEATMAPVIEW_HPP
#define MATRIXINSPECTOR_VIEW_HEATMAPVIEW_HPP




#include "View.hpp"
#include "HeatMap.hpp"




namespace MatrixInspector
{


class HeatMapView :
  public View
{
  public:
    HeatMapView(
        wxFrame * parent);


    ~HeatMapView();


    void draw() override;


    void refresh() override;


  private:
    HeatMap m_heatmap;
    GLuint m_glTexture;
    wxPoint m_mousePos;
    float m_zoom;
    float m_originX;
    float m_originY;


    void onMouseMove(
        wxMouseEvent& event);


    void onKey(
        wxKeyEvent& event);


    void onWheel(
        wxMouseEvent& event);


    void releaseTexture();


    // disable copying
    HeatMapView(
        HeatMapView const & rhs);
    HeatMapView& operator=(
        HeatMapView const & rhs);


    wxDECLARE_EVENT_TABLE();




};




}




#endif
