#ifndef QT6SPEAKER_H
#define QT6SPEAKER_H

#include <QObject>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QAudioSource>
#include <QAudioSink>
#include "Qt6ConfigContext.h"



class QT6FFMPEG_EXPORT Qt6Speaker : public QObject
{
    Q_OBJECT
public:
    explicit Qt6Speaker(QObject *parent = nullptr);
    QList<QAudioDevice> getAudioDevices();
    QAudioFormat format();
    void destroy();
    void init(const QAudioFormat &format, const QString &description=nullptr);
    void create();
public slots:
    void write(const QByteArray &data);

private:
    QAudioDevice defaultDevice;
    QAudioFormat defaultFormat;
    QMediaDevices *qMediaDevices;
    QAudioSink* qAudioSink;
    QIODevice *qIODevice;
    QList<QAudioDevice> listDevices;
signals:
    void finished();
};

#endif // QT6SPEAKER_H
