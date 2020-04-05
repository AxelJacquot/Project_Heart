#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "serial.h"

int main(int argc, char *argv[])
{
    Serial *serial = new Serial;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("Serial", serial); //Création du contexte Serial pour la communication entre le programme QML et C++

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);   //Chargement de l'application

    return app.exec();
}
