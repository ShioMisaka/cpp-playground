#include "MenuBar.h"

#include <QMenu>
#include <QDebug>

#include <QFile>

MenuBar::MenuBar(QWidget* parent)
    : QMenuBar(parent)
{
    QMenu *fileM = new QMenu("File");

    QAction *action = fileM->addAction("New");
    connect(action, &QAction::triggered, this, [this](){ Q_EMIT new_open_signal(); });
    action = fileM->addAction("Open");
    connect(action, &QAction::triggered, this, [this](){ Q_EMIT open_file_signal(); });
    action = fileM->addAction("Save");
    connect(action, &QAction::triggered, this, [this](){ Q_EMIT save_file_signal(); });
    action = fileM->addAction("Save As");
    connect(action, &QAction::triggered, this, [this](){ Q_EMIT saveAs_file_signal(); });

    addMenu(fileM);
}

MenuBar::~MenuBar()
{

}