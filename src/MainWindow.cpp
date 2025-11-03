#include "MainWindow.h"
#include "AddEditDialog.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();
}

void MainWindow::setupUi() {
    resize(600, 400);
    setWindowTitle("Smart Timer");

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    table = new QTableWidget(0, 3, this);
    table->setHorizontalHeaderLabels({"Назва", "Залишилось (сек)", "Статус"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *addBtn = new QPushButton("Додати", this);
    QPushButton *startBtn = new QPushButton("Старт", this);
    QPushButton *stopBtn = new QPushButton("Стоп", this);
    QPushButton *removeBtn = new QPushButton("Видалити", this);

    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(stopBtn);
    btnLayout->addWidget(removeBtn);
    layout->addLayout(btnLayout);

    setCentralWidget(central);


    m_addBtn = addBtn;
    m_startBtn = startBtn;
    m_stopBtn = stopBtn;
    m_removeBtn = removeBtn;
}

void MainWindow::setupConnections() {
    connect(m_addBtn, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(m_startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(m_stopBtn, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(m_removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);


    connect(&m_manager, &TimerManager::timerUpdated, this, &MainWindow::updateTable);


    connect(&m_manager, &TimerManager::timerFinished, this, [this](const QString &id) {
        qDebug() << "MainWindow: got finished signal for id:" << id;
        QMessageBox::information(this, "Таймер", "⏰ Таймер завершився!");
        updateTable();
    });
}

void MainWindow::onAddClicked() {
    AddEditDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto timer = std::make_shared<Timer>();
        timer->setName(dlg.name());
        timer->setDuration(dlg.duration());
        m_manager.addTimer(timer);
        updateTable();
    }
}

void MainWindow::onStartClicked() {
    int row = table->currentRow();
    if (row < 0) return;
    QString id = table->item(row, 0)->data(Qt::UserRole).toString();
    qDebug() << "MainWindow: Start clicked, id=" << id;
    m_manager.startTimer(id);
}

void MainWindow::onStopClicked() {
    int row = table->currentRow();
    if (row < 0) return;
    QString id = table->item(row, 0)->data(Qt::UserRole).toString();
    qDebug() << "MainWindow: Stop clicked, id=" << id;
    m_manager.stopTimer(id);
}

void MainWindow::onRemoveClicked() {
    int row = table->currentRow();
    if (row < 0) return;
    QString id = table->item(row, 0)->data(Qt::UserRole).toString();
    m_manager.removeTimer(id);
    updateTable();
}

void MainWindow::updateTable() {
    table->setRowCount(m_manager.timers().size());

    int row = 0;
    for (auto &t : m_manager.timers()) {
        QTableWidgetItem *nameItem = new QTableWidgetItem(t->name());
        nameItem->setData(Qt::UserRole, t->id());

        QTableWidgetItem *timeItem = new QTableWidgetItem(QString::number(t->remainingSeconds()));
        QTableWidgetItem *statusItem = new QTableWidgetItem(t->isRunning() ? "Працює" : "Зупинено");

        table->setItem(row, 0, nameItem);
        table->setItem(row, 1, timeItem);
        table->setItem(row, 2, statusItem);
        row++;
    }
}
