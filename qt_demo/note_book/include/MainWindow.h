#pragma once

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
private:
    QTextEdit *m_textEdit;

    QString m_currentFilePath;

    bool m_isModify = false;

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* envent) override;

private Q_SLOTS:
    void new_file();

    void open_file();

    void save_file();

    void saveAs_file();

private:
    void saveFromFilePath(const QString &filePath);

    bool AskCheckout();
};