#include "wordengine.h"
#include "qt_windows.h"
//#include <QDebug>
//#include <windows.h>
WordEngine::WordEngine(QObject *parent) :
    QObject(parent),
    m_bOpened(false),
    m_wordDocuments(NULL),
    m_wordWidget(NULL)
{
}
 
WordEngine::WordEngine(const QString& strFilePath, QObject *parent):
    QObject(parent),
    m_bOpened(false),
    m_strFilePath(strFilePath),
    m_wordDocuments(NULL),
    m_wordWidget(NULL)
{
}
 
WordEngine::~WordEngine()
{
    close();
}
 
/******************************************************************************
 * 函数：open
 * 功能：打开文件
 * 参数：bVisable 是否显示弹窗
 * 返回值： bool
 *****************************************************************************/
bool WordEngine::open(bool bVisable)
{
    //新建一个word应用程序,并设置为可见
    m_wordWidget = new QAxObject();
    bool bFlag = m_wordWidget->setControl( "word.Application" );
    if(!bFlag)
    {
        // 用wps打开
        bFlag = m_wordWidget->setControl( "kwps.Application" );
        if(!bFlag)
        {
            return false;
        }
    }
    m_wordWidget->setProperty("Visible", bVisable);
    //获取所有的工作文档
    QAxObject *document = m_wordWidget->querySubObject("Documents");
    if(!document)
    {
        return false;
    }
    //以文件template.dot为模版新建一个文档
    document->dynamicCall("Add(QString)", m_strFilePath);
    //获取当前激活的文档
    m_wordDocuments = m_wordWidget->querySubObject("ActiveDocument");
    m_bOpened = true;
    return m_bOpened;
}
 
/******************************************************************************
 * 函数：open
 * 功能：打开文件
 * 参数：strFilePath 文件路径；bVisable 是否显示弹窗
 * 返回值：bool
 *****************************************************************************/
bool WordEngine::open(const QString& strFilePath, bool bVisable)
{
    m_strFilePath = strFilePath;
    close();
    return open(bVisable);
}
 
/******************************************************************************
 * 函数：close
 * 功能：关闭文件
 * 参数：无
 * 返回值：bool
 *****************************************************************************/
bool WordEngine::close()
{
//    qDebug()<<m_bOpened;
    if (m_bOpened)
    {
        if(m_wordDocuments)
            m_wordDocuments->dynamicCall("Close (bool)", true);//关闭文本窗口
        if(m_wordWidget)
            m_wordWidget->dynamicCall("Quit()");//退出word
        if(m_wordDocuments)
            delete m_wordDocuments;
        if(m_wordWidget)
            delete m_wordWidget;
        m_bOpened = false;
    }
 
    return m_bOpened;
}
 
/******************************************************************************
 * 函数：isOpen
 * 功能：获得当前的打开状态
 * 参数：无
 * 返回值：bool
 *****************************************************************************/
bool WordEngine::isOpen()
{
    return m_bOpened;
}
 
/******************************************************************************
 * 函数：save
 * 功能：保存文件
 * 参数：无
 * 返回值：void
 *****************************************************************************/
void WordEngine::save()
{
    m_wordDocuments->dynamicCall("Save()");
}
 
/******************************************************************************
 * 函数：saveAs
 * 功能：另存文件
 * 参数：strSaveFile 文件路径
 * 返回值：void
 *****************************************************************************/
bool WordEngine::saveAs(const QString& strSaveFile)
{
    return m_wordDocuments->dynamicCall("SaveAs (const QString&)",
                                 strSaveFile).toBool();
}
 
/******************************************************************************
 * 函数：setMarks
 * 功能：设置标签内容
 * 参数：strMark 标签名；strContent 文本
 * 返回值：bool
 *****************************************************************************/
bool WordEngine::setMarks(const QString& strMark, const QString& strContent)
{
    QAxObject* bookmarkCode = NULL;
    bookmarkCode = m_wordDocuments->querySubObject("Bookmarks(QVariant)", strMark);
    //选中标签，将字符textg插入到标签位置
    if(bookmarkCode)
    {
        bookmarkCode->dynamicCall("Select(void)");
        bookmarkCode->querySubObject("Range")->setProperty("Text", strContent);
        return true;
    }
 
    return false;
}
 
/******************************************************************************
 * 函数：setTitle
 * 功能：插入标题 3
 * 参数：strMark 标签名；strContent 文本
 * 返回值：bool
 *****************************************************************************/
