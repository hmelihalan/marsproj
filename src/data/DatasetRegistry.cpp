#include "DatasetRegistry.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
DatasetRegistry::DatasetRegistry(QString root,QObject* parent):QObject(parent),m_root(std::move(root)){refresh();}
void DatasetRegistry::refresh() {
    QVariantList next;
    const QStringList labels={"ctx-dem.xml","ctx-ortho.xml"};
    const QStringList files={"dem_20m.tif","ctx_5m.tif"};
    const QStringList sources={
        "https://astrogeology.usgs.gov/ckan/dataset/f8ef46b8-d512-4c46-b825-fe9633c38e1c/resource/5691ea08-1db4-4018-a341-e415e716add7/download/m20_jezerocrater_ctxdem_20m.xml",
        "https://astrogeology.usgs.gov/ckan/dataset/f8ef46b8-d512-4c46-b825-fe9633c38e1c/resource/d139f28f-9976-40b1-aa55-49b78cebf055/download/m20_jezerocrater_ctxortho_mosaic_5m.xml"};
    for(int i=0;i<labels.size();++i) {
        QVariantMap item;
        const QString path=QDir(m_root).filePath("pilot_v1/pilot_v1/"+files[i]);
        item["name"]=i==0?"CTX elevation / DEM":"CTX optical orthomosaic";
        item["path"]=path;
        item["available"]=QFileInfo(path).isFile() && QFileInfo(path).isReadable();
        item["source"]=sources[i];
        item["limitations"]=i==0?
            "Precise CTX vertical datum and independent feature registration remain unresolved. 20 m terrain cannot resolve rock-scale hazards.":
            "PDS label declares zero missing; TIFF omits NoData. Phase 0 found 8,277 missing pilot pixels. Independent feature registration remains unresolved.";
        item["verification"]="Phase 0 recorded exact parent pixel identity; current files have not been revalidated by this application.";
        QFile file(QDir(m_root).filePath("data/metadata/"+labels[i]));
        QVariantMap metadata;
        QString error;
        if(!file.open(QIODevice::ReadOnly)) error="Authoritative local label unavailable";
        else if(file.size()>1024*1024) error="Metadata exceeds the supported 1 MiB label size";
        else {
            QXmlStreamReader xml(&file);
            const QStringList keys={"logical_identifier","version_id","map_projection_name","latitude_type","longitude_direction","a_axis_radius","pixel_resolution_x","pixel_resolution_y","missing_constant","file_name"};
            while(!xml.atEnd()) {
                xml.readNext();
                if(xml.isStartElement() && keys.contains(xml.name().toString())) {
                    const auto key=xml.name().toString(); metadata[key]=xml.readElementText();
                }
            }
            if(xml.hasError()) error="Malformed metadata: "+xml.errorString();
            else for(const auto& key:keys) if(metadata.value(key).toString().isEmpty()) {error="Incomplete metadata: "+key;break;}
            const QString expected="urn:nasa:pds:mars2020_ctx_basemap_2021:data:"+QString(i==0?"m20_ctxdem_mosaic":"m20_ctxortho_mosaic");
            if(error.isEmpty() && metadata.value("logical_identifier").toString()!=expected)
                error="Metadata product identity does not match the registry entry";
        }
        item["metadataReady"]=error.isEmpty(); item["error"]=error;
        item["metadata"]=error.isEmpty()?metadata:QVariantMap{};
        next.append(item);
    }
    m_datasets=next; emit changed();
}
