#include "PageControl.h"

PagedTableWidget::PagedTableWidget(QWidget *parent,QHBoxLayout *PageLayout) : QWidget(parent), rowsPerPage(10), currentPage(0) {
    pageLayout = PageLayout;
    if (pageLayout) {
        // 遍历并删除布局中的所有项
        while (QLayoutItem *item = pageLayout->takeAt(0)) {
            // 删除项，这会释放相应的QWidget（如果存在）
            delete item->widget();
            // 删除QLayoutItem本身
            delete item;
        }
    }
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(pageLayout);
    setLayout(mainLayout);
}

PagedTableWidget::~PagedTableWidget() {
    for (auto button : pageButtons) {
        delete button;
    }
}

void PagedTableWidget::setData(int datacount, int rowsPerPage) {
    //tableWidget=table;
    totaldatacount=datacount;
    this->rowsPerPage = rowsPerPage;
    //allData = data;
    currentPage = 0;
    createPageButtons();
    //updateTable();
}

void PagedTableWidget::setCurrentPage(int page) {
    if (page >= 0 && page < pageButtons.size()) {
        /*for (auto button : pageButtons) {
         //button->setChecked(false);
         button->setChecked(false);
        }*/
        currentPage = page;
        //updateTable();
        emit pageChanged(page);
    }
}

void PagedTableWidget::onPageButtonClicked() {
    for (auto button : pageButtons) {
     button->setChecked(false);
    }
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        button->setChecked(true);
        int pageIndex = pageButtons.indexOf(button);
        setCurrentPage(pageIndex-1);
    }
}
void PagedTableWidget::onPageSwitching(bool Switch)
{
    QPushButton *Currbutton;
    for (auto button : pageButtons) {
     button->setChecked(false);
    }
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {

        if(Switch){
            currentPage++;
            if(currentPage>calculateTotalPages()-1)
            {
               currentPage=calculateTotalPages()-1;
            }

        }else{
            currentPage--;
            if(currentPage<0)
            {
               currentPage=0;
            }

        }
        for (auto button : pageButtons) {
            if(button->text()==QString::number(currentPage+1))
            {
               button->setChecked(true);
            }
        }
        int pageIndex = currentPage;
        setCurrentPage(pageIndex);
    }
}
/*
void PagedTableWidget::updateTable() {
    //tableWidget->clear();
    tableWidget->setRowCount(0);
     //tableWidget->setRowCount(rowsPerPage);
    int startRow = currentPage * rowsPerPage;
    int endRow = std::min(startRow + rowsPerPage, allData.size());
    //tableWidget->setRowCount(endRow - startRow);
    int rowCount = 0;
    for (int i = startRow; i < endRow; ++i) {
        rowCount = tableWidget->rowCount();
        tableWidget->insertRow(i - startRow);
        for (int j = 0; j < allData[i].count(); ++j) {
            tableWidget->setItem(i - startRow, j, new QTableWidgetItem(allData[i][j]));
        }
    }
}
*/
void PagedTableWidget::createPageButtons() {
    int totalPages = calculateTotalPages();
    for (auto button : pageButtons) {
        pageLayout->removeWidget(button);
        delete button;
    }
    pageButtons.clear();

    for (int i = 0; i < totalPages; ++i) {
        if(i==0)
        {
            QPushButton *PageUpbutton = new QPushButton("上一页");
            pageButtons.append(PageUpbutton);
            pageLayout->addWidget(PageUpbutton);
            connect(PageUpbutton, &QPushButton::clicked, this, [this]() {
                this->onPageSwitching(false);
            });

        }


        QPushButton *button = new QPushButton(QString::number(i + 1));
        button->setStyleSheet("QPushButton:checked {color: blue;}");

        button->setCheckable(true);
        connect(button, &QPushButton::clicked, this, &PagedTableWidget::onPageButtonClicked);
        if (i == currentPage) {
            button->setChecked(true);
        }
        pageButtons.append(button);
        pageLayout->addWidget(button);
        if(i==totalPages-1)
        {
            QPushButton *PageNextbutton = new QPushButton("下一页");
            pageButtons.append(PageNextbutton);
            pageLayout->addWidget(PageNextbutton);
            connect(PageNextbutton, &QPushButton::clicked, this, [this]() {
                this->onPageSwitching(true);
            });
        }
    }
}

int PagedTableWidget::calculateTotalPages() {
    return ((totaldatacount + rowsPerPage -1) / rowsPerPage);
}
