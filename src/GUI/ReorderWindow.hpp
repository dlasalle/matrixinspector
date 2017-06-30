/**
 * @file ReorderWindow.hpp
 * @brief The ReorderWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_GUI_REORDERWINDOW_HPP
#define MATRIXINSPECTOR_GUI_REORDERWINDOW_HPP




#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/valnum.h>
#include <vector>
#include "Types.hpp"
#include "Data/DataStorage.hpp"




namespace MatrixInspector
{


class ReorderWindow :
  public wxDialog
{
  public:
    enum reorder_type {
      RANDOM,
      DENSITY,
      FILE
    };

    struct reorder_struct {
      // generic paramters
      int type;
      bool orderRows;
      bool orderColumns;
      bool symmetric;
      // random parameters
      uint32_t seed;
      // density parameters
      bool descending;
      // file parameters
      std::vector<value_type> rowKeys;
      std::vector<value_type> colKeys;
    };


    ReorderWindow(
        wxFrame * parent,
        DataStorage * storage);


    virtual ~ReorderWindow();


    reorder_struct getOptions() const;


  private:
    DataStorage * m_storage;
    reorder_struct m_options;
    unsigned int m_seed;
    std::string m_rowFile;
    std::string m_colFile;

    // random controls
    wxRadioButton * m_randomRadio;
    wxChoice * m_randomChoice;
    wxTextCtrl * m_seedText;
    wxCheckBox * m_randomSym;

    // density controls
    wxRadioButton * m_densityRadio;
    wxChoice * m_densityChoice;
    wxChoice * m_descendingChoice;

    // file controls
    wxRadioButton * m_fileRadio;
    wxButton * m_rowFileButton;
    wxTextCtrl * m_rowFileText;
    wxCheckBox * m_fileSym;
    wxButton * m_colFileButton;
    wxTextCtrl * m_colFileText;


    void setSubjectChoice(
        int choice);


    void activateRandom(
        bool active);


    void activateDensity(
        bool active);


    void activateFile(
        bool active);


    bool getPermutationFile(
        std::string * filename);


    void onRandomSelected(
        wxCommandEvent& event);


    void onDensitySelected(
        wxCommandEvent& event);


    void onFileSelected(
        wxCommandEvent& event);


    void onRowFileButton(
        wxCommandEvent& event);


    void onFileSymCheck(
        wxCommandEvent&);


    void onColFileButton(
        wxCommandEvent& event);


    void onOK(
        wxCommandEvent& event);


    void onCancel(
        wxCommandEvent& event);


    wxDECLARE_EVENT_TABLE();


    // prevent copying
    ReorderWindow(
        ReorderWindow const & rhs);
    ReorderWindow& operator=(
        ReorderWindow const & rhs);

};




}






#endif
