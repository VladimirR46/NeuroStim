#include "mailagent.h"
#include <QDebug>
#include "sender.h"
#include "mimemessage.h"
#include "mimetext.h"
#include "mimeattachment.h"

MailAgent::MailAgent(QObject *parent)
    : QObject{parent}
{

}

void MailAgent::sendMail(QString recipient, QString text, Experiment experiment)
{
    if(recipient.isEmpty() || text.isEmpty()) return;

    SimpleMail::Sender sender("smtp.mail.ru", 465, SimpleMail::Sender::SslConnection);

    sender.setUser("neurostim@mail.ru");
    sender.setPassword("90vaCkKHBkXmzLNURiTi");

    // Now we create a MimeMessage object. This will be the email.
    SimpleMail::MimeMessage message;
    message.setSender(SimpleMail::EmailAddress("neurostim@mail.ru", "NeuroStim"));
    message.addTo(SimpleMail::EmailAddress(recipient));
    message.setSubject(experiment.name);

    // First we create a MimeText object.
    auto mail_text = new SimpleMail::MimeText;

    // Now add some text to the email.
    mail_text->setText(text);
    message.addPart(mail_text);


    message.addPart(new SimpleMail::MimeAttachment(new QFile(experiment.path+"/LslEvents.json")));
    message.addPart(new SimpleMail::MimeAttachment(new QFile(experiment.path+"/Task.json")));

    if (!sender.sendMail(message)) {
        qDebug() << "Failed to send mail!" << sender.lastError();
    }
}
