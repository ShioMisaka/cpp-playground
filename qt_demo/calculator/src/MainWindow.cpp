#include "MainWindow.h"

#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QValidator>
#include <QList>
#include <math.h>

MainWindow::MainWindow(QWidget* parent)
 : QMainWindow(parent)
{
    setWindowTitle("Calculator");
    setGeometry(1000, 500, 500, 500);
    setStyleSheet(
        "QPushButton {width: 70px; height: 70px; font: 20px; background-color: #d3d3d3;}"
        "QLineEdit { height: 70px; font: bold 52px; }");

    QWidget *mainWidget = new QWidget;
    setCentralWidget(mainWidget);

    QGridLayout *mainLayout = new QGridLayout;
    mainWidget->setLayout(mainLayout);

    m_lineEdit = new QLineEdit("0");
    m_lineEdit->setAlignment(Qt::AlignRight);
    m_lineEdit->setValidator(new QDoubleValidator);
    mainLayout->addWidget(m_lineEdit, 0, 0, 1, 4);

    QPushButton *clearBtn = new QPushButton("C");
    mainLayout->addWidget(clearBtn, 1, 2);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::pressClear);

    QPushButton *backBtn = new QPushButton("<");
    mainLayout->addWidget(backBtn, 1, 3);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::pressBack);

    QPushButton *reciprocalBtn = new QPushButton("1/x");
    mainLayout->addWidget(reciprocalBtn, 2, 0);
    connect(reciprocalBtn, &QPushButton::clicked, this, &MainWindow::pressReciprocal);

    QPushButton *sqBtn = new QPushButton("x²");
    mainLayout->addWidget(sqBtn, 2, 1);
    connect(sqBtn, &QPushButton::clicked, this, &MainWindow::pressSquare);

    QPushButton *dsqBtn = new QPushButton("√(x)");
    mainLayout->addWidget(dsqBtn, 2, 2);
    connect(dsqBtn, &QPushButton::clicked, this, &MainWindow::pressDSquare);

    for (size_t i = 1; i <= 9;++i) {
        QPushButton *numBtn = new QPushButton;
        numBtn->setText(QString::number(i));
        mainLayout->addWidget(numBtn, 5 - (i - 1) / 3, (i - 1) % 3);
        connect(numBtn, &QPushButton::clicked, this, [this, i]() { pressNumber(i); });
        numBtn->setStyleSheet("QPushButton { background-color: #ffffff; }");
    }

    QPushButton *zoreBtn = new QPushButton;
    zoreBtn->setText(QString::number(0));
    mainLayout->addWidget(zoreBtn, 6, 1);
    connect(zoreBtn, &QPushButton::clicked, this, [this]() { pressNumber(0); });
    zoreBtn->setStyleSheet("QPushButton { background-color: #ffffff; }");

    QPushButton *negateBtn = new QPushButton;
    negateBtn->setText("+/-");
    mainLayout->addWidget(negateBtn, 6, 0);
    connect(negateBtn, &QPushButton::clicked, this, &MainWindow::pressNegate);
    negateBtn->setStyleSheet("QPushButton { background-color: #ffffff; }");
    
    QPushButton *dotBtn = new QPushButton(".");
    mainLayout->addWidget(dotBtn, 6, 2);
    connect(dotBtn, &QPushButton::clicked, this, &MainWindow::pressDot);
    dotBtn->setStyleSheet("QPushButton { background-color: #ffffff; }");

    QPushButton *equalBtn = new QPushButton("=");
    equalBtn->setStyleSheet("QPushButton { background-color: #47b1e9; }");
    mainLayout->addWidget(equalBtn, 6, 3);
    connect(equalBtn, &QPushButton::clicked, this, &MainWindow::pressEqual);

    QList<QPair<Operator, QString>> operators = {
        {Plus, "+"},
        {Minus, "-"},
        {Multiply, "*"},
        {Divide, "/"}
    };

    for (int i = 0; const auto &[op, str] : operators) {
        QPushButton *opBtn = new QPushButton;
        opBtn->setText(str);
        mainLayout->addWidget(opBtn, i + 2, 3);
        connect(opBtn, &QPushButton::clicked, this, [this, op = op]() { pressOperator(op); });
        ++i;
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::setLineText(QString text)
{
    int pos;
    if (m_lineEdit->validator()->validate(text, pos) != QValidator::Invalid) {
        while (text.size() >= 2 && text[0] == '0' && (text[1] >= '1' && text[1] <= '9')) {
            text.remove(0, 1);
        }
        m_lineEdit->setText(text);
    }
}

void MainWindow::calcuResult()
{
    double result = 0;
    double new_value = m_lineEdit->text().toDouble();
    switch(m_lastOperator)
    {
    case Plus :
        result = m_lastValue + new_value;
        break;
    case Minus :
        result = m_lastValue - new_value;
        break; 
    case Multiply :
        result = m_lastValue * new_value;
        break; 
    case Divide :
        result = m_lastValue / new_value;
        break; 
    default:
        result = new_value;
    }

    setLineText(QString::number(result));
}

void MainWindow::pressBack()
{
    auto str = m_lineEdit->text();
    if (str.size() > 1) {
        str.remove(-1, 1);
    }
    else if (str != "0") {
        str = "0";    
    }
    m_lineEdit->setText(str);
}

void MainWindow::pressClear()
{
    m_lastOperator = None;
    m_lastValue = 0;
    m_lineEdit->setText("0");
}

void MainWindow::pressReciprocal()
{
    double num = m_lineEdit->text().toDouble();
    num = 1.0 / num;
    m_lineEdit->setText(QString::number(num));
}

void MainWindow::pressSquare()
{
    double num = m_lineEdit->text().toDouble();
    num = num * num;
    m_lineEdit->setText(QString::number(num));
}

void MainWindow::pressDSquare()
{
    double num = m_lineEdit->text().toDouble();
    num = std::sqrt(num);
    m_lineEdit->setText(QString::number(num));
}

void MainWindow::pressOperator(Operator op)
{
    m_newNum = true;
    if(m_lastOperator != None) {
        calcuResult();
    }
    m_lastValue = m_lineEdit->text().toDouble();
    m_lastOperator = op;
}

void MainWindow::pressNumber(int n) {
    QString text;
    if (m_newNum) {
        m_lastValue = m_lineEdit->text().toDouble();
        text = "0";
        m_newNum = false;
    }
    else text = m_lineEdit->text();
    text.append(QString::number(n));
    setLineText(text);
}

void MainWindow::pressNegate()
{
    double num = m_lineEdit->text().toDouble();
    num = - num;
    m_lineEdit->setText(QString::number(num));
}

void MainWindow::pressDot()
{   
    QString text = m_lineEdit->text();
    text.append(".");
    setLineText(text);
}

void MainWindow::pressEqual()
{
    double result = 0;
    double new_value = m_lineEdit->text().toDouble();
    switch(m_lastOperator)
    {
    case Plus :
        result = m_lastValue + new_value;
        break;
    case Minus :
        result = m_lastValue - new_value;
        break; 
    case Multiply :
        result = m_lastValue * new_value;
        break; 
    case Divide :
        result = m_lastValue / new_value;
        break; 
    default:
        result = new_value;
    }

    m_lastOperator = None;
    setLineText(QString::number(result));
}