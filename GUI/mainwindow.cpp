#include "mainwindow.h"
#include <QFileDialog>
#include <QWidget>

#include "mainwindow.h"
#include "../Logic/XmlEditorManager.h" // Connects to your Facade
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 1. Window Setup
    this->resize(900, 600);
    this->setWindowTitle("XML Editor - CSE331 Team Project");

    // 2. Setup Central Widget & Layouts
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *fileLayout = new QHBoxLayout();      // Top row
    QHBoxLayout *algoLayout = new QHBoxLayout();      // Middle row

    // 3. Create UI Elements
    textEdit = new QTextEdit();
    textEdit->setPlaceholderText("Output will appear here...");
    textEdit->setReadOnly(true); // Requirement: "Read-only text field"

    statusLabel = new QLabel("Ready");

    // 4. Create Buttons
    btnBrowse = new QPushButton("📂 Browse");
    btnSave = new QPushButton("💾 Save As");

    btnVerify = new QPushButton("Verify");
    btnFormat = new QPushButton("Format");
    btnJson = new QPushButton("To JSON");
    btnMini = new QPushButton("Minify");
    btnCompress = new QPushButton("Compress");
    btnDecompress = new QPushButton("Decompress");

    // 5. Arrange Layouts
    // -- Top Row (File Ops) --
    fileLayout->addWidget(btnBrowse);
    fileLayout->addWidget(btnSave);
    fileLayout->addStretch(); // Push buttons to the left

    // -- Middle Row (Algorithms) --
    algoLayout->addWidget(btnVerify);
    algoLayout->addWidget(btnFormat);
    algoLayout->addWidget(btnJson);
    algoLayout->addWidget(btnMini);
    algoLayout->addWidget(btnCompress);
    algoLayout->addWidget(btnDecompress);

    // -- Add to Main Layout --
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(algoLayout);
    mainLayout->addWidget(textEdit);
    mainLayout->addWidget(statusLabel);

    // 6. Connect Buttons to Functions
    connect(btnBrowse, &QPushButton::clicked, this, &MainWindow::handleBrowse);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::handleSave);

    // Connect Algorithm Buttons
    connect(btnVerify, &QPushButton::clicked, this, &MainWindow::handleVerify);
    connect(btnFormat, &QPushButton::clicked, this, &MainWindow::handleFormat);
    connect(btnJson, &QPushButton::clicked, this, &MainWindow::handleJson);
    connect(btnMini, &QPushButton::clicked, this, &MainWindow::handleMinify);
    connect(btnCompress, &QPushButton::clicked, this, &MainWindow::handleCompress);
    connect(btnDecompress, &QPushButton::clicked, this, &MainWindow::handleDecompress);
}

MainWindow::~MainWindow() {}

// ----------------------
// File Implementation
// ----------------------

void MainWindow::handleBrowse() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open XML File", "", "XML Files (*.xml)");

    if (!fileName.isEmpty()) {
        currentFilePath = fileName;
        statusLabel->setText("Loaded: " + fileName);

        // Use Manager to read file
        // XmlEditorManager manager;
        // std::string content = manager.readFile(fileName.toStdString());
        // textEdit->setText(QString::fromStdString(content));
    }
}

void MainWindow::handleSave() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;XML Files (*.xml)");
    if (fileName.isEmpty()) return;

    // Logic to save the content of textEdit to file
    // For now, simple placeholder:
    statusLabel->setText("File Saved (Placeholder)");
}

// ----------------------
// Algorithm Implementation
// ----------------------

void MainWindow::handleVerify() {

}

// --- Placeholders for future tasks ---

void MainWindow::handleFormat() {
    // Member C will implement this later using manager.format()
    QMessageBox::information(this, "Info", "Format feature coming soon!");
}

void MainWindow::handleJson() {
    // Member B will implement this later
    QMessageBox::information(this, "Info", "JSON feature coming soon!");
}

void MainWindow::handleMinify() {
    QMessageBox::information(this, "Info", "Minify feature coming soon!");
}

void MainWindow::handleCompress() {
    QMessageBox::information(this, "Info", "Compression feature coming soon!");
}

void MainWindow::handleDecompress() {
    QMessageBox::information(this, "Info", "Decompression feature coming soon!");
}