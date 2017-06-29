/**
 * @file MainWindow.hpp
 * @brief The MainWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_GUI_MAINWINDOW_HPP
#define MATRIXINSPECTOR_GUI_MAINWINDOW_HPP





#include <wx/wx.h>
#include "Data/DataStorage.hpp"
#include "View/View.hpp"




namespace MatrixInpsector
{


class MainWindow :
  public wxFrame
{
  public:
    MainWindow(
        char const * title,
        int x,
        int y,
        int width,
        int height);


  private:
    DataStorage m_storage;
    // gui elements
    wxMenuBar * m_menuBar;
    wxMenu * m_menuFile;
    wxMenu * m_menuEdit;
    wxMenu * m_menuAnalyze;
    std::string m_currentPath;
    std::unique_ptr<View> m_view;


    void updateMatrixSize();


    void runTaskProgress(
        std::string title,
        std::string msg,
        std::function<void (double * progress)> job);


    void save(
        std::string name);


/* FILE **********************************************************************/


    void onExit(
        wxCommandEvent& event);


    void onOpen(
        wxCommandEvent& event);


    void onSave(
        wxCommandEvent& event);


    void onSaveAs(
        wxCommandEvent& event);


/* EDIT **********************************************************************/


    void onTranspose(
        wxCommandEvent& event);


    void onReorder(
        wxCommandEvent& event);


    void onSample(
        wxCommandEvent& event);



/* ANALYZE *******************************************************************/

    void onStats(
        wxCommandEvent& event);


    wxDECLARE_EVENT_TABLE();

    // disable copying
    MainWindow(
        MainWindow const & rhs);
    MainWindow& operator=(
        MainWindow const & rhs);

};




}




#endif
