import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
ApplicationWindow {
    id: window; objectName: "mainWindow"
    width: 1100; height: 820; minimumWidth: 700; minimumHeight: 500
    visible: true; title: "MarsPath · Jezero workspace"; color: Theme.background
    palette.window: Theme.background; palette.windowText: Theme.text
    palette.button: Theme.panel; palette.buttonText: Theme.text
    palette.highlight: Theme.accent
    header: ToolBar {
        background: Rectangle {color: Theme.panel}
        RowLayout {
            anchors.fill: parent; anchors.margins: 8
            Label {text: "MARSPATH"; font.bold: true; color: Theme.accent; Layout.leftMargin: 12}
            Item {Layout.fillWidth: true}
            Repeater {
                model: ["Exploration", "Mission Planning", "Scientific Discovery"]
                Button {
                    required property int index
                    required property string modelData
                    objectName: "nav" + index
                    text: modelData; checkable: true; checked: services.currentModule === index
                    onClicked: services.currentModule = index
                    Accessible.name: modelData
                }
            }
        }
    }
    ScrollView {
        anchors.fill: parent; contentWidth: availableWidth; clip: true
        ColumnLayout {
            width: parent.width; spacing: Theme.spacing
            Label {text: "JEZERO CRATER / DATA WORKSPACE"; color: Theme.muted; Layout.margins: 20}
            Loader {
                objectName: "moduleLoader"
                Layout.fillWidth: true; Layout.leftMargin: 20; Layout.rightMargin: 20
                source: ["ExplorationView.qml","MissionPlanningView.qml","ScientificDiscoveryView.qml"][services.currentModule]
            }
            Label {text: "Dataset registry"; color: Theme.text; font.pixelSize: 24; Layout.leftMargin: 20}
            Button {text: "Refresh local availability"; Layout.leftMargin: 20; onClicked: services.registry.refresh()}
            Repeater {
                model: services.registry.datasets
                Rectangle {
                    required property var modelData
                    Layout.fillWidth: true; Layout.leftMargin: 20; Layout.rightMargin: 20
                    implicitHeight: details.implicitHeight + 40; color: Theme.panel; radius: Theme.radius
                    ColumnLayout {
                        id: details; anchors.fill: parent; anchors.margins: 20; spacing: 10
                        Label {text: modelData.name; color: Theme.text; font.pixelSize: 20; font.bold: true}
                        Label {text: modelData.available ? "Local file present · scientific validity not rechecked" : "Local raster unavailable"; color: Theme.accent; wrapMode: Text.WordWrap; Layout.fillWidth: true}
                        Label {text: modelData.metadataReady ? modelData.metadata.logical_identifier + " · v" + modelData.metadata.version_id : modelData.error; color: Theme.muted; wrapMode: Text.WrapAnywhere; Layout.fillWidth: true}
                        Label {visible: modelData.metadataReady; text: modelData.metadataReady ? modelData.metadata.map_projection_name + " · Mars sphere " + modelData.metadata.a_axis_radius + " m · " + modelData.metadata.latitude_type + " · " + modelData.metadata.longitude_direction + "\nPixel spacing: " + modelData.metadata.pixel_resolution_x + " × " + modelData.metadata.pixel_resolution_y + " m" : ""; color: Theme.muted; wrapMode: Text.WordWrap; Layout.fillWidth: true}
                        Label {text: modelData.verification; color: Theme.muted; wrapMode: Text.WordWrap; Layout.fillWidth: true}
                        Label {visible: modelData.metadataReady; text: modelData.metadataReady ? "GeoTIFF · " + modelData.metadata.file_name + "\nPDS missing value: " + modelData.metadata.missing_constant + " · MRO / CTX · USGS / JPL science mosaic" : ""; color: Theme.muted; wrapMode: Text.WrapAnywhere; Layout.fillWidth: true}
                        Label {text: modelData.limitations; color: Theme.accent; wrapMode: Text.WordWrap; Layout.fillWidth: true}
                        Label {text: "PDS label: " + modelData.source; color: Theme.muted; wrapMode: Text.WrapAnywhere; Layout.fillWidth: true}
                    }
                }
            }
            Label {text: geospatialRuntime + " · Phase 1 skeleton"; color: Theme.muted; Layout.margins: 20}
        }
    }
}
