#ifndef CHECKBOXTABLEHEADER_H
#define CHECKBOXTABLEHEADER_H

#include <QTableWidget>
#include <QCheckBox>
#include <QHeaderView>

class CheckBoxTableHeader : public QHeaderView {
    Q_OBJECT
public:
    CheckBoxTableHeader(Qt::Orientation orientation, QWidget *parent = nullptr,int _index=1)
        : QHeaderView(orientation, parent) {
        checkBox = new QCheckBox(this);
        connect(checkBox, &QCheckBox::stateChanged, this, &CheckBoxTableHeader::onCheckBoxStateChanged);

        checkBox->setStyleSheet("QCheckBox { margin-left: 35px;"
                                            "margin-top:6px; }");
        index=_index;
    }



private slots:
    void onCheckBoxStateChanged(int state) {
        QTableWidget *table = qobject_cast<QTableWidget *>(parent());

        if (table) {
            for (int row = 0; row < table->rowCount(); ++row) {
                QCheckBox *cellCheckBox = qobject_cast<QCheckBox *>(table->cellWidget(row, index));
                if (cellCheckBox) {
                    if(state==2){
                        cellCheckBox->setCheckState(Qt::Checked);
                    }else{
                        cellCheckBox->setCheckState(Qt::Unchecked);
                    }

                }
            }
        }
    }

private:
    QCheckBox *checkBox;
    int index;
};
#endif // CHECKBOXTABLEHEADER_H
