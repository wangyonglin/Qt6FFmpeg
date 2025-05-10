#ifndef QT6DECODERAUDIO_H
#define QT6DECODERAUDIO_H

#include <QObject>
#include "Qt6Decoder.h"
#include "Qt6Demuxer.h"
#include "Qt6Resampler.h"
#include "Qt6Synchronizer.h"
#include "Qt6Speaker.h"
class Qt6DecoderAudio : public Qt6Decoder
{
    Q_OBJECT
public:
    explicit Qt6DecoderAudio(QObject *parent = nullptr);
    int start(Qt6Demuxer *demux,Qt6Synchronizer *sync, Qt6Packet *packet, Qt6Frame *frame);
private:
    Qt6Demuxer *demux;
    Qt6Synchronizer * sync;
    Qt6Packet *packet;
    Qt6Frame *frame;
    Qt6Resampler *resampler;
    Qt6Speaker *speaker;
    bool frameFinished= true;
    // Qt6Thread interface
signals:
    void signalSpeaker(const QByteArray &audio_bytes);
protected:
    virtual void loop() override;
};

#endif // QT6DECODERAUDIO_H
