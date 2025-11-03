#ifndef ADDEDITDIALOG_H
#define ADDEDITDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class AddEditDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddEditDialog(QWidget* parent = nullptr);

    QString name() const;
    int duration() const;

private:
    QLineEdit* m_nameEdit;
    QSpinBox* m_durationSpin;
};

#endif
