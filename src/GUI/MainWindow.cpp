/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#include <exception>
#include <future>
#include <wx/progdlg.h>
#include "MainWindow.hpp"
#include "GUI/ReorderWindow.hpp"
#include "GUI/SampleWindow.hpp"
#include "GUI/StatsWindow.hpp"
#include "View/HeatMapView.hpp"
#include "Operations/Reorder.hpp"
#include "Operations/Sample.hpp"
#include "Utility/Debug.hpp"
#include "Data/CSRMatrix.hpp"




namespace MatrixInspector
{


/******************************************************************************
* TYPES ***********************************************************************
******************************************************************************/


namespace
{


enum event_types {
  // file
  ID_OPEN,
  ID_SAVE,
  ID_SAVEAS,
  // edit
  ID_TRANSPOSE,
  ID_REORDER,
  ID_SAMPLE,
  // analyze 
  ID_STATS,
  ID_DISTRIBUTION
};


}


/******************************************************************************
* CONSTANTS *******************************************************************
******************************************************************************/

namespace
{

const std::chrono::milliseconds WAIT_TIME(100);

const char * const SUPPORTED_TYPES_STRING = \
      "GRAPH / MATRIX (*.csr;*.graph;*.chaco;*.mtx;*.mm;*.snap)|" \
      "*.csr;*.graph;*.chaco;*.mtx;*.mm;*.snap";

}




/******************************************************************************
* MACROS **********************************************************************
******************************************************************************/

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
  // File
  EVT_MENU(ID_OPEN, MainWindow::onOpen)
  EVT_MENU(ID_SAVE, MainWindow::onSave)
  EVT_MENU(ID_SAVEAS, MainWindow::onSaveAs)
  EVT_MENU(wxID_EXIT, MainWindow::onExit)
  // Edit
  EVT_MENU(ID_TRANSPOSE, MainWindow::onTranspose)
  EVT_MENU(ID_REORDER, MainWindow::onReorder)
  EVT_MENU(ID_SAMPLE, MainWindow::onSample)
  // Analyze
  EVT_MENU(ID_STATS, MainWindow::onStats)
wxEND_EVENT_TABLE()




/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/


MainWindow::MainWindow(
    char const * const title,
    int const x,
    int const y,
    int const width,
    int const height) :
  wxFrame(NULL, wxID_ANY, title, wxPoint(x,y), wxSize(width,height)),
  m_storage(),
  m_menuBar(nullptr),
  m_menuFile(nullptr),
  m_menuEdit(nullptr),
  m_menuAnalyze(nullptr),
  m_currentPath(),
  m_view(new HeatMapView(this))
{
  m_menuFile = new wxMenu;
	m_menuFile->Append(ID_OPEN, "&Open...\tCtrl-O", "Open a matrix.");
	m_menuFile->Append(ID_SAVE, "&Save...\tCtrl-S", "Save the current matrix.");
  m_menuFile->Enable(ID_SAVE,false);
	m_menuFile->Append(ID_SAVEAS, "Save As...", "Save the current matrix.");
  m_menuFile->Enable(ID_SAVEAS,false);
  m_menuFile->Append(wxID_EXIT);

  m_menuEdit = new wxMenu;
  m_menuEdit->Append(ID_TRANSPOSE, "Transpose", "Transpose the matrix.");
  m_menuEdit->Append(ID_REORDER, "Reorder", "Reorder the matrix.");
  m_menuEdit->Append(ID_SAMPLE, "Sample", "Sample the matrix.");

  m_menuAnalyze = new wxMenu;
  m_menuAnalyze->Append(ID_STATS, "Statistics", \
      "View the statistics of the matrix.");
  /*
  m_menuAnalyze->Append(ID_DISTRIBUTION, "Distribution", \
      "View the distribution of the matrix.");
  */

  m_menuBar = new wxMenuBar;
  m_menuBar->Append( m_menuFile, "&File" );
  m_menuBar->Append( m_menuEdit, "&Edit" );
  m_menuBar->EnableTop(1,false);
  m_menuBar->Append( m_menuAnalyze, "&Analyze" );
  m_menuBar->EnableTop(2,false);
  SetMenuBar( m_menuBar );

  CreateStatusBar(3);
}



/******************************************************************************
* PUBLIC FUNCTIONS ************************************************************
******************************************************************************/


void MainWindow::load(
    std::string const name)
{
  try {
    runTaskProgress("Loading", \
        std::string("Opening ") + name + std::string(" ..."),
        [&](double * const done) {
          m_storage.loadDataset(name.c_str(), done);
        });

    updateMatrixSize();

    Matrix const * const mat = m_storage.getMatrix();
    m_view->setMatrix(mat);

    m_view->render();

    // set current path
    m_currentPath = name;

    // re-enable things
    m_menuBar->EnableTop(1,true);
    m_menuBar->EnableTop(2,true);
    m_menuBar->Enable(ID_SAVE,true);
    m_menuBar->Enable(ID_SAVEAS,true);
  } catch (std::bad_alloc const & e) {
    wxMessageDialog msg(this, \
        "Not enough memory to load matrix.", "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
    m_currentPath = std::string("");
  } catch (std::exception const & e) {
    wxMessageDialog msg(this,std::string("Error: ") + e.what(), "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
    m_currentPath = std::string("");
  }
}

void MainWindow::save(
    std::string const name)
{
  try {
    runTaskProgress("Saving",std::string("Saving ") + name,
        [&](double * done){m_storage.saveDataset(name.c_str(),done);});
  } catch (std::exception const & e) {
    wxMessageDialog errMsg(this,e.what(),"Error",wxOK|wxICON_ERROR);

    errMsg.ShowModal();
  }
}


/******************************************************************************
* PRIVATE FUNCTIONS ***********************************************************
******************************************************************************/

void MainWindow::updateMatrixSize()
{
  Matrix const * mat = m_storage.getMatrix();

  std::string msg( \
      std::to_string(mat->getNumRows()) + std::string(" x ") + \
      std::to_string(mat->getNumColumns()));

  CSRMatrix const * csr;
  if ((csr = dynamic_cast<CSRMatrix const *>(mat)) != nullptr) {
    msg += std::string(" and ") + std::to_string(csr->getNumNonZeros()) + \
      std::string(" non-zeros");
  }

  SetStatusText( msg, 1 );
}

void MainWindow::runTaskProgress(
    std::string title,
    std::string msg,
    std::function<void (double *)> job)
{
  std::unique_ptr<wxProgressDialog> diag( \
      new wxProgressDialog(title, msg));

  double done = 0.0;
  std::future<void> task = std::async(job,&done);

  do {
    int per = std::min(static_cast<int>(done * 100.0),100);
    diag->Update(per);
  } while (task.wait_for(WAIT_TIME) == std::future_status::timeout);

  diag->Update(100);

  task.get();
}


void MainWindow::onExit(
    wxCommandEvent&)
{
  Close( true );
}


void MainWindow::onOpen(
    wxCommandEvent&)
{
	wxFileDialog openFileDialog(this, _("Open Matrix/Graph"), "", "", \
      SUPPORTED_TYPES_STRING, \
			wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		// user canceled
		return;
	}
	
  std::string const filename(openFileDialog.GetPath().mb_str());

  load(filename);
}


void MainWindow::onSave(
    wxCommandEvent&)
{
  std::string const msg(std::string("Overwrite '") + m_currentPath + \
      std::string("'"));
  wxMessageDialog confirmMsg(this,msg.c_str(),"Confirm", \
      wxYES_NO|wxICON_WARNING);

  if (confirmMsg.ShowModal() != wxID_YES) {
    // do nothing
    return;
  }

  save(m_currentPath);
}


void MainWindow::onSaveAs(
    wxCommandEvent&)
{
  wxFileDialog saveFileDialog(this, _("Save Matrix/Graph"), "", "", \
      SUPPORTED_TYPES_STRING, \
      wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		// user canceled
		return;
	}

  std::string const name(saveFileDialog.GetPath().mb_str());

  save(name);
}


void MainWindow::onTranspose(
    wxCommandEvent&)
{
  Matrix * const mat = m_storage.getMatrix();

  ASSERT_NOTNULL(mat);

  std::unique_ptr<wxProgressDialog> progress( \
      new wxProgressDialog("Transposing", "Transposing matrix..."));


  try {
    runTaskProgress("Transposing","Transposing matrix...",
        [&](double * done) { mat->transpose(done); });

    if (m_view) {
      m_view->refresh();
    }
  } catch (std::bad_alloc const & e) {
    wxMessageDialog msg(this, \
        "Not enough memory to perform transpose operation.", "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
  } catch (std::exception const & e) {
    wxMessageDialog msg(this,std::string("Error: ") + e.what(), "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
  }

  updateMatrixSize();
}


void MainWindow::onReorder(
    wxCommandEvent&)
{
  Matrix * const mat = m_storage.getMatrix();
  ASSERT_NOTNULL(mat);

  ReorderWindow rw(this,&m_storage);

  rw.Show(true);

  if (rw.ShowModal() == wxID_CANCEL) {
    // user canceled
    return;
  }

  // perform re-order
  ReorderWindow::reorder_struct options = rw.getOptions();

  try {
    runTaskProgress("Reordering","Reordering matrix...",
        [&](double * done) {
          switch (options.type) {
            case ReorderWindow::RANDOM: {
              Reorder::random( \
                  mat,options.orderRows, \
                  options.orderColumns,options.seed,options.symmetric,done);
              }
              break;
            case ReorderWindow::DENSITY: {
              Reorder::density( \
                  mat,options.orderRows, \
                  options.orderColumns,options.descending,done);
              }
              break;
            case ReorderWindow::FILE: {
                value_type const * const rowPtr = \
                    options.rowKeys.size() == mat->getNumRows() ? \
                    options.rowKeys.data() : nullptr;
                value_type const * const colPtr = \
                    options.symmetric ? rowPtr : (
                    options.colKeys.size() == mat->getNumColumns() ? \
                    options.colKeys.data() : nullptr);
                Reorder::keys(mat,rowPtr,colPtr,true,done);
              }
              break;
          }});

    if (m_view) {
      m_view->refresh();
    }
  } catch (std::bad_alloc const & e) {
    wxMessageDialog msg(this, \
        "Not enough memory to perform reorder operation.", "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
  } catch (std::exception const & e) {
    wxMessageDialog msg(this,std::string("Error: ") + e.what(), "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
  }
}


void MainWindow::onSample(
    wxCommandEvent&)
{
  Matrix * const mat = m_storage.getMatrix();
  ASSERT_NOTNULL(mat);

  SampleWindow sw(this,&m_storage);

  sw.Show(true);

  if (sw.ShowModal() == wxID_CANCEL) {
    // user canceled
    return;
  }

  // perform sample
  SampleWindow::sample_struct options = sw.getOptions();

  try {
    runTaskProgress("Sampling","Sampling matrix...",
        [&](double * done) {
          switch (options.type) {
            case SampleWindow::RANDOM:
              Sample::random(mat, options.numRandRows, options.numRandCols, \
                  options.symmetric,done);
              break;
            case SampleWindow::THRESHOLD:
              if (options.threshType == SampleWindow::ROWS) {
                Sample::thresholdRows(mat, options.minSize,options.maxSize, \
                    done);
              } else {
                Sample::thresholdColumns(mat, options.minSize, \
                    options.maxSize,done);
              }
              break;
          }
        });

    updateMatrixSize();

    if (m_view) {
      m_view->refresh();
    }
  } catch (std::bad_alloc const & e) {
    wxMessageDialog msg(this, \
        "Not enough memory to perform sample operation.", "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
  } catch (std::exception const & e) {
    wxMessageDialog msg(this,std::string("Error: ") + e.what(), "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
  }
}


void MainWindow::onStats(
    wxCommandEvent&)
{
  Matrix * const mat = m_storage.getMatrix();

  try {
    if (!mat->isStatsSet()) {
      runTaskProgress("Statistics","Computing matrix statistics...",
          [&](double * done) {
            if (mat->isSymmetrySet()) {
              mat->computeSymmetry(done, 0.6);
              mat->computeStats(done, 0.4);
            } else {
              mat->computeStats(done, 1.0);
            }
          });
    }
  } catch (std::bad_alloc const & e) {
    wxMessageDialog msg(this, \
        "Not enough memory to perform stats operation.", "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
  } catch (std::exception const & e) {
    wxMessageDialog msg(this,std::string("Error: ") + e.what(), "", \
        wxOK|wxICON_ERROR);
    msg.ShowModal();
  }

  StatsWindow sw(this, &m_storage);

  sw.ShowModal();
}




}
