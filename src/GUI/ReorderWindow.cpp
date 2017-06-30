/**
 * @file ReorderWindow.cpp
 * @brief Implementation of the ReorderWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include "ReorderWindow.hpp"
#include "GUI/WindowProperties.hpp"
#include "Utility/Random.hpp"




namespace MatrixInspector
{


/******************************************************************************
* TYPES ***********************************************************************
******************************************************************************/


namespace
{


enum event_types {
  ID_RANDOM,
  ID_RANDOM_CHOICE,
  ID_DENSITY,
  ID_DENSITY_CHOICE,
  ID_FILE,
  ID_ROW_FILE,
  ID_FILE_SYM,
  ID_COL_FILE
};


enum subject_types {
  ROWS_AND_COLUMNS = 0,
  ROWS = 1,
  COLUMNS = 2
};

enum descending_types {
  DESCENDING = 0,
  ASCENDING = 1
};


}




/******************************************************************************
* MACROS **********************************************************************
******************************************************************************/


wxBEGIN_EVENT_TABLE(ReorderWindow, wxDialog)
  EVT_RADIOBUTTON(ID_RANDOM, ReorderWindow::onRandomSelected)
  EVT_RADIOBUTTON(ID_DENSITY, ReorderWindow::onDensitySelected)
  EVT_RADIOBUTTON(ID_FILE, ReorderWindow::onFileSelected)
  EVT_BUTTON(ID_ROW_FILE, ReorderWindow::onRowFileButton)
  EVT_CHECKBOX(ID_FILE_SYM, ReorderWindow::onFileSymCheck)
  EVT_BUTTON(ID_COL_FILE, ReorderWindow::onColFileButton)
  EVT_BUTTON(wxID_OK, ReorderWindow::onOK)
  EVT_BUTTON(wxID_CANCEL, ReorderWindow::onCancel)
wxEND_EVENT_TABLE()





/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/


