#ifndef MAILAGENT_H
#define MAILAGENT_H

#include <QObject>
#include "historylistmodel.h"

class MailAgent : public QObject
{
    Q_OBJECT
public:
    explicit MailAgent(QObject *parent = nullptr);

    Q_INVOKABLE void sendMail(QString recipient, QString text, Experiment experiment);

signals:

};

#endif // MAILAGENT_H
