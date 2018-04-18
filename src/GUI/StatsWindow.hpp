/**
 * @file StatsWindow.hpp
 * @brief The StatsWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_GUI_STATSWINDOW_HPP
#define MATRIXINSPECTOR_GUI_STATSWINDOW_HPP



#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/valnum.h>
#include <vector>
#include "Data/DataStorage.hpp"




namespace MatrixInspector
{


class StatsWindow :
  public wxDialog
{
  public:
    StatsWindow(
        wxFrame * parent,
        DataStorage * storage);


    virtual ~StatsWindow();


  private:
    DataStorage * m_storage;


    wxDECLARE_EVENT_TABLE();


    void addRow(
        wxBoxSizer * topSizer,
        std::string key,
        std::string value);


    void addRowInt(
        wxBoxSizer * topSizer,
        std::string key,
        index_type num);


    void addRowFloat(
        wxBoxSizer * topSizer,
        std::string key,
        double num);


    void onOK(
        wxCommandEvent& event);


    // prevent copying
    StatsWindow(
        StatsWindow const & rhs);
    StatsWindow& operator=(
        StatsWindow const & rhs);




};




}




#endif
