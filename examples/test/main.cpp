#include <QCoreApplication>
#include <QApplication>
#include <Qt6FFmpeg/YUV420PPlayer>
#include <Qt6FFmpeg/RGBAPlayer>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Qt6FFmpeg::YUV420PPlayer player;
    Qt6FFmpeg::RGBAPlayer player;
    //player.resize(800,600);
    player.resize(800,600);
    player.play("https://cesium.com/public/SandcastleSampleData/big-buck-bunny_trailer.mp4");
    a.connect(&player,&Qt6FFmpeg::RGBAPlayer::reject,[&](int err){
        qDebug() << "error:" << err;
        player.stop();
        a.exit();
    });
    player.show();
    return a.exec();
}

