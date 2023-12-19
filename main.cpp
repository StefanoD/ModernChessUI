#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Board.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Board board;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("_board"), &board);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("ModernChessUI", "Main");

    QObject *object = engine.rootObjects().first()->findChild<QObject*>("board", Qt::FindChildrenRecursively);

    QObject::connect(object, SIGNAL(boardRotated()),
                     &board, SLOT(onRotate()));

    return app.exec();
}
