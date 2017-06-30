/**
 * @file HeatMapView.cpp
 * @brief Implementation of the HeatMapView class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <GL/gl.h>
#include <GL/glu.h>
#include "HeatMapView.hpp"
#include "Data/CSRMatrix.hpp"
#include "Utility/Debug.hpp"




namespace MatrixInspector
{



/******************************************************************************
* CONSTANTS *******************************************************************
******************************************************************************/

namespace
{

float const TARGET_PIXELS = 2048*2048;

GLuint const NULL_TEXTURE = static_cast<GLuint>(-1);

double const MIN_ZOOM = 0.8f;

}



/******************************************************************************
* MACROS **********************************************************************
******************************************************************************/

wxBEGIN_EVENT_TABLE(HeatMapView, View)
  EVT_MOTION(HeatMapView::onMouseMove)
  EVT_MOUSEWHEEL(HeatMapView::onWheel)
  EVT_CHAR(HeatMapView::onKey)
wxEND_EVENT_TABLE()


/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/

HeatMapView::HeatMapView(
    wxFrame * const parent) :
  View(parent),
  m_heatmap(),
  m_glTexture(NULL_TEXTURE),
  m_mousePos(0,0),
  m_zoom(1.0f),
  m_originX(0),
  m_originY(0)
{
  // do nothing
}



HeatMapView::~HeatMapView()
{
  releaseTexture();
}




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


void HeatMapView::refresh()
{
  // reset view
  m_zoom = 1.0;
  m_originX = 0;
  m_originY = 0;

  Matrix const * matrix = getMatrix();

  if (matrix == nullptr) {
    // nothing to refresh
    return;
  }

  dim_type const hPixels = std::sqrt( \
      TARGET_PIXELS * \
      (static_cast<float>(matrix->getNumRows()) / \
      static_cast<float>(matrix->getNumColumns())));
  dim_type const wPixels = std::ceil(TARGET_PIXELS / hPixels);

  float const conv = static_cast<float>(hPixels) / \
      static_cast<float>(matrix->getNumRows());

  m_heatmap.resize(wPixels, hPixels);

  ASSERT_LESSEQUAL(static_cast<dim_type>(matrix->getNumRows()*conv),hPixels);
  ASSERT_LESSEQUAL(static_cast<dim_type>(matrix->getNumColumns()*conv), \
      wPixels);

  CSRMatrix const * csrPtr;

  // fill heat map
  if ((csrPtr = dynamic_cast<CSRMatrix const *>(matrix)) != nullptr) {
    dim_type const numRows = csrPtr->getNumRows();

    index_type const * const offsets = csrPtr->getOffsets();
    dim_type const * const columns = csrPtr->getColumns();

    for (dim_type row = 0; row < numRows; ++row) {
      for (index_type idx = offsets[row]; idx < offsets[row+1]; ++idx) {
        dim_type const column = columns[idx];
        dim_type const x = column * conv;
        ASSERT_LESS(x,wPixels);
        dim_type const y = row * conv;
        ASSERT_LESS(y,hPixels);

        m_heatmap.add(x,y);
      }
    }
  }
  m_heatmap.normalize();

  // generate texture
  std::vector<uint32_t> pixels(m_heatmap.getWidth()*m_heatmap.getHeight());
  size_t const numPixels = m_heatmap.getValues()->size();
  value_type const * const values = m_heatmap.getValues()->data(); 
  for (size_t i = 0; i < numPixels; ++i) {
    pixels[i] = HeatMap::floatToRGBA(values[i]);
  }

  // setup opengl texture
  glGenTextures(1,&m_glTexture);
  glBindTexture(GL_TEXTURE_2D,m_glTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, 4, m_heatmap.getWidth(),
      m_heatmap.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, \
			GL_NEAREST_MIPMAP_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D); on 3.0
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_heatmap.getWidth(), \
			m_heatmap.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}



/******************************************************************************
* PROTECTED FUNCTIONS *********************************************************
******************************************************************************/


void HeatMapView::draw()
{
  Matrix const * matrix = getMatrix();

  if (matrix != nullptr) {
    dim_type const maxDim = std::max(
        matrix->getNumRows(),
        matrix->getNumColumns());

    float const width = matrix->getNumColumns() / static_cast<float>(maxDim);
    float const height = matrix->getNumRows() / static_cast<float>(maxDim);

    glPushMatrix();

    glScalef(m_zoom,m_zoom,1.0f);

    float const xTrans = m_originX *getGLWidth() / GetSize().x;
    float const yTrans = -m_originY *getGLHeight() / GetSize().y;

    glTranslatef(xTrans,yTrans,0.0f);

    glBindTexture(GL_TEXTURE_2D,m_glTexture);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-0.5f*width,-0.5f*height,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.5f*width,-0.5f*height,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(0.5f*width,0.5f*height,0.0f);

    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-0.5f*width,-0.5f*height,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(0.5f*width,0.5f*height,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-0.5f*width,0.5f*height,0.0f);

    glEnd();

    glPopMatrix();
  }
}



/******************************************************************************
* PRIVATE FUNCTIONS ***********************************************************
******************************************************************************/


void HeatMapView::onMouseMove(
    wxMouseEvent& event)
{
  wxPoint const newPos = event.GetPosition();

  if (event.Dragging()) {
    float const deltaX = newPos.x - m_mousePos.x;
    float const deltaY = newPos.y - m_mousePos.y;

    m_originX += (deltaX/m_zoom);
    m_originY += (deltaY/m_zoom);

    // seemse like a bad way to update...
    render();
  }

  m_mousePos = newPos;
}


void HeatMapView::onKey(
    wxKeyEvent& event)
{
  int const keyCode = event.GetKeyCode();  
  if (keyCode == '=') {
    m_zoom *= 1.05;
  } else if (keyCode == '-') {
    m_zoom = std::max(MIN_ZOOM, m_zoom/1.05);
  }

  render();
}


void HeatMapView::onWheel(
    wxMouseEvent& event)
{
  m_zoom = std::max(MIN_ZOOM, \
      m_zoom * (1.0 + (static_cast<float>(event.GetWheelRotation())/1000.0)));

  render();
}


void HeatMapView::releaseTexture()
{
  // release previous texture if needed
  if (m_glTexture != NULL_TEXTURE) {
    glDeleteTextures(1,&m_glTexture);
    m_glTexture = NULL_TEXTURE;
  }
}


}
