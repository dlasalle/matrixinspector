/**
 * @file MainWindow.hpp
 * @brief The MainWindow class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_GUI_MAINWINDOW_HPP
#define MATRIXINSPECTOR_GUI_MAINWINDOW_HPP




#include <functional>
#include <wx/wx.h>
#include "Data/DataStorage.hpp"
#include "View/View.hpp"




namespace MatrixInspector
{


class MainWindow :
  public wxFrame
{
  public:
    /**
    * @brief Create a new main window.
    *
    * @param title The title of the window.
    * @param x The x-offset of the window.
    * @param y The y-offest of the window.
    * @param width The width of the window.
    * @param height The height of the window.
    */
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


    /**
    * @brief Update the size of the matrix info.
    */
    void updateMatrixSize();


    /**
    * @brief Run a task using a progress bar.
    *
    * @param title The title of the task.
    * @param msg The task message.
    * @param job The task to execute.
    */
    void runTaskProgress(
        std::string title,
        std::string msg,
        std::function<void (double * progress)> job);


    /**
    * @brief Save the matrix to the given filename.
    *
    * @param name The filename.
    */
    void save(
        std::string name);


/* FILE **********************************************************************/


    /**
    * @brief Handle the 'exit' event.
    *
    * @param event The event.
    */
    void onExit(
        wxCommandEvent& event);


    /**
    * @brief Handle the 'open' event.
    *
    * @param event The event.
    */
    void onOpen(
        wxCommandEvent& event);


    /**
    * @brief Handle the 'save' event.
    *
    * @param event The event.
    */
    void onSave(
        wxCommandEvent& event);


    /**
    * @brief Handle the 'save as' event.
    *
    * @param event The event.
    */
    void onSaveAs(
        wxCommandEvent& event);


/* EDIT **********************************************************************/


    /**
    * @brief Handle the 'transpose' event.
    *
    * @param event The event.
    */
    void onTranspose(
        wxCommandEvent& event);


    /**
    * @brief Handle the 'reorder' event.
    *
    * @param event The event.
    */
    void onReorder(
        wxCommandEvent& event);


    /**
    * @brief Handle the 'sample' event.
    *
    * @param event The event.
    */
    void onSample(
        wxCommandEvent& event);



/* ANALYZE *******************************************************************/

    /**
    * @brief Handle the 'stats' event.
    *
    * @param event The event.
    */
    void onStats(
        wxCommandEvent& event);


    wxDECLARE_EVENT_TABLE();

    // disable copying
    MainWindow(
        MainWindow const & rhs) = delete;
    MainWindow& operator=(
        MainWindow const & rhs) = delete;

};




}




#endif
