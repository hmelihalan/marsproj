#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QDir>
#include "app/ApplicationServices.h"
#include "gis/PlanetaryContext.h"
int main(int argc,char** argv){
    QGuiApplication app(argc,argv);
    app.setApplicationName("MarsPath");
    QCommandLineParser parser; parser.addHelpOption();
    parser.addOption({"data-root","Repository/data root containing data/metadata and pilot_v1.","directory",QDir::currentPath()});
    parser.process(app);
    ApplicationServices services(parser.value("data-root"));
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("services",&services);
    engine.rootContext()->setContextProperty("geospatialRuntime",MarsPath::geospatialRuntime());
    engine.load(QUrl("qrc:/qml/Main.qml"));
    if(engine.rootObjects().isEmpty())return 1;
    return app.exec();
}
