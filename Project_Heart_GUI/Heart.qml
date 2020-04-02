import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.5



Item{
    id: heart
    width: main.width
    height: main. width

    Connections{
        target: Serial
        onHeartChanged:{
            heartRate.text = Serial.heart
            heartRate.x = (main.width - heartRate.width) / 2
            heartRate.y = ((main.height - heartRate.height) / 2) - 60
        }
    }


    /*function updateHeart(rate){
        console.log(rate)
        heartRate.text = rate
        heartRate.x = (main.width - heartRate.width) / 2
        heartRate.y = ((main.height - heartRate.height) / 2) - 60
    }*/

    Image{
        id: heartImage
        Layout.alignment: Qt.AlignCenter
        source: "Image/heartBlack.png"
        sourceSize.width: 480
        sourceSize.height: 438
    }

    Text {
        id: heartRate
        text: qsTr("0")
        font.pointSize: 120
        x: (main.width - heartRate.width) / 2
        y: ((main.height - heartRate.height) / 2) - 60
        /*Component.onCompleted: {
            Serial.receiveHeart.connect(updateHeart)
        }*/
    }

    Text {
        id: bpm
        text: qsTr("Bpm")
        font.pointSize: 60
        x: (main.width - bpm.width) / 2
        y: ((main.height - bpm.height) / 2) + 80
    }
}
