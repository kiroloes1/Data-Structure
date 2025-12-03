//
// Created by Nouran Atef on 01/12/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QLabel>
#include "../Logic/XmlEditorManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File Operations
    void handleBrowse();
    void handleSave();

    // Level 1 Algorithms
    void handleVerify();
    void handleFormat();
    void handleJson();
    void handleMinify();
    void handleCompress();
    void handleDecompress();

private:
    QString currentFilePath;

    XmlEditorManager *xmlManager;


    QWidget *centralWidget;
    QTextEdit *textEdit;
    QLabel *statusLabel;


    QPushButton *btnBrowse;
    QPushButton *btnSave;
    QPushButton *btnVerify;
    QPushButton *btnFormat;
    QPushButton *btnJson;
    QPushButton *btnMini;
    QPushButton *btnCompress;
    QPushButton *btnDecompress;
};
#endif

