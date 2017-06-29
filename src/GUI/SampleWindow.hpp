/**
 * @file SampleWindow.hpp
 * @brief The SampleWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_GUI_SAMPLEWINDOW_HPP
#define MATRIXINSPECTOR_GUI_SAMPLEWINDOW_HPP





#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/valnum.h>
#include <vector>
#include "Data/DataStorage.hpp"



namespace MatrixInpsector
{


class SampleWindow :
  public wxDialog
{
  public:
    enum sample_type {
      RANDOM,
      THRESHOLD
    };


    enum threshold_type {
      ROWS = 0,
      COLUMNS = 1
    };


    struct sample_struct {
      int type;
      bool symmetric;
      // random
      dim_type numRandRows;
      dim_type numRandCols;
      // threshold
      int threshType;
      dim_type minSize;
      dim_type maxSize;
    };


    SampleWindow(
        wxFrame * parent,
        DataStorage * storage);


    virtual ~SampleWindow();


    sample_struct getOptions() const;


  private:
    sample_struct m_options;
    DataStorage * m_storage;

    // random controls
    wxRadioButton * m_randomRadio;
    wxCheckBox * m_randRowCheck;
    wxTextCtrl * m_randRowText;
    wxCheckBox * m_randColCheck;
    wxTextCtrl * m_randColText;
    dim_type m_randNumRows;
    dim_type m_randNumCols;

    // density controls
    wxRadioButton * m_thresholdRadio;
    wxChoice * m_threshSubjectChoice;
    wxChoice * m_threshCondChoice;
    wxTextCtrl * m_threshRowText;
    wxTextCtrl * m_threshColText;
    dim_type m_threshNNZRows;
    dim_type m_threshNNZCols;


    void activateRandom(
        bool active);

    
    void activateThreshold(
        bool active);


    void onRandomSelected(
        wxCommandEvent&);


    void onThresholdSelected(
        wxCommandEvent&);


    void onRandomRowChecked(
        wxCommandEvent&);


    void onRandomColChecked(
        wxCommandEvent&);


    void onThresholdSubjectSelected(
        wxCommandEvent&);


    void onOK(
        wxCommandEvent&);


    void onCancel(
        wxCommandEvent&);


    wxDECLARE_EVENT_TABLE();


    // prevent copying
    SampleWindow(
        SampleWindow const & rhs);
    SampleWindow& operator=(
        SampleWindow const & rhs);


};




}




#endif