ReorderWindow::ReorderWindow(
    wxFrame * const parent,
    DataStorage * const storage) :
  wxDialog(parent, wxID_ANY, "Reorder", wxDefaultPosition, wxDefaultSize),
  m_storage(storage),
  m_options{RANDOM,true,true,false,0,true,{},{}},
  m_seed(0),
  m_rowFile(),
  m_colFile(),
  m_randomRadio(nullptr),
  m_randomChoice(nullptr),
  m_seedText(nullptr),
  m_randomSym(nullptr),
  m_densityRadio(nullptr),
  m_densityChoice(nullptr),
  m_descendingChoice(nullptr),
  m_fileRadio(nullptr),
  m_rowFileButton(nullptr),
  m_rowFileText(nullptr),
  m_fileSym(nullptr),
  m_colFileButton(nullptr),
  m_colFileText(nullptr)
{
  wxBoxSizer * allSizer = new wxBoxSizer(wxVERTICAL);

  wxArrayString rowColOpts;
  rowColOpts.Add("Rows and columns");
  rowColOpts.Add("Rows only");
  rowColOpts.Add("Columns only");

  wxArrayString descendingOpts;
  descendingOpts.Add("Descending");
  descendingOpts.Add("Ascending");

  // build three radio buttons together
  m_randomRadio = new wxRadioButton(this,ID_RANDOM,"Random");
  m_densityRadio = new wxRadioButton(this,ID_DENSITY,"By Density");
  m_fileRadio = new wxRadioButton(this,ID_FILE,"By File");

  // random permutation options
  wxBoxSizer * randSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer * horRandSizer1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer * horRandSizer2 = new wxBoxSizer(wxHORIZONTAL);
  m_randomChoice = new wxChoice(this,ID_RANDOM_CHOICE,wxDefaultPosition, \
      wxDefaultSize,rowColOpts);
  m_randomChoice->SetSelection(ROWS_AND_COLUMNS);
  m_randomChoice->Disable();

  // generate random number
  Random::setSeed();
  m_seed = Random::inRange(0U,UINT_MAX);
  wxIntegerValidator<unsigned int> val(&m_seed);
  m_seedText = new wxTextCtrl(this, wxID_ANY, std::to_string(m_seed), \
      wxDefaultPosition, wxDefaultSize, 0, val);
  m_seedText->Disable();
  m_randomSym = new wxCheckBox(this,wxID_ANY, \
      "Same permutation for rows and columns");
  m_randomSym->Disable();
  horRandSizer1->Add(m_randomChoice, 0, wxALIGN_RIGHT, BORDER);
  horRandSizer1->Add(new wxStaticText(this,wxID_ANY,"Seed"), 0, \
      wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER);
  horRandSizer1->Add(m_seedText, 0, wxALIGN_RIGHT, BORDER);
  horRandSizer2->Add(m_randomSym, 0, wxALIGN_RIGHT, BORDER);
  randSizer->Add(m_randomRadio, 0, wxALIGN_LEFT, BORDER);
  randSizer->Add(horRandSizer1);
  randSizer->Add(horRandSizer2);
  allSizer->Add(randSizer);
  allSizer->AddSpacer(20);

  // density based permutations
  wxBoxSizer * densitySizer = new wxBoxSizer(wxVERTICAL);
  densitySizer->Add(m_densityRadio, 0, wxALIGN_LEFT, BORDER);
  wxBoxSizer * horDensitySizer = new wxBoxSizer(wxHORIZONTAL);
  m_densityChoice = new wxChoice(this,ID_DENSITY_CHOICE,wxDefaultPosition, \
      wxDefaultSize,rowColOpts);
  m_densityChoice->SetSelection(ROWS_AND_COLUMNS);
  m_densityChoice->Disable();
  horDensitySizer->Add(m_densityChoice, 0, wxALIGN_RIGHT, BORDER);
  m_descendingChoice = new wxChoice(this,wxID_ANY,wxDefaultPosition, \
      wxDefaultSize, descendingOpts); 
  m_descendingChoice->SetSelection(DESCENDING);
  m_descendingChoice->Disable();
  horDensitySizer->Add(m_descendingChoice, 0, wxALIGN_RIGHT, BORDER);
  densitySizer->Add(horDensitySizer, 0, wxALIGN_RIGHT, BORDER);
  allSizer->Add(densitySizer);
  allSizer->AddSpacer(20);

  // file based permutations
  wxBoxSizer * fileSizer = new wxBoxSizer(wxVERTICAL);
  fileSizer->Add(m_fileRadio, 0, wxALIGN_LEFT, BORDER);
  wxBoxSizer * rowFileSizer = new wxBoxSizer(wxHORIZONTAL);
  m_rowFileButton = new wxButton(this, ID_ROW_FILE, "Browse");
  m_rowFileButton->Disable();
  m_rowFileText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, \
      wxDefaultSize, wxTE_READONLY);
  m_rowFileText->Disable();
  rowFileSizer->Add(new wxStaticText(this,wxID_ANY,"Row"), 0, \
      wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER);
  rowFileSizer->Add(m_rowFileButton, BORDER);
  rowFileSizer->Add(m_rowFileText, BORDER);
  fileSizer->Add(rowFileSizer, 0, wxALIGN_RIGHT, BORDER);
  m_fileSym = new wxCheckBox(this,ID_FILE_SYM, \
      "Same permutation for rows and columns");
  m_fileSym->Disable();
  fileSizer->Add(m_fileSym, BORDER);
  wxBoxSizer * colFileSizer = new wxBoxSizer(wxHORIZONTAL);
  m_colFileButton = new wxButton(this, ID_COL_FILE, "Browse");
  m_colFileButton->Disable();
  m_colFileText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, \
      wxDefaultSize, wxTE_READONLY);
  m_colFileText->Disable();
  colFileSizer->Add(new wxStaticText(this,wxID_ANY,"Column"), 0,
      wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER);
  colFileSizer->Add(m_colFileButton, BORDER);
  colFileSizer->Add(m_colFileText, BORDER);
  fileSizer->Add(colFileSizer, 0, wxALIGN_RIGHT, BORDER);

  allSizer->Add(fileSizer);
  allSizer->AddSpacer(20);

  // setup dialog buttons
  wxBoxSizer * bottomSizer = new wxBoxSizer(wxHORIZONTAL);
  bottomSizer->Add(new wxButton(this, wxID_OK, "Order"), BORDER);
  bottomSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), BORDER);
  allSizer->Add(bottomSizer,0,wxALIGN_RIGHT);

  SetSizerAndFit(allSizer);

  // select random radio button
  assert(m_randomRadio->GetValue());
  activateRandom(true);
}


ReorderWindow::~ReorderWindow()
{
  // do nothing
}




/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


ReorderWindow::reorder_struct ReorderWindow::getOptions() const
{
  return m_options;
}




/******************************************************************************
* PRIVATE FUNCTIONS ***********************************************************
******************************************************************************/


void ReorderWindow::setSubjectChoice(
    int const choice)
{
  switch (choice) {
    case ROWS_AND_COLUMNS:
      m_options.orderRows = true;
      m_options.orderColumns = true;
      break;
    case ROWS:
      m_options.orderRows = true;
      m_options.orderColumns = false;
      break;
    case COLUMNS:
      m_options.orderRows = false;
      m_options.orderColumns = true;
      break;
  }
}


void ReorderWindow::activateRandom(
    bool const active)
{
  if (active) {
    m_randomChoice->Enable();
    m_seedText->Enable();
    if (m_storage->getMatrix()->isSquare()) {
      m_randomSym->Enable();
    }
  } else {
    m_randomChoice->Disable();
    m_seedText->Disable();
    m_randomSym->Disable();
  }
}


void ReorderWindow::activateDensity(
    bool const active)
{
  if (active) {
    m_densityChoice->Enable();
    m_descendingChoice->Enable();
  } else {
    m_densityChoice->Disable();
    m_descendingChoice->Disable();
  }
}


