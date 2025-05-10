#include "Qt6Speaker.h"
#include <QDebug>
#include <QAudioSink>

Qt6Speaker::Qt6Speaker(QObject *parent)
    : QObject{parent}
{
    defaultDevice = QMediaDevices::defaultAudioOutput();
    defaultFormat.setSampleRate(16000);
    defaultFormat.setChannelCount(2);
    defaultFormat.setSampleFormat(QAudioFormat::Int16);
    qMediaDevices=new QMediaDevices(this);
    listDevices= qMediaDevices->audioOutputs();
    for (auto &audioDevice : listDevices){
        qInfo() << "可用扬声器:" <<     audioDevice.description();
    }

}


QList<QAudioDevice> Qt6Speaker::getAudioDevices()
{
    return listDevices;
}

void Qt6Speaker::init(const QAudioFormat &format,const QString &description)
{
    defaultFormat=format;

    for (auto &currentDevice : listDevices){
        if(currentDevice.description() == description){
            defaultDevice=currentDevice;
            break;
        }
    }
}

QAudioFormat Qt6Speaker::format()
{
    return defaultFormat;
}

void Qt6Speaker::destroy()
{
    if(qAudioSink){
        qAudioSink->stop();
        delete qAudioSink;
    }
}

void Qt6Speaker::create()
{

    if (!defaultDevice.isFormatSupported(defaultFormat)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
    }
    qAudioSink= new QAudioSink(defaultDevice, defaultFormat,this);
    qInfo() << tr("Speakers - %1").arg(defaultDevice.description());
    qIODevice=qAudioSink->start();


}

void Qt6Speaker::write(const QByteArray &audio_bytes)
{
    if(!audio_bytes.isEmpty() && qIODevice){
        qIODevice->write(audio_bytes);
    }
}
