#pragma once
#include <QObject>
#include "data/DatasetRegistry.h"
class ApplicationServices : public QObject {
    Q_OBJECT
    Q_PROPERTY(DatasetRegistry* registry READ registry CONSTANT)
    Q_PROPERTY(int currentModule READ currentModule WRITE setCurrentModule NOTIFY currentModuleChanged)
public:
    explicit ApplicationServices(const QString& root,QObject* parent=nullptr):QObject(parent),m_registry(root,this){}
    DatasetRegistry* registry(){return &m_registry;}
    int currentModule() const{return m_module;}
    void setCurrentModule(int index){if(index<0 || index>2 || index==m_module)return;m_module=index;emit currentModuleChanged();}
signals:
    void currentModuleChanged();
private:
    DatasetRegistry m_registry;
    int m_module=0;
};
