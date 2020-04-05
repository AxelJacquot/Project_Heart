import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3



ColumnLayout{
    width: main.width
    height: main.height
    spacing: 20
    Layout.margins: 5

    Text{
        text: qsTr("Serial Connexion to STM32")
        font.pointSize: 18
        Layout.alignment: Qt.AlignCenter
    }
    GridLayout{
        Layout.alignment: Qt.AlignCenter
        columnSpacing: 10
        columns: 2
        Text {
            text: qsTr("Port Connect")
        }
        TextField{
            width: 20
            id: port
            placeholderText: "/dev/ttyUSB0"
        }
    }

    Button{
        Layout.alignment: Qt.AlignCenter
        text: qsTr("Connect")
        onClicked: {
            var error = Serial.init_port(port.text)
            if(error === false){
                Serial.transmitData(55);
                Serial.start();
                main.connect = true;
            }
            else{
                console.log("error")
            }
        }
    }

}

