#include "mainwindow.h"

#include "../Logic/XmlEditorManager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1000, 600);
    this->setWindowTitle("XML Editor - CSE331 Team Project");

    xmlManager = new XmlEditorManager();

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *fileLayout = new QHBoxLayout();
    QHBoxLayout *algoLayout = new QHBoxLayout();


    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    QWidget *inputContainer = new QWidget();
    QVBoxLayout *inputLayout = new QVBoxLayout(inputContainer);
    QLabel *inputLabel = new QLabel("<b>Input XML (Editable)</b>");
    inputTextEdit = new QTextEdit();
    inputTextEdit->setPlaceholderText("Type XML here or browse...");
    inputTextEdit->setReadOnly(false);
    inputLayout->addWidget(inputLabel);
    inputLayout->addWidget(inputTextEdit);

    QWidget *outputContainer = new QWidget();
    QVBoxLayout *outputLayout = new QVBoxLayout(outputContainer);
    QLabel *outputLabel = new QLabel("<b>Output (Read-Only)</b>");
    outputTextEdit = new QTextEdit();
    outputTextEdit->setPlaceholderText("Results will appear here...");
    outputTextEdit->setReadOnly(true);
    outputLayout->addWidget(outputLabel);
    outputLayout->addWidget(outputTextEdit);

    splitter->addWidget(inputContainer);
    splitter->addWidget(outputContainer);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    statusLabel = new QLabel("Ready");

    btnBrowse = new QPushButton("📂 Browse");
    btnSave = new QPushButton("💾 Save As");

    btnVerify = new QPushButton("Verify");
    btnFormat = new QPushButton("Format");
    btnJson = new QPushButton("To JSON");
    btnMini = new QPushButton("Minify");
    btnCompress = new QPushButton("Compress");
    btnDecompress = new QPushButton("Decompress");

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
    mainLayout->addWidget(splitter);
    mainLayout->addLayout(algoLayout);
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
            inputTextEdit->setText(in.readAll());
            currentFilePath = fileName;
            statusLabel->setText("Loaded: " + fileName);
            file.close();
        }
    }
}

void MainWindow::handleVerify() {
    QString qContent = inputTextEdit->toPlainText();
    if (qContent.isEmpty()) {
        QMessageBox::warning(this, "Warning", "The text area is empty. Please load a file or paste XML first.");
        return;
    }

    std::string content = qContent.toStdString();
    std::string errorLog;


    bool isValid = xmlManager->verifyXml(content, errorLog);

    if (isValid) {
        outputTextEdit->setText("✅ Valid XML");
        statusLabel->setText("Status: Valid");
        QMessageBox::information(this, "Valid", "The XML is consistent.");
    } else {
        outputTextEdit->setText(QString::fromStdString(errorLog));
        statusLabel->setText("Status: Invalid XML - Errors Found");

        QString errorMsg = QString("❌ Errors found:\n\n%1").arg(QString::fromStdString(errorLog));


        if (QMessageBox::question(this, "Fix?", "Errors found. Fix automatically?") == QMessageBox::Yes) {
             std::string fixed = xmlManager->fixXml(content);
             outputTextEdit->setText(QString::fromStdString(fixed));
             statusLabel->setText("Status: Fixed XML in Output");
        }
    }
}

void MainWindow::handleSave() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Output", "", "XML Files (*.xml)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << outputTextEdit->toPlainText();
        file.close();
        statusLabel->setText("Saved output to: " + fileName);
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