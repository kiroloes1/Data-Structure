#include "mainwindow.h"

#include "../Logic/XmlEditorManager.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(900, 600);
    this->setWindowTitle("XML Editor - CSE331 Team Project");

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *fileLayout = new QHBoxLayout();
    QHBoxLayout *algoLayout = new QHBoxLayout();

    textEdit = new QTextEdit();
    textEdit->setPlaceholderText("Output will appear here...");
    textEdit->setReadOnly(false);

    statusLabel = new QLabel("Ready");

    btnBrowse = new QPushButton("📂 Browse");
    btnSave = new QPushButton("💾 Save As");

    btnVerify = new QPushButton("Verify");
    btnFormat = new QPushButton("Format");
    btnJson = new QPushButton("To JSON");
    btnMini = new QPushButton("Minify");
    btnCompress = new QPushButton("Compress");
    btnDecompress = new QPushButton("Decompress");
    xmlManager = new XmlEditorManager();

    fileLayout->addWidget(btnBrowse);
    fileLayout->addWidget(btnSave);
    fileLayout->addStretch();

    algoLayout->addWidget(btnVerify);
    algoLayout->addWidget(btnFormat);
    algoLayout->addWidget(btnJson);
    algoLayout->addWidget(btnMini);
    algoLayout->addWidget(btnCompress);
    algoLayout->addWidget(btnDecompress);

    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(algoLayout);
    mainLayout->addWidget(textEdit);
    mainLayout->addWidget(statusLabel);

    connect(btnBrowse, &QPushButton::clicked, this, &MainWindow::handleBrowse);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::handleSave);

    connect(btnVerify, &QPushButton::clicked, this, &MainWindow::handleVerify);
    connect(btnFormat, &QPushButton::clicked, this, &MainWindow::handleFormat);
    connect(btnJson, &QPushButton::clicked, this, &MainWindow::handleJson);
    connect(btnMini, &QPushButton::clicked, this, &MainWindow::handleMinify);
    connect(btnCompress, &QPushButton::clicked, this, &MainWindow::handleCompress);
    connect(btnDecompress, &QPushButton::clicked, this, &MainWindow::handleDecompress);
}

MainWindow::~MainWindow() {
    delete xmlManager;
}



void MainWindow::handleBrowse() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open XML File", "", "XML Files (*.xml);;All Files (*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();

            textEdit->setText(content);
            currentFilePath = fileName;
            statusLabel->setText("Loaded: " + fileName);
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Could not open file!");
        }
    }
}

void MainWindow::handleSave() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "XML Files (*.xml);;Text Files (*.txt)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();

        statusLabel->setText("File Saved: " + fileName);
        QMessageBox::information(this, "Success", "File saved successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Could not save file!");
    }
}



void MainWindow::handleVerify() {
    QString qContent = textEdit->toPlainText();

    if (qContent.isEmpty()) {
        QMessageBox::warning(this, "Warning", "The text area is empty. Please load a file or paste XML first.");
        return;
    }

    std::string content = qContent.toStdString();
    std::string errorLog;


    bool isValid = xmlManager->verifyXml(content, errorLog);

    if (isValid) {
        QMessageBox::information(this, "Verification Result", "✅ The XML file is consistent (Valid).");
        statusLabel->setText("Status: Valid XML");
    } else {
        statusLabel->setText("Status: Invalid XML - Errors Found");

        QString errorMsg = QString("❌ Errors found:\n\n%1").arg(QString::fromStdString(errorLog));

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Validation Failed",
                                      errorMsg + "\n\nDo you want to attempt to fix these errors automatically?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            std::string fixedContent = xmlManager->fixXml(content);

            textEdit->setText(QString::fromStdString(fixedContent));
            QMessageBox::information(this, "Success", "The XML has been repaired!");
            statusLabel->setText("Status: XML Repaired");
        }
    }
}


void MainWindow::handleFormat() {
    QMessageBox::information(this, "Info", "Format feature coming soon!");
}

void MainWindow::handleJson() {
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