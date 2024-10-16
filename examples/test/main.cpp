#include <QCoreApplication>
#include <QApplication>
#include <Qt6FFmpeg/QYUV420Player>
#include <Qt6FFmpeg/QRGBAPlayer>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Qt6FFmpeg::QYUV420Player player;
    Qt6FFmpeg::QRGBAPlayer player;
    //player.resize(800,600);
    player.resize(800,600);
    player.play("https://cesium.com/public/SandcastleSampleData/big-buck-bunny_trailer.mp4");
    a.connect(&player,&Qt6FFmpeg::QRGBAPlayer::reject,[&](int err){
        qDebug() << "error:" << err;
        player.stop();
        a.exit();
    });
    player.show();
    return a.exec();
}

