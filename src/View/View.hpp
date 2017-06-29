/**
 * @file View.hpp
 * @brief The View class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_VIEW_VIEW_HPP
#define MATRIXINSPECTOR_VIEW_VIEW_HPP




#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "Data/Matrix.hpp"




namespace MatrixInpsector
{


class View :
  public wxGLCanvas
{
  public:
    View(
        wxFrame * parent);


    virtual ~View();


    void render(
        wxPaintEvent& event);


    void render();


    void setMatrix(
        Matrix const * matrix);


    void moveCamera(
        float x,
        float y);


    virtual void refresh() = 0;


  protected:
    virtual void draw() = 0;


    Matrix const * getMatrix() const;


    float getGLWidth() const;


    float getGLHeight() const;


  private:
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
    wxGLContext m_context;
    Matrix const * m_matrix;


    DECLARE_EVENT_TABLE()


    // disable copying
    View(
        View const & rhs);
    View& operator=(
        View const & rhs);



};




}




#endif
