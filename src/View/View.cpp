/**
 * @file View.cpp
 * @brief Implementation of the View class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include "View.hpp"




namespace MatrixInpsector
{


/******************************************************************************
* CONSTANTS *******************************************************************
******************************************************************************/

namespace
{

float const MARGIN = 1.2f;

}


/******************************************************************************
* MACROS **********************************************************************
******************************************************************************/


wxBEGIN_EVENT_TABLE(View, wxGLCanvas)
  EVT_PAINT(View::render)
wxEND_EVENT_TABLE()




/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/


View::View(
    wxFrame * const parent) :
  // TODO: update for wxwidgets 3.1+
  wxGLCanvas(parent, wxID_ANY, nullptr),
  m_left(0),
  m_right(0),
  m_bottom(0),
  m_top(0),
  m_context(this),
  m_matrix(nullptr)
{
  // do nothing
}


View::~View()
{
  // do nothing
}




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


void View::setMatrix(
    Matrix const * const matrix)
{
  m_matrix = matrix;

  refresh();
}


void View::render(
    wxPaintEvent&)
{
  render();
}


void View::render()
{
  int const width = GetSize().x;
  int const height = GetSize().y;

  int const smallest = std::min(width,height);

  m_left = MARGIN * (-width / (2.0*smallest));
  m_right = MARGIN * (width / (2.0*smallest));
  m_bottom = MARGIN * (-height / (2.0*smallest));
  m_top = MARGIN * (height / (2.0*smallest));

	wxPaintDC(this);
  SetCurrent(m_context);

  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

	glClearColor(0.1, 0.1, 0.1, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(m_left,m_right,m_bottom,m_top, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  draw();

	glFlush();
	SwapBuffers();
}


float View::getGLWidth() const
{
  return m_right - m_left;
}


float View::getGLHeight() const
{
  return m_top - m_bottom;
}





/******************************************************************************
* PROTECTED FUNCTIONS *********************************************************
******************************************************************************/


Matrix const * View::getMatrix() const
{
  return m_matrix;
}




}