bool WordEngine::setTitle(const QString& strMark, const QString& strContent)
{
    QAxObject* bookmarkCode = NULL;
    bookmarkCode = m_wordDocuments->querySubObject("Bookmarks(QVariant)", strMark);
    //选中标签，将字符textg插入到标签位置
    if(bookmarkCode)
    {
        bookmarkCode->dynamicCall("Select(void)");
        bookmarkCode->querySubObject("Range")->setProperty("Text", strContent+"\n");
        bookmarkCode->querySubObject("Range")->dynamicCall("SetStyle(QVariant)", QString::fromLocal8Bit("标题 3"));
        return true;
    }
 
    return false;
}
/******************************************************************************
 * 函数：setBatchMarks
 * 功能：批量设置标签内容
 * 参数：itemList 标签名列表；sometexts 内容列表
 * 返回值：bool
 *****************************************************************************/
bool WordEngine::setBatchMarks(QStringList &itemList,QStringList sometexts)
{
    QAxObject* allBookmarks = NULL;
    allBookmarks = m_wordDocuments->querySubObject("Bookmarks");
    //选中标签，将字符textg插入到标签位置
    if(allBookmarks)
    {
        int count = allBookmarks->property("Count").toInt();
        /*填写模板中的书签*/
        for(int i=count;i>0;--i){
            QAxObject *bookmark = allBookmarks->querySubObject("Item(QVariant)", i);
            QString name= bookmark->property("Name").toString();
            int j=0;
            foreach(QString itemName , itemList){
                if (name == itemName) {
                    QAxObject *curBM = m_wordDocuments->querySubObject("Bookmarks(QString)", name);
                    curBM->querySubObject("Range")->setProperty("Text", sometexts.at(j));
                    break;
                }
                j++;
            }
        return true;
    }
 
    return false;
}
}
 
/******************************************************************************
 * 函数：insertTable
 * 功能：创建表格
 * 参数：nStart 开始位置; nEnd 结束位置; row 行; column 列
 * 返回值： void
 *****************************************************************************/
void WordEngine::insertTable(int nStart, int nEnd, int row, int column)
{
      QAxObject* ptst = m_wordDocuments->querySubObject( "Range( Long, Long )",
                                                         nStart, nEnd );
       QAxObject* pTable = m_wordDocuments->querySubObject( "Tables" );
       QVariantList params;
       params.append(ptst->asVariant());
       params.append(row);
       params.append(column);
       if( pTable )
       {
         pTable->dynamicCall( "Add(QAxObject*, Long ,Long )",params);

       }
}

QAxObject *WordEngine::insertTable(QString sLabel,int row, int column,QString sTitle){
    QAxObject *bookmark = m_wordDocuments->querySubObject("Bookmarks(QVariant)", sLabel);
    if(bookmark)
     {
       bookmark->dynamicCall("Select(void)");
       QAxObject *selection = m_wordWidget->querySubObject("Selection");

       selection->dynamicCall("InsertAfter(QString&)", "\n");
       //selection->dynamicCall("MoveLeft(int)", 1);
       //selection->querySubObject("ParagraphFormat")->dynamicCall("Alignment", "wdAlignParagraphCenter");
       //selection->dynamicCall("TypeText(QString&)", "Table Test");//设置标题
       selection->dynamicCall("TypeText(QString&)", sTitle);
       QAxObject *range = selection->querySubObject("Range");
       QAxObject *tables = m_wordDocuments->querySubObject("Tables");
       QAxObject *table = tables->querySubObject("Add(QVariant,int,int)",range->asVariant(),row,column);

       for(int i=1;i<=6;i++)
       {
           QString str = QString("Borders(-%1)").arg(i);
           QAxObject *borders = table->querySubObject(str.toLocal8Bit().constData());
           borders->dynamicCall("SetLineStyle(int)",1);
       }
       return table;
     }
     return NULL;
}

/******************************************************************************
 * 函数：insertPic
 * 功能：插入图片
 * 参数：sLable 标签名；picPath 图片路径
 * 返回值：void
 *****************************************************************************/
