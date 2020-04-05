import QtQuick 2.14
import QtQuick.Window 2.14



Window {
    visible: true
    height: 438
    width: 480
    minimumHeight: height
    minimumWidth: width
    maximumHeight: height
    maximumWidth: width
    title: qsTr("Heart Project")
    id: main
    property bool connect: false

    Connexion{
        visible: !connect
    }

    Sensor{
        visible: connect
    }
}