void ReorderWindow::activateFile(
    bool const active)
{
  if (active) {
    m_rowFileButton->Enable();
    m_rowFileText->Enable();
    if (m_storage->getMatrix()->isSquare()) {
      m_fileSym->Enable();
    }
    if (!m_fileSym->GetValue()) {
      m_colFileButton->Enable();
      m_colFileText->Enable();
    }
  } else {
    m_rowFileButton->Disable();
    m_rowFileText->Disable();
    m_fileSym->Disable();
    m_colFileButton->Disable();
    m_colFileText->Disable();
  }
}


bool ReorderWindow::getPermutationFile(
    std::string * const filename)
{
  wxFileDialog fileDialog(this, _("Open permutation/clustering"), "", "", \
      "Permutation (*.perm;*.txt)|*.perm;*.txt|" \
      "Partition/Clustering (*.part;*.cluster)|*.part;*.cluster", \
			wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	if (fileDialog.ShowModal() == wxID_CANCEL) {
		// user canceled
		return false;
	} else {
    *filename = fileDialog.GetPath().ToStdString();

    return true;
  }
}


void ReorderWindow::onRandomSelected(
    wxCommandEvent&)
{
  activateRandom(true);
  activateDensity(false);
  activateFile(false);

  m_options.type = RANDOM;
}


void ReorderWindow::onDensitySelected(
    wxCommandEvent&)
{
  activateRandom(false);
  activateDensity(true);
  activateFile(false);

  m_options.type = DENSITY;
}


void ReorderWindow::onFileSelected(
    wxCommandEvent&)
{
  activateRandom(false);
  activateDensity(false);
  activateFile(true);

  m_options.type = FILE;
}


void ReorderWindow::onRowFileButton(
    wxCommandEvent&)
{
  if (getPermutationFile(&m_rowFile)) {
    // load vector
    try {
      m_options.rowKeys = DataStorage::loadVector(m_rowFile.c_str());

      // make sure it's the proper size
      dim_type const numRows = m_storage->getMatrix()->getNumRows();
      size_t const numValues = m_options.rowKeys.size();
      if (numValues != numRows) {
        throw std::runtime_error(std::string("Permutation file has ") + \
             std::to_string(numValues) + std::string(" values and matrix "
             "has ") + std::to_string(numRows) + std::string(" rows."));
      }

      m_rowFileText->ChangeValue(m_rowFile);
    } catch (const std::exception& e) {
      wxMessageDialog errMsg(this,e.what(),"Error",wxOK|wxICON_ERROR);
      errMsg.ShowModal();

      // empty the keys
      m_options.rowKeys.clear();
    }
  }
}


void ReorderWindow::onFileSymCheck(
    wxCommandEvent&)
{
  if (m_fileSym->GetValue()) {
    m_colFileButton->Disable();
    m_colFileText->Disable();
  } else {
    m_colFileButton->Enable();
    m_colFileText->Enable();
  }
}


void ReorderWindow::onColFileButton(
    wxCommandEvent&)
{
  if (getPermutationFile(&m_colFile)) {
    // load vector
    try {
      m_options.colKeys = DataStorage::loadVector(m_colFile.c_str());

      // make sure it's the proper size
      dim_type const numCols = m_storage->getMatrix()->getNumColumns();
      size_t const numValues = m_options.colKeys.size();
      if (numValues != numCols) {
        throw std::runtime_error(std::string("Permutation file has ") + \
             std::to_string(numValues) + std::string(" values and matrix "
             "has ") + std::to_string(numCols) + std::string(" columns."));
      }

      m_colFileText->ChangeValue(m_colFile);
    } catch (const std::exception& e) {
      wxMessageDialog errMsg(this,e.what(),"Error",wxOK|wxICON_ERROR);
      errMsg.ShowModal();

      // empty the keys
      m_options.colKeys.clear();
    }
  }
}


void ReorderWindow::onOK(
    wxCommandEvent&)
{
  // attempt to validate input
  if (!this->Validate() || !this->TransferDataFromWindow()) {
    return;
  }

  // set reorder struct properties
  if (IsModal()) {
    EndDialog(wxID_OK);
  } else {
    SetReturnCode(wxID_OK);
    Show(false);
  }

  // set options
  int choice;
  switch (m_options.type) {
    case RANDOM:
      choice = m_randomChoice->GetSelection();
      setSubjectChoice(choice);
      m_options.seed = m_seed;
      m_options.symmetric = m_randomSym->GetValue();
      break;
    case DENSITY:
      choice = m_densityChoice->GetSelection();
      setSubjectChoice(choice);
      m_options.descending = m_descendingChoice->GetSelection() == DESCENDING;
      break;
    case FILE:
      m_options.symmetric = m_fileSym->GetValue();
      break;
    default:
      assert(false);
  }
}


void ReorderWindow::onCancel(
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
