#include "multiinputdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPushButton>

MultiInputDialog::MultiInputDialog(int count, QWidget *parent)
: QDialog(parent), m_GroupCount(count)
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *subLayout;
    m_Labels = new QLabel*[m_GroupCount];
    m_LineEdits = new QDoubleSpinBox*[m_GroupCount];
    //设计界面
    for(int i=0; i<m_GroupCount; i++)
    {
        subLayout = new QHBoxLayout;
        m_LineEdits[i] = new QDoubleSpinBox(this);
        m_Labels[i] = new QLabel(this);
        subLayout->addWidget(m_Labels[i]);
        subLayout->addWidget(m_LineEdits[i]);
        layout->addLayout(subLayout);
    }

    m_OKButton = new QPushButton( tr("确定"), this );
    m_CancelButton = new QPushButton( tr("取消"), this );
    subLayout = new QHBoxLayout;
    subLayout->addStretch();
    subLayout->addWidget(m_OKButton);
    subLayout->addWidget(m_CancelButton);
    layout->addLayout(subLayout);
    setLayout(layout);

    connect(m_OKButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    setWindowTitle("输入测试值");
    setStyleSheet("background-color:white;");
    resize(400,300);
}

MultiInputDialog::~MultiInputDialog()
{
    delete m_LineEdits;
    delete m_Labels;
}

void MultiInputDialog::SetLabelTexts(const QStringList &listText)
{
    for(int i=0; i<listText.size(); i++)
    {
        if( i >= m_GroupCount )
            break;
        m_Labels[i]->setText( listText.at(i) );
    }
}

void MultiInputDialog::SetOneLabelText(int index, const QString &text)
{
    m_Labels[index]->setText(text);
}

void MultiInputDialog::SetLabelsWidth(int width)
{
    for(int i=0; i<m_GroupCount; i++)
        m_Labels[i]->setFixedWidth(width);
}

void MultiInputDialog::SetLineEditRegExp(int index, QRegExp regExp)
{
    //QValidator *validator = new QRegExpValidator(regExp, this);
    //m_LineEdits[index]->setValidator( validator );
}

QString MultiInputDialog::GetOneText(int index)
{
    return m_LineEdits[index]->text();
}

QList<double> MultiInputDialog::GetAllTexts()
{
    QList<double> list;
    for(int i=0; i<m_GroupCount; i++)
    {
        list.push_back( m_LineEdits[i]->value() );
    }
    return list;
}
