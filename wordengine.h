#ifndef WORDENGINE_H
#define WORDENGINE_H
#include <QObject>
#include <QAxObject>
#include <QAxWidget>
//#include <QThread>
class WordEngine : public QObject
{
    Q_OBJECT
public:
    explicit WordEngine(QObject *parent = 0);
    WordEngine(const QString& strFile, QObject *parent = 0);
    ~WordEngine();
 
    bool open(bool bVisable = false);
    bool open(const QString& strFile, bool bVisable = false);
    bool close();
    bool isOpen();
    void save();
    bool saveAs(const QString& strSaveFile);
    // 设置标签内容
    bool setMarks(const QString& strMark, const QString& strContent);
    //插入标题 3
    bool setTitle(const QString& strMark, const QString& strContent);
    // 批量设置标签
    bool setBatchMarks(QStringList &itemList,QStringList sometexts);
    // 创建表格
    void insertTable(int nStart, int nEnd, int row, int column);
    QAxObject *insertTable(QString sLabel,int row, int column,QString sTitle);
    //合并单元格
    void MergeCells(QAxObject *table, int nStartRow,int nStartCol,int nEndRow,int nEndCol);
    //插入图片
    void insertPic(QString sLabel,const QString& picPath);
    //设置列宽
    void setColumnWidth(QAxObject *table, int column, int width);
    // 设置表格内容
    void setCellString(QAxObject *table, int row, int column, const QString& text);
    // 为表格添加行
    void addTableRow(QAxObject *table, int nRow, int rowCount);
    // 设置内容粗体  isBold控制是否粗体
    void setCellFontBold(QAxObject *table, int row, int column, bool isBold);
    // 设置文字大小
    void setCellFontSize(QAxObject *table, int row, int column, int size);
    // 在表格中插入图片
    void insertCellPic(QAxObject *table, int row,int column,const QString& picPath);
 
private:
    bool m_bOpened;
    QString m_strFilePath;
    QAxObject *m_wordDocuments;
    QAxObject *m_wordWidget;
//    QThread *m_thread;
signals:
 
public slots:
 
};
#endif // WORDENGINE_H
