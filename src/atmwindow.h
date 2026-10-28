#pragma once

#include "account.h"
#include <QMainWindow>

class QLineEdit;
class QLabel;
class QStackedWidget;
class QTableWidget;

class ATMWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ATMWindow(QWidget *parent = nullptr);

private:
    void buildLoginPage();
    void buildDashboardPage();
    void showDashboard();
    void showLogin();
    void updateBalance();
    void showMessage(const QString &title, const QString &message, bool error = false);
    bool parseAmount(const QString &text, qint64 *cents) const;
    QString money(qint64 cents) const;

    Account m_account;
    QStackedWidget *m_pages = nullptr;
    QWidget *m_loginPage = nullptr;
    QWidget *m_dashboardPage = nullptr;
    QLineEdit *m_pinInput = nullptr;
    QLabel *m_balanceLabel = nullptr;
    QTableWidget *m_historyTable = nullptr;
    int m_failedAttempts = 0;
};
