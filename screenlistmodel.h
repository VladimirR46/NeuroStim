#ifndef SCREENLISTMODEL_H
#define SCREENLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QApplication>

class ScreenListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ScreenListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & parent = {}) const override;
    QVariant data(const QModelIndex & index = {}, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setCurrentScreen(int i);
    Q_INVOKABLE int getCurrentScreen();

    QScreen *currentScreen();

private:
    enum ScreenRole{
        NameRole = Qt::UserRole + 1,
    };

    QList<QScreen*> m_screens;
    int current_screen = 0;
};

#endif // SCREENLISTMODEL_H
