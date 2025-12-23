//
// Created by Nouran Atef on 01/12/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../Logic/XmlEditorManager.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  // File Operations
  void handleBrowse();
  void handleSave();

  // Level 1 Algorithms
  void handleValidate();
  void handleFormat();
  void handleJson();
  void handleMinify();
  void handleCompress();
  void handleDecompress();
  void handleDrawGraph();

private:
  QString currentFilePath;

  XmlEditorManager *xmlManager;
  QTextEdit *inputTextEdit;
  QTextEdit *outputTextEdit;

  QWidget *centralWidget;
  QTextEdit *textEdit;
  QLabel *statusLabel;

  QPushButton *btnBrowse;
  QPushButton *btnSave;
  QPushButton *btnValidate;
  QPushButton *btnFormat;
  QPushButton *btnJson;
  QPushButton *btnMini;
  QPushButton *btnCompress;
  QPushButton *btnDecompress;

  QPushButton *btnDraw;
};

#endif
