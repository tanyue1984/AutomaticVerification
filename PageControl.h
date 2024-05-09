#ifndef PAGECONTROL_H
#define PAGECONTROL_H


#include <QWidget>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVector>

class PagedTableWidget : public QWidget {
    Q_OBJECT

public:
    explicit PagedTableWidget(QWidget *parent = nullptr,QHBoxLayout *PageLayout=NULL);
    ~PagedTableWidget();

    void setData(int datacount, int rowsPerPage = 10);
    void setCurrentPage(int page);

signals:
    void pageChanged(int page);

private slots:
    void onPageButtonClicked();
    void onPageSwitching(bool Switch);

private:
    QTableWidget *tableWidget;
    QHBoxLayout *pageLayout;
    QList<QPushButton *> pageButtons;
    QList<QList<QString>> allData;
    int totaldatacount;
    int rowsPerPage;
    int currentPage;

    void updateTable();
    void createPageButtons();
    int calculateTotalPages();
};
#endif // PAGECONTROL_H
