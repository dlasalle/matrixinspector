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
#include <stdexcept>




namespace MatrixInspector
{



/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/

bool App::OnInit()
{
  wxDisplay disp;
  wxRect rect = disp.GetGeometry();

  MainWindow * win = new MainWindow("MatrixInspector",rect.GetWidth()*0.1, \
      rect.GetHeight()*0.1,rect.GetWidth()*0.8,rect.GetHeight()*0.8);
  win->Show(true);

  if (wxApp::argc == 1) {
    // proceed normally
  } else if (wxApp::argc == 2) {
    // load a matrix/graph
    win->load(std::string(wxApp::argv[1].mb_str()));
  } else {
    // bad commands
    throw std::runtime_error("Bad arguments");
  }

  return true;
}


}
