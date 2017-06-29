/**
 * @file App.cpp
 * @brief Implementation of the App class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */





#include <wx/wx.h>
#include <wx/display.h>
#include "App.hpp"
#include "MainWindow.hpp"




namespace MatrixInpsector
{



/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/

bool App::OnInit()
{
  wxDisplay disp;
  wxRect rect = disp.GetGeometry();

  MainWindow * win = new MainWindow("MatrixInpsector",rect.GetWidth()*0.1, \
      rect.GetHeight()*0.1,rect.GetWidth()*0.8,rect.GetHeight()*0.8);
  win->Show(true);
  return true;
}


}
