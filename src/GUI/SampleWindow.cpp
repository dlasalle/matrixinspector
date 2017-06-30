/**
 * @file SampleWindow.cpp
 * @brief Implementation of the SampleWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include "SampleWindow.hpp"
#include "GUI/WindowProperties.hpp"
#include "Data/CSRMatrix.hpp"
#include "Utility/String.hpp"
#include "Utility/Debug.hpp"




namespace MatrixInspector
{



/******************************************************************************
* TYPES ***********************************************************************
******************************************************************************/

namespace
{

enum event_types {
  // random
  ID_RANDOM,
  ID_RAND_ROW_CHECK,
  ID_RAND_COL_CHECK,
  // threshold
  ID_THRESHOLD,
  ID_THRESH_SUBJECT
};


enum cond_types {
  MORE = 0,
  LESS = 1
};



}


/******************************************************************************
* MACROS **********************************************************************
******************************************************************************/

wxBEGIN_EVENT_TABLE(SampleWindow, wxDialog)
  EVT_RADIOBUTTON(ID_RANDOM, SampleWindow::onRandomSelected)
  EVT_RADIOBUTTON(ID_THRESHOLD, SampleWindow::onThresholdSelected)
  EVT_CHECKBOX(ID_RAND_ROW_CHECK, SampleWindow::onRandomRowChecked)
  EVT_CHECKBOX(ID_RAND_COL_CHECK, SampleWindow::onRandomColChecked)
  EVT_CHOICE(ID_THRESH_SUBJECT, SampleWindow::onThresholdSubjectSelected)
  EVT_BUTTON(wxID_OK, SampleWindow::onOK)
  EVT_BUTTON(wxID_CANCEL, SampleWindow::onCancel)
wxEND_EVENT_TABLE()



/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/


