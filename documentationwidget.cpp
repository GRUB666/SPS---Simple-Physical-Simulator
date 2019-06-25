#include "documentationwidget.h"
#include "ui_documentationwidget.h"

DocumentationWidget::DocumentationWidget(QString string, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentationWidget)
{
    ui->setupUi(this);

    first_selected = true;
    open_string = string;
}

void DocumentationWidget::selectItemRecursive(QString &text, QTreeWidgetItem *item)
{
    if(item->text(0) == text)
    {
        item->setSelected(true);
        openBranch(item);
        return;
    }

    else
    {
        for(int i = 0; i < item->childCount(); i++)
            selectItemRecursive(text, item->child(i));
    }
}

void DocumentationWidget::openBranch(QTreeWidgetItem *item)
{
    bool next = true;

    for(int i = 0; ui->treeWidget->topLevelItem(i) != 0 && next; i++)
    {
        next = item != ui->treeWidget->topLevelItem(i);
    }

    ui->treeWidget->expandItem(item);

    if(next)
        openBranch(item->parent());
}

void DocumentationWidget::openCatalog(QString &name)
{
    QString way = ":/n/documentation/" + name + ".xml";

    QString str;
    str = readFromFile(way);

    ui->textEdit->setHtml(str);
}

DocumentationWidget::~DocumentationWidget()
{
    delete ui;
}

QString DocumentationWidget::readFromFile(QString &way)
{
    QFile file(way);
    QTextStream reader(&file);
    QString result_string = "<font color = 'red'><h1>ОШИБКА</h1></font>";

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка работы с ресурсами программы", "Файл по пути: " + way + " невозможно открыть");
    }

    else
    {
        result_string = reader.readAll();
        file.close();
    }

    return  result_string;
}

void DocumentationWidget::selectItemWithText(QString &text)
{
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        selectItemRecursive(text, ui->treeWidget->topLevelItem(i));
    }
}

void DocumentationWidget::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    if(!first_selected)
    {
        QString str = current->text(0);
        openCatalog(str);
    }

    else
    {
        first_selected = false;
        selectItemWithText(open_string);
        openCatalog(open_string);
    }
}
