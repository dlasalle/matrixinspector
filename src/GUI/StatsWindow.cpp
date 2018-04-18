/**
 * @file StatsWindow.cpp
 * @brief Implementation of the StatsWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <vector>
#include "GUI/WindowProperties.hpp"
#include "Data/SparseMatrix.hpp"
#include "Utility/String.hpp"
#include "StatsWindow.hpp"




namespace MatrixInspector
{



/******************************************************************************
* CONSTANTS *******************************************************************
******************************************************************************/

namespace
{

std::vector<std::string> const BOOL_NAMES {
  "no",
  "yes"
};

}




/******************************************************************************
* MACROS **********************************************************************
******************************************************************************/

wxBEGIN_EVENT_TABLE(StatsWindow, wxDialog)
  EVT_BUTTON(wxID_OK, StatsWindow::onOK)
wxEND_EVENT_TABLE()




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


StatsWindow::StatsWindow(
    wxFrame * const parent,
    DataStorage * const storage) :
  wxDialog(parent, wxID_ANY, "Statistics", wxDefaultPosition, wxDefaultSize),
  m_storage(storage)
{
  Matrix const * const mat = storage->getMatrix();

  wxBoxSizer * allSizer = new wxBoxSizer(wxVERTICAL);

  // matrix size
  addRowInt(allSizer,"Number of rows",mat->getNumRows());
  addRowInt(allSizer,"Number of columns",mat->getNumColumns());
  SparseMatrix const * spMat = dynamic_cast<SparseMatrix const *>(mat);
  if (spMat != nullptr) {
    addRowInt(allSizer,"Number of non-zeros", spMat->getNumNonZeros());
  }

  // matrix properties
  addRow(allSizer,"Square",BOOL_NAMES[mat->isSquare()]);
  addRow(allSizer,"Symmetric",BOOL_NAMES[mat->isSymmetric()]);

  if (spMat != nullptr) {
    addRow(allSizer,"Structurally Symmetric",
        BOOL_NAMES[spMat->isStructurallySymmetric()]);
  }

  // real stats
  addRowInt(allSizer,"Maximum NNZ per row", mat->getMaxRowSize());
  addRowInt(allSizer,"Maximum NNZ per column", mat->getMaxColumnSize());
  addRowInt(allSizer,"Number of empty rows", mat->getNumEmptyRows());
  addRowInt(allSizer,"Number of empty columns", mat->getNumEmptyColumns());

  // setup dialog buttons
  wxBoxSizer * bottomSizer = new wxBoxSizer(wxHORIZONTAL);
  bottomSizer->Add(new wxButton(this, wxID_OK, "Ok"), BORDER);
  allSizer->Add(bottomSizer,0,wxALIGN_RIGHT);

  SetSizerAndFit(allSizer);
}


StatsWindow::~StatsWindow()
{
  // do nothing
}




/******************************************************************************
* PRIVATE FUNCTIONS ***********************************************************
******************************************************************************/


void StatsWindow::addRow(
    wxBoxSizer * const topSizer,
    std::string const key,
    std::string const value)
{
  wxBoxSizer * rowSizer = new wxBoxSizer(wxHORIZONTAL);

  rowSizer->Add(new wxStaticText(this,wxID_ANY, key + std::string(":")), 0, \
      wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER);

  rowSizer->Add(new wxTextCtrl(this, wxID_ANY, value, \
      wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_RIGHT), 1, \
      wxEXPAND | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER);

  topSizer->Add(rowSizer,1,wxEXPAND);
}


void StatsWindow::addRowInt(
    wxBoxSizer * const topSizer,
    std::string const key,
    index_type const num)
{
  addRow(topSizer, key, String::addThousandsSeparators(num));
}


void StatsWindow::addRowFloat(
    wxBoxSizer * const topSizer,
    std::string const key,
    double const num)
{
  addRow(topSizer, key, std::to_string(num));
}


void StatsWindow::onOK(
    wxCommandEvent&)
{
  if (IsModal()) {
    EndDialog(wxID_OK);
  } else {
    SetReturnCode(wxID_OK);
    Show(false);
  }
}




}
