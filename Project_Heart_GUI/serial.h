#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QThread>
#include <QString>
#include <QDebug>
#include <QByteArray>

class Serial : public QThread
{
    Q_OBJECT
    Q_PROPERTY(int heart READ heart NOTIFY heartChanged)
    Q_PROPERTY(float alcohol READ alcohol NOTIFY alcoholChanged)
public:
    Serial(QThread *parent = nullptr);
    int heart() const;
    float alcohol() const;


protected:
     void run(void); // receive Data to STM32 in Thread for the GUI QML

public slots:
    bool init_port(QString port);
    void transmitData(unsigned char data);

signals:
    void alcoholChanged();
    void heartChanged();

private:
    QSerialPort *m_serial; // Objet pour la communication série
    int m_heart = 0;
    float m_alcohol = 0;
};

#endif // SERIAL_H
