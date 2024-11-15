#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "primecounter.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    qmlRegisterType<PrimeCounter>("com.primecounter", 1,0, "PrimeCounter");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/PrimeCounter/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
