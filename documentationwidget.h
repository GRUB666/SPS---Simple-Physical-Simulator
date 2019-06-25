#ifndef DOCUMENTATIONWIDGET_H
#define DOCUMENTATIONWIDGET_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTreeWidget>
#include <QDebug>

namespace Ui {
class DocumentationWidget;
}

class DocumentationWidget : public QDialog
{
    Q_OBJECT

private:
    QString Main_inf_way;
    bool first_selected;
    QString open_string;

public:
    explicit DocumentationWidget(QString string = "Основная информация о программе", QWidget *parent = nullptr);
    ~DocumentationWidget();
    QString readFromFile(QString& str);
    void selectItemWithText(QString& text);
    void selectItemRecursive(QString& text, QTreeWidgetItem* item);
    void openBranch(QTreeWidgetItem* item);
    void openCatalog(QString& name);

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::DocumentationWidget *ui;
};

#endif // DOCUMENTATIONWIDGET_H
