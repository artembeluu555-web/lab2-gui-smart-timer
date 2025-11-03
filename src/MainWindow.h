#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "TimerManager.h"
#include "Timer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddClicked();
    void onStartClicked();
    void onStopClicked();
    void onRemoveClicked();
    void updateTable();

private:
    void setupUi();
    void setupConnections();

    TimerManager m_manager;
    QTableWidget *table{};
    QPushButton *m_addBtn{};
    QPushButton *m_startBtn{};
    QPushButton *m_stopBtn{};
    QPushButton *m_removeBtn{};
};

#endif
