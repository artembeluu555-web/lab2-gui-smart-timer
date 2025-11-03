#include "AddEditDialog.h"

AddEditDialog::AddEditDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Новий таймер");
    resize(300, 150);

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Назва:"));
    m_nameEdit = new QLineEdit(this);
    layout->addWidget(m_nameEdit);

    layout->addWidget(new QLabel("Тривалість (секунди):"));
    m_durationSpin = new QSpinBox(this);
    m_durationSpin->setRange(1, 3600);
    m_durationSpin->setValue(60);
    layout->addWidget(m_durationSpin);

    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* ok = new QPushButton("OK", this);
    QPushButton* cancel = new QPushButton("Скасувати", this);
    buttons->addWidget(ok);
    buttons->addWidget(cancel);
    layout->addLayout(buttons);

    connect(ok, &QPushButton::clicked, this, &AddEditDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &AddEditDialog::reject);
}

QString AddEditDialog::name() const {
    return m_nameEdit->text();
}

int AddEditDialog::duration() const {
    return m_durationSpin->value();
}
