#include "account.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QRegularExpression>
#include <QStandardPaths>

namespace {
constexpr qint64 MaxTransactionCents = 100000000000LL;
}

Account::Account()
{
    QString ignored;
    load(&ignored);
}

QString Account::dataFilePath() const
{
    QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (base.isEmpty()) base = QDir::homePath() + "/.atm-cpp";
    QDir().mkpath(base);
    return base + "/account.json";
}

bool Account::deposit(qint64 amountCents, QString *error)
{
    if (amountCents <= 0 || amountCents > MaxTransactionCents) {
        if (error) *error = "Introduce un importe positivo y razonable.";
        return false;
    }
    if (m_balanceCents > MaxTransactionCents - amountCents) {
        if (error) *error = "El saldo resultante supera el límite de la simulación.";
        return false;
    }
    m_balanceCents += amountCents;
    addTransaction("Depósito", amountCents);
    return save(error);
}

bool Account::withdraw(qint64 amountCents, QString *error)
{
    if (amountCents <= 0 || amountCents > MaxTransactionCents) {
        if (error) *error = "Introduce un importe positivo y razonable.";
        return false;
    }
    if (amountCents > m_balanceCents) {
        if (error) *error = "Saldo insuficiente para realizar esta retirada.";
        return false;
    }
    m_balanceCents -= amountCents;
    addTransaction("Retirada", amountCents);
    return save(error);
}

bool Account::changePin(const QString &currentPin, const QString &newPin, QString *error)
{
    if (!verifyPin(currentPin)) {
        if (error) *error = "El PIN actual no es correcto.";
        return false;
    }
    if (newPin.size() != 4 || !newPin.contains(QRegularExpression("^[0-9]{4}$"))) {
        if (error) *error = "El nuevo PIN debe tener exactamente 4 dígitos.";
        return false;
    }
    if (newPin == currentPin) {
        if (error) *error = "El nuevo PIN debe ser diferente del actual.";
        return false;
    }
    const QString previous = m_pin;
    m_pin = newPin;
    if (!save(error)) {
        m_pin = previous;
        return false;
    }
    addTransaction("Cambio de PIN", 0);
    return save(error);
}

void Account::addTransaction(const QString &type, qint64 amountCents)
{
    m_transactions.prepend({type, amountCents, m_balanceCents, QDateTime::currentDateTime()});
    constexpr int MaxHistory = 100;
    while (m_transactions.size() > MaxHistory) m_transactions.removeLast();
}

bool Account::save(QString *error) const
{
    QJsonObject root;
    root["version"] = 1;
    root["pin"] = m_pin;
    root["balanceCents"] = static_cast<double>(m_balanceCents);
    QJsonArray history;
    for (const auto &tx : m_transactions) {
        QJsonObject item;
        item["type"] = tx.type;
        item["amountCents"] = static_cast<double>(tx.amountCents);
        item["balanceAfterCents"] = static_cast<double>(tx.balanceAfterCents);
        item["timestamp"] = tx.timestamp.toString(Qt::ISODate);
        history.append(item);
    }
    root["transactions"] = history;
    QSaveFile file(dataFilePath());
    if (!file.open(QIODevice::WriteOnly)) {
        if (error) *error = "No se pudo guardar la cuenta: " + file.errorString();
        return false;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    if (!file.commit()) {
        if (error) *error = "No se pudo completar el guardado de la cuenta.";
        return false;
    }
    return true;
}

bool Account::load(QString *error)
{
    QFile file(dataFilePath());
    if (!file.exists()) return save(error); // First run: create demo account.
    if (!file.open(QIODevice::ReadOnly)) {
        if (error) *error = "No se pudo leer la cuenta guardada.";
        return false;
    }
    QJsonParseError parseError{};
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        if (error) *error = "El archivo de cuenta no contiene JSON válido.";
        return false;
    }
    const QJsonObject root = doc.object();
    const QString pin = root.value("pin").toString();
    const qint64 balance = static_cast<qint64>(root.value("balanceCents").toDouble(-1));
    if (pin.size() != 4 || !pin.contains(QRegularExpression("^[0-9]{4}$")) || balance < 0 || balance > MaxTransactionCents) {
        if (error) *error = "Los datos guardados de la cuenta no son válidos.";
        return false;
    }
    m_pin = pin;
    m_balanceCents = balance;
    m_transactions.clear();
    const QJsonArray history = root.value("transactions").toArray();
    for (const auto &value : history) {
        const QJsonObject item = value.toObject();
        Transaction tx;
        tx.type = item.value("type").toString();
        tx.amountCents = static_cast<qint64>(item.value("amountCents").toDouble());
        tx.balanceAfterCents = static_cast<qint64>(item.value("balanceAfterCents").toDouble());
        tx.timestamp = QDateTime::fromString(item.value("timestamp").toString(), Qt::ISODate);
        if (!tx.type.isEmpty() && tx.amountCents >= 0 && tx.balanceAfterCents >= 0 && tx.timestamp.isValid())
            m_transactions.append(tx);
    }
    return true;
}
