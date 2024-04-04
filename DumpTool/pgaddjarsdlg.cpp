#include "pgaddjarsdlg.h"
#include "ui_pgaddjarsdlg.h"
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QAction>
#include <QItemSelectionModel>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QFile>
#include <QFileIconProvider>
#include <QSettings>

const QString filter = QObject::tr("Jar файлы (*.jar)");

class EditableJarDelegate : public QStyledItemDelegate
{
    QSettings *m_pSettings;
public:
    EditableJarDelegate(QSettings *settings, QObject *parent = nullptr) :
        QStyledItemDelegate(parent),
        m_pCurrent(nullptr),
        m_IsOpenDialog(false)
    {
        m_pSettings = settings;
    }

    virtual QWidget *createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        EditableJarDelegate *pThis = const_cast<EditableJarDelegate*>(this);

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);

        QWidget *frame = new QWidget(parent);
        frame->setLayout(layout);
        frame->setFocusPolicy(Qt::StrongFocus);
        frame->setObjectName("frame");

        QLineEdit *edit = new QLineEdit(frame);
        edit->setObjectName("lineEdit");

        QPushButton *btn = new QPushButton();
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setFlat(true);

        btn->setIcon(QIcon(":/img/openfolderHS.png"));
        btn->setFixedSize(option.rect.height(), option.rect.height());
        btn->setFocusPolicy(Qt::NoFocus);
        layout->addWidget(edit);
        layout->addWidget(btn);

        connect(btn, &QPushButton::clicked, [=]() -> void
        {
            m_pSettings->beginGroup("PgAddJarsDlg");
            QString lastPath = m_pSettings->value("LastJarDir").toString();
            m_pSettings->endGroup();

            pThis->m_IsOpenDialog = true;
            QString filename = QFileDialog::getOpenFileName(parent,
                                                            QString(),
                                                            lastPath,
                                                            filter);

            if (!filename.isEmpty())
            {
                edit->setText(filename);

                QFileInfo fi(filename);
                m_pSettings->beginGroup("PgAddJarsDlg");
                m_pSettings->setValue("LastJarDir", fi.absoluteDir().path());
                m_pSettings->endGroup();
                m_pSettings->sync();

                QApplication::postEvent(edit, new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
            }
            edit->setFocus();
            pThis->m_IsOpenDialog = false;
        });

        frame->setFocusProxy(edit);
        edit->installEventFilter(pThis);

        pThis->m_pCurrent = frame;
        pThis->m_CurrentIndex = index;
        return frame;
    }

    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE
    {
        if (event->type() == QEvent::FocusOut)
        {
            if (!qstrcmp(obj->metaObject()->className(), "QLineEdit") && !m_IsOpenDialog)
            {
                QListView *view = qobject_cast<QListView*>(parent());
                setModelData(m_pCurrent, view->model(), m_CurrentIndex);
                emit closeEditor(m_pCurrent, QAbstractItemDelegate::SubmitModelCache);
            }

            return true;
        }

        return QStyledItemDelegate::eventFilter(obj, event);
    }

    virtual void setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        QLineEdit *edit = editor->findChild<QLineEdit*>();

        if (!edit)
            return;

        QFileIconProvider provider;
        model->setData(index, edit->text().trimmed());
        model->setData(index, provider.icon(QFileInfo(edit->text().trimmed())), Qt::DecorationRole);
    }

    virtual void setEditorData(QWidget *editor,
                               const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        QLineEdit *edit = editor->findChild<QLineEdit*>();

        if (!edit)
            return;

        edit->setText(index.data().toString());
    }

private:
    QWidget *m_pCurrent;
    QModelIndex m_CurrentIndex;

    bool m_IsOpenDialog;
};

PgAddJarsDlg::PgAddJarsDlg(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PgAddJarsDlg)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(this);
    m_pDelegate = new EditableJarDelegate(settings, ui->listView);
    ui->listView->setModel(m_pModel);
    ui->listView->setItemDelegate(m_pDelegate);

    m_pSettings = settings;

    connect(m_pModel, &QStandardItemModel::dataChanged, this, &PgAddJarsDlg::dataChanged);
}

PgAddJarsDlg::~PgAddJarsDlg()
{
    delete ui;
}

void PgAddJarsDlg::addStatic(const QString &value, const QIcon &icon)
{
    QStandardItem *item = new QStandardItem();
    item->setEnabled(false);
    item->setText(value);
    item->setSelectable(false);
    item->setIcon(icon);
    m_pModel->appendRow(item);
}

void PgAddJarsDlg::addList(const QStringList &lst)
{
    QFileIconProvider provider;
    for (const QString &file : lst)
    {
        QStandardItem *item = new QStandardItem();
        item->setText(file);
        item->setIcon(provider.icon(file));
        m_pModel->appendRow(item);
    }
}

void PgAddJarsDlg::on_newBtn_clicked()
{
    m_pModel->insertRow(m_pModel->rowCount());
    ui->listView->edit(m_pModel->index(m_pModel->rowCount() - 1, 0));
}

void PgAddJarsDlg::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    QString value = topLeft.data().toString().trimmed();
    if (value.isEmpty())
    {
        m_pModel->removeRow(topLeft.row());
        return;
    }

    if (!QFile::exists(value))
        m_pModel->removeRow(topLeft.row());
}

QStringList PgAddJarsDlg::files()
{
    QStringList files;
    for (int i = 0; i < m_pModel->rowCount(); i++)
    {
        QStandardItem *item = m_pModel->item(i);

        if (item->isEditable() && item->isEditable())
            files.append(item->text());
    }

    return files;
}

void PgAddJarsDlg::on_addFilesBtn_clicked()
{
    m_pSettings->beginGroup("PgAddJarsDlg");
    QString lastPath = m_pSettings->value("LastJarDir").toString();
    m_pSettings->endGroup();

    QStringList lst = QFileDialog::getOpenFileNames(this, QString(),
                                                    lastPath,
                                                    filter);

    if (lst.isEmpty())
        return;

    QFileIconProvider provider;
    for (const QString &file : lst)
    {
        QStandardItem *item = new QStandardItem();
        item->setText(file);
        item->setIcon(provider.icon(file));
        m_pModel->appendRow(item);
    }

    QFileInfo fi(lst.first());
    m_pSettings->beginGroup("PgAddJarsDlg");
    m_pSettings->setValue("LastJarDir", fi.absoluteDir().path());
    m_pSettings->endGroup();
    m_pSettings->sync();
}


void PgAddJarsDlg::on_remBtn_clicked()
{
    QItemSelectionModel *selection = ui->listView->selectionModel();
    QModelIndex index = selection->currentIndex();

    if (!index.isValid())
        return;

    m_pModel->removeRow(index.row());
}

