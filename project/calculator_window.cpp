#include "calculator_window.h"
#include <QtMath>

CalculatorWindow::CalculatorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_central = new QWidget(this);
    setCentralWidget(m_central);

    m_rootLayout = new QVBoxLayout(m_central);
    m_rootLayout->setContentsMargins(12, 12, 12, 12);
    m_rootLayout->setSpacing(10);


    m_display = new QLabel("0", m_central);
    m_display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_display->setMinimumHeight(70);
    m_display->setStyleSheet("font-size: 24pt; background: #222; color: white; padding: 5px; border-radius: 5px;");
    m_rootLayout->addWidget(m_display);

    m_grid = new QGridLayout();
    m_grid->setSpacing(8);
    m_rootLayout->addLayout(m_grid);

 
    int number = 1;
    for (int row = 2; row >= 0; --row) {
        for (int col = 0; col < 3; ++col) {
            QString val = QString::number(number++);
            m_grid->addWidget(makeButton(val, val), row + 1, col);
        }
    }

 
    m_grid->addWidget(makeButton("0", "0"), 4, 0, 1, 2);
    m_grid->addWidget(makeButton(".", "."), 4, 2);

  
    const QStringList ops = { "/", "*", "-", "+", "=" };
    for (int i = 0; i < ops.size(); ++i) {
        m_grid->addWidget(makeButton(ops[i], ops[i]), i, 3);
    }

  
    m_grid->addWidget(makeButton("C", "C"), 0, 0);
    m_grid->addWidget(makeButton("←", "back"), 0, 1);
    m_grid->addWidget(makeButton("±", "sign"), 0, 2);
}


QPushButton* CalculatorWindow::makeButton(const QString& text, const QString& objName) {
    QPushButton* b = new QPushButton(text, m_central);
    b->setObjectName(objName);
    b->setMinimumSize(60, 55);
    b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    b->setStyleSheet("font-size: 14pt;");
    connect(b, &QPushButton::clicked, this, &CalculatorWindow::onButtonClicked);
    return b;
}



double CalculatorWindow::currentValue() const {
    bool ok = false;
    double v = m_display->text().toDouble(&ok);
    return ok ? v : 0.0;
}

void CalculatorWindow::setDisplayNumber(double v) {

    if (qAbs(v) < 1e-15) v = 0.0;
    m_display->setText(QString::number(v, 'g', 15));
}

void CalculatorWindow::clearAll() {
    m_acc = 0.0;
    m_pendingOp.clear();
    m_waitingForNewNumber = true;
    m_error = false;
    m_display->setText("0");
}

void CalculatorWindow::applyPendingOperation(double rhs) {
    if (m_pendingOp.isEmpty()) {
        m_acc = rhs;
        return;
    }

    if (m_pendingOp == "+") m_acc += rhs;
    else if (m_pendingOp == "-") m_acc -= rhs;
    else if (m_pendingOp == "*") m_acc *= rhs;
    else if (m_pendingOp == "/") {
        if (qFuzzyIsNull(rhs)) {
            m_error = true;
            m_display->setText("Error");
            return;
        }
        m_acc /= rhs;
    }
}

void CalculatorWindow::inputDigit(const QString& d) {
    if (m_error) clearAll();
    if (m_waitingForNewNumber) {
        m_display->setText(d);
        m_waitingForNewNumber = false;
    } else {
        if (m_display->text() == "0") m_display->setText(d);
        else m_display->setText(m_display->text() + d);
    }
}

void CalculatorWindow::inputDot() {
    if (m_error) clearAll();
    if (m_waitingForNewNumber) {
        m_display->setText("0.");
        m_waitingForNewNumber = false;
        return;
    }
    if (!m_display->text().contains('.'))
        m_display->setText(m_display->text() + ".");
}

void CalculatorWindow::inputOp(const QString& op) {
    if (m_error) return;
    double rhs = currentValue();

    if (!m_waitingForNewNumber) {
        applyPendingOperation(rhs);
        if (m_error) return;
        setDisplayNumber(m_acc);
    }

    m_pendingOp = op;
    m_waitingForNewNumber = true;
}

void CalculatorWindow::inputEquals() {
    if (m_error || m_pendingOp.isEmpty()) return;
    applyPendingOperation(currentValue());
    if (m_error) return;
    setDisplayNumber(m_acc);
    m_pendingOp.clear();
    m_waitingForNewNumber = true;
}

void CalculatorWindow::inputBackspace() {
    if (m_error) { clearAll(); return; }
    if (m_waitingForNewNumber) return;

    QString t = m_display->text();
    t.chop(1);
    if (t.isEmpty() || t == "-") t = "0";
    m_display->setText(t);
}

void CalculatorWindow::onButtonClicked() {
    QPushButton* b = qobject_cast<QPushButton*>(sender());
    if (!b) return;

    QString key = b->objectName();

    if (key.size() == 1 && key[0].isDigit()) inputDigit(key);
    else if (key == "C") clearAll();
    else if (key == "back") inputBackspace();
    else if (key == ".") inputDot();
    else if (key == "+" || key == "-" || key == "*" || key == "/") inputOp(key);
    else if (key == "=") inputEquals();
    else if (key == "sign") {
        if (m_error) return;
        setDisplayNumber(-currentValue());
    }
}
