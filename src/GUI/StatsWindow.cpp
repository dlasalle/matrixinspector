/**
 * @file StatsWindow.cpp
 * @brief Implementation of the StatsWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <vector>
#include "GUI/WindowProperties.hpp"
#include "Data/CSRMatrix.hpp"
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
  addRow(allSizer,"Number of rows",mat->getNumRows());
  addRow(allSizer,"Number of columns",mat->getNumColumns());
  CSRMatrix const * csr;
  if ((csr = dynamic_cast<CSRMatrix const *>(mat)) != nullptr) {
    addRow(allSizer,"Number of non-zeros", csr->getNumNonZeros());
  }

  // matrix properties
  addRow(allSizer,"Square",BOOL_NAMES[mat->isSquare()]);
  addRow(allSizer,"Symmetric",BOOL_NAMES[mat->isSymmetric()]);

  // real stats
  addRow(allSizer,"Maximum NNZ per row", mat->getMaxRowSize());
  addRow(allSizer,"Maximum NNZ per column", mat->getMaxColumnSize());
  addRow(allSizer,"Number of empty rows", mat->getNumEmptyRows());
  addRow(allSizer,"Number of empty columns", mat->getNumEmptyColumns());

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


void StatsWindow::addRow(
    wxBoxSizer * const topSizer,
    std::string const key,
    index_type num)
{
  addRow(topSizer, key, String::addThousandsSeparators(num));
}


void StatsWindow::addRow(
    wxBoxSizer * const topSizer,
    std::string const key,
    dim_type num)
{
  addRow(topSizer,key,static_cast<index_type>(num));
}


void StatsWindow::addRow(
    wxBoxSizer * const topSizer,
    std::string const key,
    double num)
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
