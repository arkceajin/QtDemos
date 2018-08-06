#include "qmlobjectlist.h"
#include <QMetaProperty>
#include <QQmlEngine>
#include <QDebug>

#define ValidateIndex(m_i) (m_i < 0 || m_i >= rowCount())

void QmlObjectList::append(const QVariantMap& properties)
{
    QObject* newObj = mMetaObject.newInstance();
    if(newObj == Q_NULLPTR) {
        qWarning("invalid constructor");
        return;
    }
    for(const QString& key : properties.keys()) {
        if(!newObj->setProperty(key.toUtf8().data(), properties.value(key)))
            qWarning()<<"append object with invalid property"<<key;
    }
    append(newObj);
}

void QmlObjectList::append(QObject *object)
{
    insert(rowCount(), object);
}

bool QmlObjectList::insert(const int& i, QObject* object)
{
    Q_ASSERT(object->metaObject() == &mMetaObject);
    if(i < 0 || i > rowCount())
        return false;
    beginInsertRows(QModelIndex(), i, i);
    mData.insert(i, QObjectPointer(object));
    endInsertRows();
    return true;
}

bool QmlObjectList::removeRows(int row, int count, const QModelIndex &parent)
{
    const int first = row;
    const int last = row + count - 1;
    if(ValidateIndex(first) || ValidateIndex(last))
        return false;
    beginRemoveRows(parent, first, last);
    mData.remove(row, count);
    endRemoveRows();
    return true;
}

bool QmlObjectList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(ValidateIndex(index.row()))
        return false;
    const QObjectPointer& data = mData[index.row()];
    if(data.isNull()) {
        qWarning()<<__FUNCTION__<<"data is null";
        return false;
    }
    if(data->setProperty(mRoles[role], value)) {
        dataChanged(index, index);
        return true;
    } else
        return false;
}

QVariant QmlObjectList::data(const QModelIndex &index, int role) const
{
    if(ValidateIndex(index.row()))
        return QVariant();
    const QObjectPointer& data = mData[index.row()];
    if(data.isNull()) {
        qWarning()<<__FUNCTION__<<"data is null";
        return QVariant();
    }
    return data->property(mRoles[role]);
}

QHash<int, QByteArray> QmlObjectList::roleNames() const
{
    return mRoles;
}

int QmlObjectList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mData.count();
}

QmlObjectList::QmlObjectList(const QMetaObject &m, QObject *parent) :
    QAbstractListModel(parent),
    mMetaObject(m),
    mRoles([this]() {
        QHash<int, QByteArray> roles;
        for(int i = 0; i < (mMetaObject.propertyCount() - mMetaObject.propertyOffset()); ++i) {
            roles[i] = QByteArray(mMetaObject.property(i + mMetaObject.propertyOffset()).name());
        }
        return roles;
    }())
{ }
