#pragma once

#include <QMainWindow>

#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    enum Operator
    {
        None,
        Plus,
        Minus,
        Multiply,
        Divide
    };

    double m_lastValue = 0;
    Operator m_lastOperator = None;

    QLineEdit *m_lineEdit;

    bool m_newNum = true;

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

private:
    void setLineText(QString text);

    void calcuResult();

    void pressClear();

    void pressBack();

    void pressReciprocal();

    void pressSquare();

    void pressDSquare();

    void pressOperator(Operator op);

    void pressNumber(int n);

    void pressNegate();

    void pressDot();

    void pressEqual();
};