#pragma once
#include <QObject>
#include <QVariantList>
class DatasetRegistry : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList datasets READ datasets NOTIFY changed)
public:
    explicit DatasetRegistry(QString root, QObject* parent=nullptr);
    QVariantList datasets() const { return m_datasets; }
    Q_INVOKABLE void refresh();
signals:
    void changed();
private:
    QString m_root;
    QVariantList m_datasets;
};
