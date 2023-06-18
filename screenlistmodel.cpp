#include "screenlistmodel.h"
#include <QScreen>

ScreenListModel::ScreenListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_screens = QGuiApplication::screens();
}

int ScreenListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return m_screens.size();
}

QVariant ScreenListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if (role == ScreenRole::NameRole)
        return m_screens.at(index.row())->name();

    return QVariant();
}

QHash<int, QByteArray> ScreenListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ScreenRole::NameRole] = "name";
    return roles;
}

void ScreenListModel::setCurrentScreen(int i)
{
    if(i < m_screens.size())
        current_screen = i;
}

int ScreenListModel::getCurrentScreen()
{
    return current_screen;
}

QScreen *ScreenListModel::currentScreen()
{
    return m_screens.at(current_screen);
}