SampleWindow::SampleWindow(
    wxFrame * const parent,
    DataStorage * const storage) :
  wxDialog(parent, wxID_ANY, "Sample", wxDefaultPosition, wxDefaultSize),
  m_options{RANDOM,false,0,0,ROWS,0,0},
  m_storage(storage),
  m_randomRadio(nullptr),
  m_randRowCheck(nullptr),
  m_randRowText(nullptr),
  m_randColCheck(nullptr),
  m_randColText(nullptr),
  m_randNumRows(0),
  m_randNumCols(0),
  m_thresholdRadio(nullptr),
  m_threshSubjectChoice(nullptr),
  m_threshCondChoice(nullptr),
  m_threshRowText(nullptr),
  m_threshColText(nullptr),
  m_threshNNZRows(0),
  m_threshNNZCols(0)
{
  CSRMatrix const * const mat = \
      dynamic_cast<const CSRMatrix*>(m_storage->getMatrix());
  ASSERT_NOTNULL(mat);

  dim_type const numRows = mat->getNumRows();
  dim_type const numCols = mat->getNumColumns();
  index_type const numNZ = mat->getNumNonZeros();

  wxBoxSizer * allSizer = new wxBoxSizer(wxVERTICAL);

  wxArrayString threshSubjects;
  threshSubjects.Add("rows");
  threshSubjects.Add("columns");

  wxArrayString threshOpts;
  threshOpts.Add("more than");
  threshOpts.Add("less than");

  // set initial values
  m_randNumRows = numRows/2;
  m_randNumCols = numCols/2;
  m_threshNNZRows = numNZ / numRows;
  m_threshNNZCols = numNZ / numCols;

  // build radio buttons
  m_randomRadio = new wxRadioButton(this,ID_RANDOM,"Random");
  m_thresholdRadio = new wxRadioButton(this,ID_THRESHOLD,"Threshold");

  // random sampling options
  wxBoxSizer * randSizer = new wxBoxSizer(wxVERTICAL);
  randSizer->Add(m_randomRadio);

  wxBoxSizer * horRandRowSizer = new wxBoxSizer(wxHORIZONTAL);
  m_randRowCheck = new wxCheckBox(this,ID_RAND_ROW_CHECK,"Rows");
  m_randRowCheck->Disable();
  horRandRowSizer->Add(m_randRowCheck,0, wxALIGN_CENTER_VERTICAL, BORDER);
  wxIntegerValidator<dim_type> randRowVal(&m_randNumRows);
  randRowVal.SetRange(1,numRows);
  m_randRowText = new wxTextCtrl(this, wxID_ANY, \
      std::to_string(m_randNumRows), wxDefaultPosition, wxDefaultSize,
      wxTE_RIGHT, randRowVal);
  m_randRowText->Disable();
  horRandRowSizer->Add(m_randRowText);
  horRandRowSizer->Add(new wxStaticText(this,wxID_ANY,std::string("/") + \
      String::addThousandsSeparators(numRows)), 0, \
      wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, \
      BORDER);
  m_randRowCheck->SetValue(true);
  randSizer->Add(horRandRowSizer);

  wxBoxSizer * horRandColSizer = new wxBoxSizer(wxHORIZONTAL);
  m_randColCheck = new wxCheckBox(this,ID_RAND_COL_CHECK,"Columns");
  m_randColCheck->Disable();
  horRandColSizer->Add(m_randColCheck,0, wxALIGN_CENTER_VERTICAL, BORDER);
  wxIntegerValidator<dim_type> randColVal(&m_randNumCols);
  randColVal.SetRange(1,numCols);
  m_randColText = new wxTextCtrl(this, wxID_ANY, \
      std::to_string(m_randNumCols), wxDefaultPosition, wxDefaultSize,
      wxTE_RIGHT, randColVal);
  m_randColText->Disable();
  horRandColSizer->Add(m_randColText);
  horRandColSizer->Add(new wxStaticText(this,wxID_ANY,std::string("/") + \
      String::addThousandsSeparators(numRows)), 0, \
      wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, \
      BORDER);
  m_randColCheck->SetValue(true);
  randSizer->Add(horRandColSizer);

  allSizer->Add(randSizer);

  // threshold sampling
  wxBoxSizer * threshSizer = new wxBoxSizer(wxVERTICAL);
  threshSizer->Add(m_thresholdRadio);

  wxBoxSizer * horThreshSizer = new wxBoxSizer(wxHORIZONTAL);
  m_threshSubjectChoice = new wxChoice(this,ID_THRESH_SUBJECT, \
      wxDefaultPosition,wxDefaultSize,threshSubjects);
  m_threshSubjectChoice->Disable();
  horThreshSizer->Add(m_threshSubjectChoice);
  m_threshCondChoice = new wxChoice(this,wxID_ANY,wxDefaultPosition, \
      wxDefaultSize,threshOpts);
  m_threshCondChoice->SetSelection(MORE);
  m_threshCondChoice->Disable();
  horThreshSizer->Add(m_threshCondChoice);

  // have one text box for rows
  wxIntegerValidator<dim_type> threshRowVal(&m_threshNNZRows);
  threshRowVal.SetRange(0,numCols);
  m_threshRowText = new wxTextCtrl(this,wxID_ANY, \
      std::to_string(m_threshNNZCols),wxDefaultPosition, wxDefaultSize, 0, \
      threshRowVal);
  m_threshRowText->Disable();
  horThreshSizer->Add(m_threshRowText);

  // and one for columns
  wxIntegerValidator<dim_type> threshColVal(&m_threshNNZCols);
  threshColVal.SetRange(0,numCols);
  m_threshColText = new wxTextCtrl(this,wxID_ANY, \
      std::to_string(m_threshNNZCols),wxDefaultPosition, wxDefaultSize, 0, \
      threshColVal);
  m_threshColText->Disable();
  m_threshColText->Hide();
  horThreshSizer->Add(m_threshColText);

  // select rows by default
  m_threshSubjectChoice->SetSelection(ROWS);

  horThreshSizer->Add(new wxStaticText(this, wxID_ANY,"non-zeros"), 0, \
      wxALIGN_CENTER_VERTICAL, BORDER);
  threshSizer->Add(horThreshSizer);
  allSizer->Add(threshSizer);

  // setup dialog buttons
  wxBoxSizer * bottomSizer = new wxBoxSizer(wxHORIZONTAL);
  bottomSizer->Add(new wxButton(this, wxID_OK, "Sample"), BORDER);
  bottomSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), BORDER);
  allSizer->Add(bottomSizer,0,wxALIGN_RIGHT);

  SetSizerAndFit(allSizer);

  // select random radio button
  assert(m_randomRadio->GetValue());
  activateRandom(true);
}


SampleWindow::~SampleWindow()
{
  // do nothing
}




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


SampleWindow::sample_struct SampleWindow::getOptions() const
{
  return m_options;
}



