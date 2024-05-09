#ifndef SEQUENTIALMAP_H
#define SEQUENTIALMAP_H
#include <QMap>
#include <QList>
#include <iterator>
#include <QDebug>
template <class Key, class T>
class SequentialMap : public QMap<Key, T>
{
public:
    using QMap<Key, T>::QMap; // 继承所有QMap的构造函数

    // 重载insert函数以更新_keys列表
    typename QMap<Key, T>::iterator insert(const Key &key, const T &value) {
         oldSize = this->count();
        auto it = QMap<Key, T>::insert(key, value);
         size=this->count();
        if (size > oldSize) {
            // 如果size增加了，说明键是新插入的
           qDebug() << QString::number(size);
            _keys.append(key);
          }
        // 返回插入位置的迭代器
        return it;
    }

    // 重载operator[]以使用insert函数
    T &operator[](const Key &key) {
        if (!this->contains(key)) {
            this->insert(key, T()); // 插入默认构造的值
        }
        return QMap<Key, T>::operator[](key);
    }

    // 重载const版本的operator[]以保持一致性
    const T &operator[](const Key &key) const {
        return QMap<Key, T>::operator[](key);
    }

    // 提供keys函数以返回按键插入顺序的键列表
    QList<Key> keys() const {
        return _keys;
    }

private:
    QList<Key> _keys;
    int oldSize;
    int size;
};
#endif // SEQUENTIALMAP_H
