#include "settingsvaluedelegate.h"
#include "rsl/reginfoobj.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>

SettingsValueDelegate::SettingsValueDelegate(int typeColumn, QObject *parent)
    : QStyledItemDelegate(parent),
      m_typeColumn(typeColumn)
{
}

static qint16 typeFromIndex(const QModelIndex &index, int typeColumn)
{
    QModelIndex typeIndex = index.sibling(index.row(), typeColumn);
    QString typeName = typeIndex.data(Qt::EditRole).toString();
    return RegInfoObj::getTypeFromTypeName(typeName);
}

QWidget *SettingsValueDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(option)

    qint16 type = typeFromIndex(index, m_typeColumn);

    switch (type) {
    case RegInfoObj::INTEGER: {
        QSpinBox *spinBox = new QSpinBox(parent);
        spinBox->setFrame(false);
        spinBox->setRange(-2147483647, 2147483647);
        return spinBox;
    }
    case RegInfoObj::DOUBLE: {
        QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox(parent);
        doubleSpinBox->setFrame(false);
        doubleSpinBox->setRange(-1.7e308, 1.7e308);
        doubleSpinBox->setDecimals(6);
        return doubleSpinBox;
    }
    case RegInfoObj::STRING: {
        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setFrame(false);
        return lineEdit;
    }
    case RegInfoObj::BINARY: {
        QTextEdit *textEdit = new QTextEdit(parent);
        textEdit->setFrameStyle(QFrame::NoFrame);
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        return textEdit;
    }
    case RegInfoObj::FLAG: {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->setFrame(false);
        comboBox->addItems({"false", "true"});
        return comboBox;
    }
    default:
        break;
    }

    return new QLineEdit(parent);
}

void SettingsValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    QString value = index.data(Qt::EditRole).toString();

    if (QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor)) {
        bool ok;
        int intValue = value.toInt(&ok);
        if (ok) spinBox->setValue(intValue);
    }
    else if (QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        bool ok;
        double doubleValue = value.toDouble(&ok);
        if (ok) doubleSpinBox->setValue(doubleValue);
    }
    else if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        lineEdit->setText(value);
    }
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(editor)) {
        textEdit->setPlainText(value);
    }
    else if (QComboBox *comboBox = qobject_cast<QComboBox*>(editor)) {
        int currentIndex = comboBox->findText(value, Qt::MatchFixedString);
        if (currentIndex >= 0)
            comboBox->setCurrentIndex(currentIndex);
    }
}

void SettingsValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                          const QModelIndex &index) const
{
    if (!editor || !model)
        return;

    QVariant value;

    if (QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor)) {
        value = spinBox->value();
    }
    else if (QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor)) {
        value = doubleSpinBox->value();
    }
    else if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        value = lineEdit->text();
    }
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(editor)) {
        value = textEdit->toPlainText();
    }
    else if (QComboBox *comboBox = qobject_cast<QComboBox*>(editor)) {
        value = comboBox->currentText();
    }

    if (value.isValid())
        model->setData(index, value, Qt::EditRole);
}

void SettingsValueDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}
