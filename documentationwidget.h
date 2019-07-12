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
    bool first_selected; //Необходимо для избежания непроизвольного срабатывания в начале работы
    QString open_string; //Названия католога, который будет открыт в первую очередь

public:
    explicit DocumentationWidget (QString string = "Основная информация о программе", QWidget *parent = nullptr);
             ~DocumentationWidget(                                                                             );

    QString  readFromFile        (QString& str); //Читает данные из файла


    void     selectItemWithText (QString& text                       );//Находит и выделяет элемент по его названию
    void     selectItemRecursive(QString& text, QTreeWidgetItem* item); //Побочная функция метода selectItemWithText

    void     openBranch (QTreeWidgetItem* item); //Открывает все родительские и дочерние ветки элемента

    void     openCatalog(QString& name); //Открывает каталог по указанному имени

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::DocumentationWidget *ui;
};

#endif // DOCUMENTATIONWIDGET_H
