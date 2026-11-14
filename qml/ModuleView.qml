import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Rectangle {
    property string heading
    property string description
    color: Theme.panel; radius: Theme.radius
    implicitHeight: content.implicitHeight + 40
    ColumnLayout {
        id: content; anchors.fill: parent; anchors.margins: Theme.spacing; spacing: 12
        Label {text: heading; color: Theme.text; font.pixelSize: 26; font.bold: true}
        Label {text: description; color: Theme.muted; wrapMode: Text.WordWrap; Layout.fillWidth: true}
        Label {text: "Phase 1 · Module placeholder"; color: Theme.accent}
    }
}
