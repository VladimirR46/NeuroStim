#include "appengine.h"
#include <QApplication>
#include <QQuickStyle>
#include <QQmlEngine>

int main(int argc, char *argv[])
{ 
    QApplication a(argc, argv);

    QQuickStyle::setStyle("Basic");

    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    AppEngine engine(&view);

    QObject::connect(view.engine(), &QQmlEngine::quit, &a, &QApplication::quit );

    view.setMinimumSize(QSize(500,500));
    view.show();
    return a.exec();
}
