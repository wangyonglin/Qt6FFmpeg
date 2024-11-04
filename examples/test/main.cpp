#include <QCoreApplication>
#include <QApplication>
#include <QtRockchip/Qt6FFmpeg/YUV420PPlayer>
#include <QtRockchip/Qt6FFmpeg/RGBAPlayer>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtRockchip::Qt6FFmpeg::YUV420PPlayer player;
   // Qt6FFmpeg::RGBAPlayer player;
    //player.resize(800,600);
    player.resize(800,600);
    player.play("https://cesium.com/public/SandcastleSampleData/big-buck-bunny_trailer.mp4");
    a.connect(&player,&QtRockchip::Qt6FFmpeg::YUV420PPlayer::reject,[&](int err){
        qDebug() << "error:" << err;
        player.stop();
        a.exit();
    });
    player.show();
    return a.exec();
}

