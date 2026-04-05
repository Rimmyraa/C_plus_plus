#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Basic");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/TimerProject/Main.qml"_qs);
    engine.load(url);

    return app.exec();
}
