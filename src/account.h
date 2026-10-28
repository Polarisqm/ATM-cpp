#pragma once

#include <QString>
#include <QVector>
#include <QDateTime>

struct Transaction {
    QString type;
    qint64 amountCents = 0;
    qint64 balanceAfterCents = 0;
    QDateTime timestamp;
};

class Account
{
public:
    Account();

    qint64 balanceCents() const { return m_balanceCents; }
    bool verifyPin(const QString &pin) const { return pin == m_pin; }
    bool changePin(const QString &currentPin, const QString &newPin, QString *error = nullptr);
    bool deposit(qint64 amountCents, QString *error = nullptr);
    bool withdraw(qint64 amountCents, QString *error = nullptr);
    const QVector<Transaction> &transactions() const { return m_transactions; }
    bool save(QString *error = nullptr) const;
    bool load(QString *error = nullptr);

private:
    void addTransaction(const QString &type, qint64 amountCents);
    QString dataFilePath() const;

    QString m_pin = "1234"; // Demo PIN only; this is not production authentication.
    qint64 m_balanceCents = 125000; // Demo starting balance: 1,250.00
    QVector<Transaction> m_transactions;
};
