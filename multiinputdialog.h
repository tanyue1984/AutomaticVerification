#ifndef MULTIINPUTDIALOG_H
#define MULTIINPUTDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QDoubleSpinBox>

class MultiInputDialog : public QDialog
{
    Q_OBJECT
    private:
        const int m_GroupCount;
        QLabel **m_Labels;
        QDoubleSpinBox **m_LineEdits;
        QPushButton *m_OKButton;
        QPushButton *m_CancelButton;

    public:
        MultiInputDialog(int count, QWidget *parent = 0);
        virtual ~MultiInputDialog();
        void SetLabelTexts(const QStringList &listText);
        void SetOneLabelText(int index, const QString &text);
        QString GetOneText(int index);
        QList<double> GetAllTexts();
        //为了让多组输入框看起来整齐些，所以我固定了QLabel的宽度
        void SetLabelsWidth(int width);
        //使用正则表达式来限制输入的字符
        void SetLineEditRegExp(int index, QRegExp regExp);

        //请根据需要重写这两个函数
        virtual void accept() { QDialog::accept(); }
        virtual void reject() { QDialog::reject(); }
};

#endif // MULTIINPUTDIALOG_H
