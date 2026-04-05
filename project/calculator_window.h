#ifndef CALCULATOR_WINDOW_H
#define CALCULATOR_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>

class CalculatorWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalculatorWindow(QWidget* parent = nullptr);

private slots:
    void onButtonClicked();

private:

    QPushButton* makeButton(const QString& text, const QString& objName);


    double currentValue() const;
    void setDisplayNumber(double v);
    void clearAll();
    void applyPendingOperation(double rhs);
    

    void inputDigit(const QString& d);
    void inputDot();
    void inputOp(const QString& op);
    void inputEquals();
    void inputBackspace();


    QWidget* m_central = nullptr;
    QLabel* m_display = nullptr;
    QVBoxLayout* m_rootLayout = nullptr;
    QGridLayout* m_grid = nullptr;


    double m_acc = 0.0;
    QString m_pendingOp;
    bool m_waitingForNewNumber = true;
    bool m_error = false;
};

#endif 