void WordEngine::insertPic(QString sLabel,const QString& picPath)
{
    QAxObject *bookmark=m_wordDocuments->querySubObject("Bookmarks(QVariant)",sLabel);
    // 选中标签，将图片插入到标签位置
    if(bookmark){
        bookmark->dynamicCall("Select(void)");
        QAxObject *selection = m_wordWidget->querySubObject("Selection");
        selection->querySubObject("ParagraphFormat")->dynamicCall("Alignment", "wdAlignParagraphCenter");
        QAxObject *range = bookmark->querySubObject("Range");
        QVariant tmp = range->asVariant();
        QList<QVariant>qList;
        qList<<QVariant(picPath);
        qList<<QVariant(false);
        qList<<QVariant(true);
        qList<<tmp;
        QAxObject *Inlineshapes = m_wordDocuments->querySubObject("InlineShapes");
        Inlineshapes->dynamicCall("AddPicture(const QString&, QVariant, QVariant ,QVariant)",qList);
    }

}

/******************************************************************************
 * 函数：MergeCells
 * 功能：合并单元格
 * 参数：table 表格; nStartRow 起始单元格行数; nStartCol ; nEndRow ; nEndCol
 * 返回值：void
 *****************************************************************************/
void WordEngine::MergeCells(QAxObject *table, int nStartRow,int nStartCol,int nEndRow,int nEndCol)
{
    QAxObject* StartCell =table->querySubObject("Cell(int, int)",nStartRow,nStartCol);
    QAxObject* EndCell = table->querySubObject("Cell(int, int)",nEndRow,nEndCol);
    StartCell->dynamicCall("Merge(LPDISPATCH)",EndCell->asVariant());
}

/******************************************************************************
 * 函数：setColumnWidth
 * 功能：设置表格列宽
 * 参数：table 表格; column 列数; width 宽度
 * 返回值：void
 *****************************************************************************/
void WordEngine::setColumnWidth(QAxObject *table, int column, int width)
{
    table->querySubObject("Columns(int)", column)->setProperty("Width", width);
}

/******************************************************************************
 * 函数：setCellString
 * 功能：设置表格内容
 * 参数：table 表格; row 行数; column 列数; text 插入文本
 * 返回值：void
 *****************************************************************************/
void WordEngine::setCellString(QAxObject *table, int row, int column, const QString& text)
{
    if(table)
    {
        table->querySubObject("Cell(int, int)", row, column)
                ->querySubObject("Range")
                ->dynamicCall("SetText(QString)", text);
    }
}

/******************************************************************************
 * 函数：addTableRow
 * 功能：为表格添加行
 * 参数：table 表格; nRow 插入行; rowCount 插入的行数
 * 返回值：void
 *****************************************************************************/
void WordEngine::addTableRow(QAxObject *table, int nRow, int rowCount)
{
    QAxObject* rows = table->querySubObject("Rows");

    int Count = rows->dynamicCall("Count").toInt();
    if(0 <= nRow && nRow <=Count)
    {
        for(int i = 0; i < rowCount; ++i)
        {
            QString sPos = QString("Item(%1)").arg(nRow + i);
            QAxObject* row = rows->querySubObject(sPos.toStdString().c_str());
            QVariant param = row->asVariant();
            rows->dynamicCall("Add(Variant)", param);
        }
    }
}


/******************************************************************************
 * 函数：setCellFontBold
 * 功能：设置内容粗体  isBold控制是否粗体
 * 参数：table 表格; row 插入行; column 列数; isBold 是否加粗
 * 返回值：void
 *****************************************************************************/
void WordEngine::setCellFontBold(QAxObject *table, int row, int column, bool isBold)
{
    table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range")
            ->dynamicCall("SetBold(int)", isBold);
}

/******************************************************************************
 * 函数：setCellFontSize
 * 功能：设置文字大小
 * 参数：table 表格; row 插入行; column 列数; size 字体大小
 * 返回值：void
 *****************************************************************************/
void WordEngine::setCellFontSize(QAxObject *table, int row, int column, int size)
{
    table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range")
            ->querySubObject("Font")->setProperty("Size", size);
}

/******************************************************************************
 * 函数：insertCellPic
 * 功能：在表格中插入图片
 * 参数：table 表格; row 插入行; column 列数; picPath 图片路径
 * 返回值：void
 *****************************************************************************/
void WordEngine::insertCellPic(QAxObject *table, int row, int column,
                               const QString& picPath)
{
    QAxObject* range = table->querySubObject("Cell(int, int)", row, column)
            ->querySubObject("Range");
    range->querySubObject("InlineShapes")
            ->dynamicCall("AddPicture(const QString&)", picPath);
}
