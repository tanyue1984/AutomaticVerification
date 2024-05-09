#include "testdataitem.h"
#include "id/TaskManager.h"
#include "basecommonapi.h"

// 定义并初始化静态成员变量 委托单编号
QMap<QString, int> TaskManager::dailyTaskCounts;
QDate TaskManager::currentDate;
QString TaskManager::KeyDate;

//检定结果保存
QMap<QString, int> TaskManager::saveResultdatanoList;
QString TaskManager::ResultKeyDate;

// 核查结果数据单编号
QMap<QString, int> TaskManager::checkResultDataNoMap;
QString TaskManager::checkDataKeyDate;

QString TaskManager::generateTask(QString Number)
{
    // 初始化当天任务单数量为0
    QString TaskNum=Number;
    currentDate = QDate::currentDate();
    KeyDate = currentDate.toString("yyyyMMdd");
    if(!dailyTaskCounts.contains(KeyDate))
    {

        int startIndex = TaskNum.indexOf("d")+1; // 找到 "YYYY" 的起始位置
        int endIndex = startIndex + 8; // "YYYYMMDD" 总共8个字符
        QString result = TaskNum.mid(startIndex, 8); // 从 startIndex 开始，截取8个字符
        if(result!=KeyDate){
             dailyTaskCounts[KeyDate] = 1;
        }else
        {
        QChar lastChar = TaskNum.at(TaskNum.length() - 1);

        if (lastChar.isDigit()) {

            int Count=lastChar.digitValue();
            dailyTaskCounts[KeyDate] = Count+1;
                }

        }

    }
    QString dateStr = KeyDate;
    // 生成任务单编号，这里假设编号格式是 "rwdYYYYMMDDXXXX"，XXXX是顺序号  字段4  10进制 不足时0填充
    QString taskId = QString("zdrwd%1%2").arg(dateStr).arg(dailyTaskCounts[KeyDate], 4, 10, QChar('0'));

    return taskId;
}
QString TaskManager::getsaveResultdatano()
{

    //获取结果保存ID
    ResultKeyDate = QDate::currentDate().toString("yyyyMMdd");
    //判断是否包含 如包含直接返回结果
    if(!saveResultdatanoList.contains(ResultKeyDate))
    {

        //找寻数据库上次的记录
      QString datano=BaseCommonApi::getNewResultData().data_no;
      if(datano!=""){
            int startIndex = datano.lastIndexOf("j")+1; // 找到 "YYYY" 的起始位置
            int endIndex = startIndex + 8; // "YYYYMMDD" 总共8个字符
            //截截取
            QString result = datano.mid(startIndex, 8);
            if(result!=ResultKeyDate){
                 saveResultdatanoList[ResultKeyDate] = 1;
            }else
            {
                QString lastSeqNo = datano.right(4); // 取最后的4位
                bool succ = false;
                int Count = lastSeqNo.toInt(&succ);
                if (succ == true) {
                    saveResultdatanoList[ResultKeyDate] = Count+1;
                }

            }

        }else{saveResultdatanoList[ResultKeyDate] = 1;};

    }

    QString dateStr =  ResultKeyDate;
    // 生成结果保存编号，这里假设编号格式是 "jdsjYYYYMMDDXXXX"，XXXX是顺序号  字段4  10进制 不足时0填充
    QString taskId = QString("zdjdsj%1%2").arg(dateStr).arg(saveResultdatanoList[ResultKeyDate], 4, 10, QChar('0'));
    //返回结果
    return  taskId;
}


/******** 20240502 tanyue添加 ********/
// 生成新的自动核查数据单编号
QString TaskManager::getCheckResultDataNo()
{
    //获取结果保存ID
    checkDataKeyDate = QDate::currentDate().toString("yyyyMMdd");
    //判断是否包含 如包含直接返回结果
    if (checkResultDataNoMap.contains(checkDataKeyDate) == false)
    {
        //找寻数据库上次的记录
        QString datano = BaseCommonApi::getNewCheckResultData().data_no;
        if(datano != "") {
            int startIndex = datano.lastIndexOf("j") + 1; // 找到 "YYYY" 的起始位置

            //截截取
            QString date = datano.mid(startIndex, 8);
            if (date != checkDataKeyDate) {
                // 当天没有自动核查数据单 从1开始编号
                checkResultDataNoMap[checkDataKeyDate] = 1;
            } else {
                // 获取最后4位数 并加1
                QString seqNo = datano.right(4);
                bool succ = false;
                int count = seqNo.toInt(&succ);
                if (succ == true) {
                    checkResultDataNoMap[checkDataKeyDate] = count + 1;
                }
            }
        } else {
            checkResultDataNoMap[checkDataKeyDate] = 1;
        }
    }

    QString dateStr =  checkDataKeyDate;
    // 生成结果保存编号，这里假设编号格式是 "zdhcsjYYYYMMDDXXXX"，XXXX是顺序号  字段4  10进制 不足时0填充
    QString checkDataNo = QString("zdhcsj%1%2").arg(dateStr).arg(checkResultDataNoMap[checkDataKeyDate], 4, 10, QChar('0'));
    //返回结果
    return  checkDataNo;
}
/******** 20240502 ********/


bool TaskManager::AddCurrenTaskCount()
{
    // 增加当天任务单数量
    try {
        dailyTaskCounts[KeyDate]++;
      return  true;
    } catch(const std::exception &e) {
        return  false;
    }
}
bool TaskManager::AddResultDataNoCount()
{
    // 增加当天任务单数量
    try {
        saveResultdatanoList[ResultKeyDate]++;
      return  true;
    } catch(const std::exception &e) {
        return  false;
    }
}











