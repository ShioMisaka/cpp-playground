#include "MainWindow.h"

#include <QDebug>
#include <QVBoxLayout>
#include <MenuBar.h>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include <string>

MainWindow::MainWindow(QWidget* parent)
 : QMainWindow(parent), m_currentFilePath("")
{
    setWindowTitle("NoteBook");
    setGeometry(1000, 500, 500, 500);

    MenuBar *menuBar = new MenuBar;
    setMenuBar(menuBar);

    m_textEdit = new QTextEdit;
    setCentralWidget(m_textEdit);
    connect(menuBar, &MenuBar::new_open_signal, this, &MainWindow::new_file);
    connect(menuBar, &MenuBar::open_file_signal, this, &MainWindow::open_file);
    connect(menuBar, &MenuBar::save_file_signal, this, &MainWindow::save_file);
    connect(menuBar, &MenuBar::saveAs_file_signal, this, &MainWindow::saveAs_file);

    connect(m_textEdit, &QTextEdit::textChanged, this, [this]() { m_isModify = true; });
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_isModify && !AskCheckout()) {
        event->ignore();
        return;
    }
    event->accept();
}


void MainWindow::new_file()
{
    if (m_isModify && !AskCheckout()) {
        return;
    }
    m_textEdit->clear();
    m_currentFilePath.clear();
    m_isModify = false;
}

void MainWindow::open_file()
{
    if (m_isModify && !AskCheckout()) {
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(this, "Open file", "", "text(*.txt *.log);;All(*)");
    QFile file(filePath);

    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Can't read file: " << filePath;
        return;
    }

    m_currentFilePath = filePath;
    QString text = QString::fromUtf8(file.readAll());
    m_textEdit->setPlainText(text);
    m_isModify = false;
}

void MainWindow::save_file()
{
    if (m_currentFilePath.isEmpty()) {
        saveAs_file();
    }
    else {
        saveFromFilePath(m_currentFilePath);
    }
}

void MainWindow::saveAs_file()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Save as", "", "text(*.txt *.log);;All(*)");
    if (filePath.isEmpty()) {
        return;
    }
    saveFromFilePath(filePath);
}

void MainWindow::saveFromFilePath(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QFile::WriteOnly)) {
        qWarning() << "Can't read file: " << filePath;
        return;
    }

    QString text = m_textEdit->toPlainText();
    file.write(text.toUtf8());
    qInfo() << "Save file: " << filePath;
    m_isModify = false;
}

bool MainWindow::AskCheckout()
{
    QMessageBox msgBox(QMessageBox::Question, "Note book",
    "Has modified file, is save?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    bool ret = true;
    auto choose = msgBox.exec();
    switch(choose) {
        case QMessageBox::Save:
            save_file();
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            ret = false;
            break;
    }
    return ret;
}