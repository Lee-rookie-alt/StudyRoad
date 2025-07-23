#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
class QPushButton;
class QSlider;
class QLabel;
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent =nullptr);
    ~Widget();
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
signals:
    void updateItembyhand();
private slots:
    void openFile();
    //更新进度条位置
    void updatePosition(qint64 position);
    //更新进度条范围
    void updateDuration(qint64 dutation);
    //设置进度条的位置
    void setPosition(int position);
    void updateVolumeSliderPosition();
    void onVideoItemClicked(QListWidgetItem *item);
private:
    Ui::Widget *ui;
    QMediaPlayer *m_player;//视频播放器
    QVideoWidget *m_videoWidget;//视频输出组件
    QPushButton  *m_openButton;//打开文件按钮
    QPushButton  *m_playButton;//播放按钮
    int playorder;             //正在播放的视频的顺序
    QLabel       *m_playLabel;
    QLabel       *m_next;
    QLabel       *m_last;
    int playstatus;
//    QPushButton  *m_stopButton;//停止按钮
    QLabel       *m_screensize;
    int screenstatus;
    int volumestatus;
    QLabel       *m_volume;
    QSlider      *m_positionSlider;//进度条
    QSlider      *m_volumeSlider;//声音条
    QLabel       *m_timeLabel;//进度时间标签
    QListWidget  *m_videoList;

    //存储视频信息的结构体和列表
    struct VideoInfo{
        QString name;
        QString path;
    };
    QList<VideoInfo> m_videoInfos;
    QRect normalGeometry;
    void addVideoToList(const QString &filePath);
    void updateCurrentPlayingItem();
};
#endif // WIDGET_H
