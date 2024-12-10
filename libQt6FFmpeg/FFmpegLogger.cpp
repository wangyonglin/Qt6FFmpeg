#include "FFmpegLogger.h"

FFmpegLogger::FFmpegLogger(QObject *parent)
    : QObject{parent}
{}

int FFmpegLogger::logError(const QString &msg, int ret)
{
    if(ret <0){
        char errmsg[AV_ERROR_MAX_STRING_SIZE]={0x00};
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        av_log(NULL, AV_LOG_FATAL, "Input not open failed: %s\n", errmsg);
        qDebug() << tr("%0 Error: %1").arg(msg).arg(errmsg);
    }

    return ret;
}
