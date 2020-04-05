#include "serial.h"
#include <QDebug>

Serial::Serial(QThread *parent) : QThread(parent)
{
    m_serial = new QSerialPort(this);
}

int Serial::heart() const
{
    return m_heart;
}

float Serial::alcohol() const
{
    return m_alcohol;
}

bool Serial::init_port(QString port)
{
    m_serial->setPortName(port);
    m_serial->setBaudRate(QSerialPort::Baud115200); //Réglage de la vitesse de la communication
    m_serial->setDataBits(QSerialPort::Data8);  //Réglage de la taille de la donnée
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    if(!m_serial->open(QIODevice::ReadWrite)) {     //Ouvre la communication série et test si elle est possible
        qDebug() <<QObject::tr("Je ne peux pas ouvrir le port série %1...").arg(m_serial->portName());
        return true;
    }
    return false;
}

void Serial::run()
{
    while(1){
        if(!m_serial->waitForReadyRead(-1)) //block until new data arrives
            qDebug() << "error: " << m_serial->errorString();
        else{
            qDebug() << "New data available: " << m_serial->bytesAvailable();
            QByteArray buf = m_serial->readAll();
            if(buf != ""){
                //qDebug() << buf << endl;
                unsigned char code = buf[0];
                unsigned char data = buf[1];

                if(code == 0x55){
                    qDebug() << "Heart Rate";
                    m_heart = data;
                    Q_EMIT heartChanged();
                }
                else if(code == 0xff) {
                    m_alcohol = (double)data/10;
                    Q_EMIT alcoholChanged();
                }
            }
            buf.clear();
        }
    }
}

void Serial::transmitData(unsigned char data)
{
    QByteArray buf;
    buf.clear();
    buf.append(data);
    m_serial->write(buf);
}
