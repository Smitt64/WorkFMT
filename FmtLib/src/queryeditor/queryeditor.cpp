#include "queryeditor.h"

QueryEditor::QueryEditor(QWidget *parent) :
    MdiSubInterface(parent)
{

}

QString QueryEditor::makeWindowTitle()
{
    return "QueryEditor";
}
