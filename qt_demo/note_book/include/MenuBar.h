#pragma once

#include <QMenuBar>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    MenuBar(QWidget *parent = nullptr);

    ~MenuBar();

Q_SIGNALS:
    void new_open_signal();

    void open_file_signal();

    void save_file_signal();

    void saveAs_file_signal();
};