/******************************************************************************
* PRIVATE FUNCTIONS ***********************************************************
******************************************************************************/


void SampleWindow::activateRandom(
    bool const active)
{
  if (active) {
    m_randRowCheck->Enable();
    if (m_randRowCheck->GetValue()) {
      m_randRowText->Enable();
    }
    m_randColCheck->Enable();
    if (m_randColCheck->GetValue()) {
      m_randColText->Enable();
    }
  } else {
    m_randRowCheck->Disable();
    m_randRowText->Disable();
    m_randColCheck->Disable();
    m_randColText->Disable();
  }
}


void SampleWindow::activateThreshold(
    bool const active)
{
  if (active) {
    m_threshSubjectChoice->Enable();
    m_threshCondChoice->Enable();
    m_threshRowText->Enable();
    m_threshColText->Enable();
  } else {
    m_threshSubjectChoice->Disable();
    m_threshCondChoice->Disable();
    m_threshRowText->Disable();
    m_threshColText->Disable();
  }
}


void SampleWindow::onRandomRowChecked(
    wxCommandEvent&)
{
  if (m_randRowCheck->GetValue()) {
    m_randRowText->Enable();
  } else {
    m_randRowText->Disable();
  }
}


void SampleWindow::onRandomColChecked(
    wxCommandEvent&)
{
  if (m_randColCheck->GetValue()) {
    m_randColText->Enable();
  } else {
    m_randColText->Disable();
  }
}


void SampleWindow::onThresholdSubjectSelected(
    wxCommandEvent&)
{
  if (m_threshSubjectChoice->GetSelection() == ROWS) {
    m_threshRowText->Show(true);
    m_threshColText->Show(false);

    m_threshRowText->SetPosition(m_threshColText->GetPosition());
  } else {
    m_threshRowText->Show(false);
    m_threshColText->Show(true);

    m_threshColText->SetPosition(m_threshRowText->GetPosition());
  }
}


void SampleWindow::onRandomSelected(
    wxCommandEvent&)
{
  activateRandom(true);
  activateThreshold(false);

  m_options.type = RANDOM;
}


void SampleWindow::onThresholdSelected(
    wxCommandEvent&)
{
  activateRandom(false);
  activateThreshold(true);

  m_options.type = THRESHOLD;
}


void SampleWindow::onOK(
    wxCommandEvent&)
{
  // get matrix info
  CSRMatrix const * const mat = \
      dynamic_cast<const CSRMatrix*>(m_storage->getMatrix());
  ASSERT_NOTNULL(mat);

  dim_type const numRows = mat->getNumRows();
  dim_type const numCols = mat->getNumColumns();

  // attempt to validate input
  if (!this->Validate() || !this->TransferDataFromWindow()) {
    // TODO: find out what failed and display a message
    return;
  }

  // set reorder struct properties
  if (IsModal()) {
    EndDialog(wxID_OK);
  } else {
    SetReturnCode(wxID_OK);
    Show(false);
  }

  switch (m_options.type) {
    case RANDOM:
      if (m_randRowCheck->GetValue()) {
        m_options.numRandRows = m_randNumRows; 
      } else {
        m_options.numRandRows = numRows; 
      }
      if (m_randColCheck->GetValue()) {
        m_options.numRandCols = m_randNumCols; 
      } else {
        m_options.numRandCols = numCols; 
      }
      break;
    case THRESHOLD:
      m_options.threshType = m_threshSubjectChoice->GetSelection();
      if (m_options.threshType == ROWS) {
        if (m_threshCondChoice->GetSelection() == MORE) {
          m_options.minSize = m_threshNNZRows;
          m_options.maxSize = numCols;
        } else {
          m_options.minSize = 0;
          m_options.maxSize = m_threshNNZRows;
        }
      } else {
        if (m_threshCondChoice->GetSelection() == MORE) {
          m_options.minSize = m_threshNNZCols;
          m_options.maxSize = numRows;
        } else {
          m_options.minSize = 0;
          m_options.maxSize = m_threshNNZCols;
        }
      }
      break;
    default:
      assert(false);
  }
}


void SampleWindow::onCancel(
    wxCommandEvent&)
{
  if ( IsModal() ) {
    EndDialog(wxID_CANCEL);
  } else {
    SetReturnCode(wxID_CANCEL);
    Show(false);
  } 
}






}
