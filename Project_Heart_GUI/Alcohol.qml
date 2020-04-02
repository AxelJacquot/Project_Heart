import QtQuick 2.0

Item {
    id: alcohol
    width: main.width
    height: main. width

    property real rate: 0

    Connections{
        target: Serial
        onAlcoholChanged:{
            rate = Serial.alcohol
            alcoholRate.text = rate.toFixed(1)
        }
    }

    Image {
        id: alcoholImage
        source: "Image/beer.png"
        sourceSize.width: 480
        sourceSize.height: 438
        x: (main.width - alcoholImage.width) / 2
        y: ((main.height - alcoholImage.height) / 2)
    }

    Text {
        id: alcoholRate
        text: qsTr("2,0")
        font.pointSize: 70
        x: ((main.width - alcoholRate.width) / 2) - 40
        y: ((main.height - alcoholRate.height) / 2) + 60
    }

    Text {
        text: qsTr("mg/L")
        font.pointSize: 40
        x: ((main.width - alcoholRate.width) / 2) - 20
        y: ((main.height - alcoholRate.height) / 2) + 170
    }

}
