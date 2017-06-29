/**
 * @file App.hpp
 * @brief The App class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_GUI_APP_HPP
#define MATRIXINSPECTOR_GUI_APP_HPP




#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif




namespace MatrixInpsector
{


class App:
  public wxApp
{
  public:
    virtual bool OnInit();

};




}




#endif
