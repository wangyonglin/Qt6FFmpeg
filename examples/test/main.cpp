#include <QCoreApplication>
#include <QApplication>
#include <Qt6FFmpeg/QFFmpegPlayer>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt6FFmpeg::QFFmpegPlayer player;
    player.resize(1080,1920);
    player.play("https://cesium.com/public/SandcastleSampleData/big-buck-bunny_trailer.mp4");
    a.connect(&player,&Qt6FFmpeg::QFFmpegPlayer::reject,[&](int err){
        qDebug() << "error:" << err;
        player.stop();
        a.exit();
    });
    player.show();
    return a.exec();
}

