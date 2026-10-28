#include "atmwindow.h"

#include <QDateTime>
#include <QAbstractItemView>
#include <QDoubleValidator>
#include <QFormLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

ATMWindow::ATMWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ATM-cpp | Simulador de cajero automático");
    setMinimumSize(760, 560);
    resize(900, 650);
    setStyleSheet(R"(
        QMainWindow, QWidget { background: #f3f6fb; color: #172033; font-family: 'Segoe UI', Arial; font-size: 14px; }
        QLabel#brand { color: #ffffff; font-size: 25px; font-weight: 700; }
        QLabel#subtitle { color: #c8d5ea; font-size: 13px; }
        QWidget#hero { background: #13294b; border-radius: 14px; }
        QLabel#balance { color: #ffffff; font-size: 30px; font-weight: 700; }
        QLabel#muted { color: #63718a; }
        QLineEdit { background: white; border: 1px solid #cbd5e1; border-radius: 8px; padding: 10px; min-height: 22px; }
        QLineEdit:focus { border: 2px solid #3978d4; }
        QPushButton { background: #ffffff; border: 1px solid #d5deeb; border-radius: 9px; padding: 12px 16px; font-weight: 600; }
        QPushButton:hover { background: #eaf1fc; border-color: #7ba5e8; }
        QPushButton#primary { background: #2563c7; color: white; border: none; }
        QPushButton#primary:hover { background: #1e4fa1; }
        QPushButton#danger { color: #b42318; }
        QTableWidget { background: white; border: 1px solid #e1e7f0; border-radius: 8px; gridline-color: #edf1f7; }
        QHeaderView::section { background: #eaf0f8; padding: 9px; border: none; font-weight: 700; }
    )");

    m_pages = new QStackedWidget(this);
    setCentralWidget(m_pages);
    buildLoginPage();
    buildDashboardPage();
    m_pages->addWidget(m_loginPage);
    m_pages->addWidget(m_dashboardPage);
    m_pages->setCurrentWidget(m_loginPage);
}

void ATMWindow::buildLoginPage()
{
    m_loginPage = new QWidget;
    auto *outer = new QVBoxLayout(m_loginPage);
    outer->setContentsMargins(70, 45, 70, 45);
    outer->addStretch();

    auto *hero = new QWidget;
    hero->setObjectName("hero");
    auto *heroLayout = new QVBoxLayout(hero);
    heroLayout->setContentsMargins(30, 26, 30, 26);
    auto *brand = new QLabel("ATM-cpp"); brand->setObjectName("brand");
    auto *subtitle = new QLabel("SIMULADOR DE CAJERO AUTOMÁTICO"); subtitle->setObjectName("subtitle");
    heroLayout->addWidget(brand);
    heroLayout->addWidget(subtitle);
    outer->addWidget(hero);

    auto *card = new QWidget;
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(30, 25, 30, 25);
    auto *heading = new QLabel("Bienvenido"); heading->setStyleSheet("font-size: 23px; font-weight: 700;");
    auto *hint = new QLabel("Introduce tu PIN de 4 dígitos para acceder a tu cuenta."); hint->setObjectName("muted");
    m_pinInput = new QLineEdit;
    m_pinInput->setPlaceholderText("PIN de 4 dígitos");
    m_pinInput->setEchoMode(QLineEdit::Password);
    m_pinInput->setMaxLength(4);
    m_pinInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]{0,4}"), m_pinInput));
    auto *loginButton = new QPushButton("Acceder a mi cuenta"); loginButton->setObjectName("primary");
    auto *demo = new QLabel("PIN inicial de demostración: 1234"); demo->setObjectName("muted");
    demo->setStyleSheet("font-size: 12px;");
    layout->addWidget(heading);
    layout->addWidget(hint);
    layout->addSpacing(12);
    layout->addWidget(m_pinInput);
    layout->addWidget(loginButton);
    layout->addWidget(demo);
    outer->addWidget(card);
    outer->addStretch();

    connect(loginButton, &QPushButton::clicked, this, [this] {
        if (m_account.verifyPin(m_pinInput->text())) {
            m_failedAttempts = 0;
            m_pinInput->clear();
            showDashboard();
        } else {
            ++m_failedAttempts;
            m_pinInput->clear();
            if (m_failedAttempts >= 3) {
                QMessageBox::warning(this, "Acceso bloqueado", "Has fallado 3 veces. La sesión de demostración se cerrará para reiniciar el contador.");
                m_failedAttempts = 0;
            } else {
                showMessage("PIN incorrecto", QString("El PIN no es correcto. Intentos restantes: %1.").arg(3 - m_failedAttempts), true);
            }
        }
    });
    connect(m_pinInput, &QLineEdit::returnPressed, loginButton, &QPushButton::click);
}

void ATMWindow::buildDashboardPage()
{
    m_dashboardPage = new QWidget;
    auto *outer = new QVBoxLayout(m_dashboardPage);
    outer->setContentsMargins(28, 24, 28, 24);
    auto *top = new QHBoxLayout;
    auto *heading = new QLabel("Panel de cuenta"); heading->setStyleSheet("font-size: 25px; font-weight: 700;");
    auto *logout = new QPushButton("Cerrar sesión"); logout->setObjectName("danger");
    top->addWidget(heading); top->addStretch(); top->addWidget(logout);
    outer->addLayout(top);

    auto *hero = new QWidget; hero->setObjectName("hero");
    auto *heroLayout = new QVBoxLayout(hero); heroLayout->setContentsMargins(24, 20, 24, 20);
    auto *caption = new QLabel("SALDO DISPONIBLE"); caption->setObjectName("subtitle");
    m_balanceLabel = new QLabel("$ 0,00"); m_balanceLabel->setObjectName("balance");
    heroLayout->addWidget(caption); heroLayout->addWidget(m_balanceLabel);
    outer->addWidget(hero);

    auto *actions = new QHBoxLayout;
    auto *depositButton = new QPushButton("＋  Depositar"); depositButton->setObjectName("primary");
    auto *withdrawButton = new QPushButton("－  Retirar");
    auto *balanceButton = new QPushButton("Consultar saldo");
    auto *pinButton = new QPushButton("Cambiar PIN");
    actions->addWidget(depositButton); actions->addWidget(withdrawButton); actions->addWidget(balanceButton); actions->addWidget(pinButton);
    outer->addLayout(actions);

    auto *historyTitle = new QLabel("Movimientos recientes"); historyTitle->setStyleSheet("font-size: 18px; font-weight: 700; margin-top: 4px;");
    outer->addWidget(historyTitle);
    m_historyTable = new QTableWidget(0, 4);
    m_historyTable->setHorizontalHeaderLabels({"Fecha", "Operación", "Importe", "Saldo posterior"});
    m_historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_historyTable->verticalHeader()->setVisible(false);
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_historyTable->setAlternatingRowColors(true);
    outer->addWidget(m_historyTable, 1);
    auto *footer = new QLabel("Proyecto educativo · Los datos se guardan localmente en este equipo.");
    footer->setObjectName("muted"); footer->setStyleSheet("font-size: 11px;");
    outer->addWidget(footer);

    connect(logout, &QPushButton::clicked, this, &ATMWindow::showLogin);
    connect(balanceButton, &QPushButton::clicked, this, [this] { showMessage("Consulta de saldo", "Tu saldo disponible es " + money(m_account.balanceCents()) + "."); });
    connect(depositButton, &QPushButton::clicked, this, [this] {
        bool ok = false;
        const QString value = QInputDialog::getText(this, "Depositar dinero", "Importe a depositar (ej. 250,50):", QLineEdit::Normal, "", &ok);
        if (!ok) return;
        qint64 cents = 0; QString error;
        if (!parseAmount(value, &cents)) { showMessage("Importe inválido", "Introduce un importe mayor que cero con máximo dos decimales.", true); return; }
        if (!m_account.deposit(cents, &error)) { showMessage("No se pudo depositar", error, true); return; }
        updateBalance(); showMessage("Depósito realizado", "Se han ingresado " + money(cents) + " correctamente.");
    });
    connect(withdrawButton, &QPushButton::clicked, this, [this] {
        bool ok = false;
        const QString value = QInputDialog::getText(this, "Retirar dinero", "Importe a retirar (ej. 100,00):", QLineEdit::Normal, "", &ok);
        if (!ok) return;
        qint64 cents = 0; QString error;
        if (!parseAmount(value, &cents)) { showMessage("Importe inválido", "Introduce un importe mayor que cero con máximo dos decimales.", true); return; }
        if (!m_account.withdraw(cents, &error)) { showMessage("No se pudo retirar", error, true); return; }
        updateBalance(); showMessage("Retirada realizada", "Has retirado " + money(cents) + " correctamente.");
    });
    connect(pinButton, &QPushButton::clicked, this, [this] {
        bool ok = false;
        const QString current = QInputDialog::getText(this, "Cambiar PIN", "PIN actual:", QLineEdit::Password, "", &ok);
        if (!ok) return;
        const QString next = QInputDialog::getText(this, "Cambiar PIN", "Nuevo PIN (4 dígitos):", QLineEdit::Password, "", &ok);
        if (!ok) return;
        const QString confirm = QInputDialog::getText(this, "Cambiar PIN", "Confirma el nuevo PIN:", QLineEdit::Password, "", &ok);
        if (!ok) return;
        if (next != confirm) { showMessage("PIN no cambiado", "La confirmación no coincide con el nuevo PIN.", true); return; }
        QString error;
        if (!m_account.changePin(current, next, &error)) { showMessage("PIN no cambiado", error, true); return; }
        updateBalance(); showMessage("PIN actualizado", "Tu PIN se ha cambiado correctamente.");
    });
}

void ATMWindow::showDashboard()
{
    updateBalance();
    m_pages->setCurrentWidget(m_dashboardPage);
}

void ATMWindow::showLogin()
{
    m_pages->setCurrentWidget(m_loginPage);
    m_pinInput->clear();
    m_pinInput->setFocus();
}

void ATMWindow::updateBalance()
{
    m_balanceLabel->setText(money(m_account.balanceCents()));
    const auto &items = m_account.transactions();
    m_historyTable->setRowCount(items.size());
    for (int row = 0; row < items.size(); ++row) {
        const auto &tx = items.at(row);
        auto *date = new QTableWidgetItem(tx.timestamp.toString("dd/MM/yyyy HH:mm"));
        auto *type = new QTableWidgetItem(tx.type);
        auto *amount = new QTableWidgetItem(tx.amountCents == 0 ? "—" : money(tx.amountCents));
        auto *balance = new QTableWidgetItem(money(tx.balanceAfterCents));
        amount->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        balance->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_historyTable->setItem(row, 0, date); m_historyTable->setItem(row, 1, type);
        m_historyTable->setItem(row, 2, amount); m_historyTable->setItem(row, 3, balance);
    }
}

void ATMWindow::showMessage(const QString &title, const QString &message, bool error)
{
    if (error) QMessageBox::warning(this, title, message);
    else QMessageBox::information(this, title, message);
}

bool ATMWindow::parseAmount(const QString &text, qint64 *cents) const
{
    QString normalized = text.trimmed();
    normalized.remove(' ');
    if (normalized.contains(',') && normalized.contains('.')) return false;
    normalized.replace(',', '.');
    static const QRegularExpression pattern("^[0-9]{1,10}(\\.[0-9]{1,2})?$");
    if (!pattern.match(normalized).hasMatch()) return false;
    const double amount = normalized.toDouble();
    if (!(amount > 0.0) || amount > 1000000000.0) return false;
    *cents = qRound64(amount * 100.0);
    return *cents > 0;
}

QString ATMWindow::money(qint64 cents) const
{
    const qint64 whole = cents / 100;
    const qint64 fraction = cents % 100;
    QString grouped = QString::number(whole);
    for (int i = grouped.size() - 3; i > 0; i -= 3) grouped.insert(i, '.');
    return QString("$ %1,%2").arg(grouped).arg(fraction, 2, 10, QChar('0'));
}
