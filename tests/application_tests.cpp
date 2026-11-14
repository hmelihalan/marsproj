#include <QtTest>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickItem>
#include "app/ApplicationServices.h"
namespace {
// QML items are linked by visual parents (childItems), not always by QObject parents,
// so QObject::findChild cannot see delegates created by Repeater/Loader/ScrollView.
QObject* findByName(QQuickItem* item, const QString& name) {
 if(!item) return nullptr;
 if(item->objectName()==name) return item;
 for(QQuickItem* child:item->childItems()) if(auto* found=findByName(child,name)) return found;
 return nullptr;
}
}
class ApplicationTests:public QObject {
 Q_OBJECT
private slots:
 void initialization(){
   ApplicationServices service(MARSPATH_SOURCE_DIR);
   QCOMPARE(service.currentModule(),0); QCOMPARE(service.registry()->datasets().size(),2);
   QSignalSpy spy(&service,&ApplicationServices::currentModuleChanged);
   service.setCurrentModule(2); QCOMPARE(service.currentModule(),2);
   service.setCurrentModule(99); QCOMPARE(service.currentModule(),2); QCOMPARE(spy.count(),1);
   for(const auto& v:service.registry()->datasets()) {
     auto m=v.toMap(); QVERIFY(m["metadataReady"].toBool());
     QVERIFY(m["source"].toString().startsWith("https://astrogeology.usgs.gov/"));
     QVERIFY(m["limitations"].toString().contains("registration"));
     QCOMPARE(m["metadata"].toMap()["a_axis_radius"].toString(),QString("3396190"));
   }
 }
 void missingAndMalformed(){
   QTemporaryDir tmp; QVERIFY(tmp.isValid()); DatasetRegistry registry(tmp.path());
   for(auto v:registry.datasets()){QVERIFY(!v.toMap()["available"].toBool());QVERIFY(!v.toMap()["metadataReady"].toBool());}
   QDir(tmp.path()).mkpath("data/metadata");
   QFile file(tmp.filePath("data/metadata/ctx-dem.xml")); QVERIFY(file.open(QIODevice::WriteOnly)); file.write("<broken>"); file.close();
   QSignalSpy spy(&registry,&DatasetRegistry::changed); registry.refresh(); QCOMPARE(spy.count(),1);
   QVERIFY(registry.datasets()[0].toMap()["error"].toString().startsWith("Malformed"));
 }
 void availabilityIsSeparate(){
   QTemporaryDir tmp; QDir(tmp.path()).mkpath("pilot_v1/pilot_v1");
   QFile f(tmp.filePath("pilot_v1/pilot_v1/dem_20m.tif")); QVERIFY(f.open(QIODevice::WriteOnly));f.close();
   DatasetRegistry r(tmp.path()); QVERIFY(r.datasets()[0].toMap()["available"].toBool());
   QVERIFY(!r.datasets()[0].toMap()["metadataReady"].toBool());
   QVERIFY(QFile::remove(f.fileName()));r.refresh();QVERIFY(!r.datasets()[0].toMap()["available"].toBool());
 }
 void qmlNavigation(){
   ApplicationServices service(MARSPATH_SOURCE_DIR); QQmlApplicationEngine engine;
   engine.rootContext()->setContextProperty("services",&service);
   engine.rootContext()->setContextProperty("geospatialRuntime","Test runtime");
   QSignalSpy warnings(&engine,&QQmlEngine::warnings);
   engine.load(QUrl("qrc:/qml/Main.qml")); QCOMPARE(engine.rootObjects().size(),1);
   auto window=qobject_cast<QQuickWindow*>(engine.rootObjects()[0]); QVERIFY(window);
   for(int size:{700,1100}){window->resize(size,700);
     for(int index:{0,1,2,0,2,1}) {
       auto button=findByName(window->contentItem(),QString("nav%1").arg(index));QVERIFY(button);
       QVERIFY(QMetaObject::invokeMethod(button,"clicked")); QCOMPARE(service.currentModule(),index);
       QCoreApplication::processEvents();
       auto loader=findByName(window->contentItem(),"moduleLoader"); QVERIFY(loader);
       QCOMPARE(loader->property("status").toInt(),1);
     }
   }
   QCOMPARE(warnings.count(),0); window->close();
 }
};
QTEST_MAIN(ApplicationTests)
#include "application_tests.moc"
