#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QListWidget>
#include <QPoint>
#include <QAbstractItemView>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,playorder(0)
    ,playstatus(0)
    ,screenstatus(0)
    ,volumestatus(1)

{
    ui->setupUi(this);
    setWindowTitle("视频播放室");
    resize(800,800);
    //初始化播放器组件
    m_player=new QMediaPlayer(this);
    m_videoWidget=new QVideoWidget(this);

    //设置视频输出
    m_player->setVideoOutput(m_videoWidget);
    // 创建控制按钮
    m_openButton = new QPushButton("打开文件");
    m_playLabel =new QLabel;
    QIcon icon(":/new/prefix2/play.png");
    m_playLabel->setPixmap(icon.pixmap(64,64));
    m_playLabel->setCursor(Qt::PointingHandCursor);
    m_playButton = new QPushButton("播放");
    //进度条和音量控制
    m_volume=new QLabel;
    QIcon icon1(":/new/prefix2/volume.png");
    m_volume->setPixmap(icon1.pixmap(32,32));//音量图标
    m_volume->installEventFilter(this);//音量图标安装事件过滤器
    m_next =new QLabel;//下一个图标
    m_next->installEventFilter(this);
    QIcon icon_next(":/new/prefix2/next.png");
    m_next->setPixmap(icon_next.pixmap(32,32));
    m_last =new QLabel;//上一个图标
    m_last->installEventFilter(this);
    QIcon icon_last(":/new/prefix2/last.png");
    m_last->setPixmap(icon_last.pixmap(32,32));
    m_positionSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider = new QSlider(Qt::Vertical);
    m_volumeSlider->installEventFilter(this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->resize(10,80);
    m_volumeSlider->setValue(50);
   // m_volumeSlider->setFixedHeight(80);
    m_volumeSlider->hide();
    m_volumeSlider->setParent(this);
    m_timeLabel = new QLabel("00:00/00:00");

    //全屏按钮控制
    m_screensize =new QLabel;
    QIcon icon_screen(":/new/prefix2/fullScreen.png");
    m_screensize->setPixmap(icon_screen.pixmap(32,32));
    m_screensize->installEventFilter(this);
    //主布局
    QHBoxLayout *mmain=new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_videoWidget,8);
    //控制按钮布局



    //视频列表初始化
    m_videoList = new QListWidget();
    m_videoList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_videoList->setAlternatingRowColors(true);//开启交替行颜色
    connect(m_videoList,&QListWidget::itemClicked,this,&Widget::onVideoItemClicked);
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel("进度:"), 0, 0);
    gridLayout->addWidget(m_positionSlider, 0, 1, 1, 16);
    gridLayout->addWidget(m_timeLabel, 0, 17, 1, 2);
    gridLayout->addWidget(m_openButton, 2, 0, 1, 3);
    gridLayout->addWidget(m_last,2,8,1,1);
    gridLayout->addWidget(m_playLabel, 2, 9, 1, 1);/**/
    gridLayout->addWidget(m_next,2,10,1,1);
    gridLayout->addWidget(m_volume, 2, 17);
    gridLayout->addWidget(m_screensize,2,18);

    mainLayout->addLayout(gridLayout,2);

    m_timeLabel->setFixedHeight(50);//设置固定高度
    m_timeLabel->setAlignment(Qt::AlignCenter); //保持文字居中

    mmain->addLayout(mainLayout,8);
    mmain->addWidget(m_videoList,2);
    setLayout(mmain);

    //信号槽连接
    connect(m_openButton, &QPushButton::clicked, this, &Widget::openFile);
    connect(m_playButton, &QPushButton::clicked, m_player, &QMediaPlayer::play);
    connect(m_volumeSlider, &QSlider::valueChanged, m_player, &QMediaPlayer::setVolume);
    connect(m_positionSlider, &QSlider::sliderMoved, this, &Widget::setPosition);
    connect(m_player, &QMediaPlayer::positionChanged, this, &Widget::updatePosition);
    connect(m_player, &QMediaPlayer::durationChanged, this, &Widget::updateDuration);
    connect(m_player,&QMediaPlayer::mediaStatusChanged,this,&Widget::updateCurrentPlayingItem);
}
void Widget::openFile(){//打开视频文件
        QStringList filenames=QFileDialog::getOpenFileNames(
                    this,
                    "选择视频文件",
                    QDir::homePath(),
                    "视频文件(*.mp4 *.avi *.mkv *,mov)"
                    );
        if(!filenames.isEmpty()){
            foreach(QString filename,filenames){
                addVideoToList(filename);
            }
            if(m_player->mediaStatus()== QMediaPlayer::NoMedia && !m_videoInfos.isEmpty()){
                m_player->setMedia(QUrl::fromLocalFile(m_videoInfos.first().path));
                playorder=0;
                playstatus=1;
                QIcon icon(":/new/prefix2/ing.png");
                m_playLabel->setPixmap(icon.pixmap(64,64));
                m_player->play();
            }
    }
}
void Widget::addVideoToList(const QString &filePath){
    QFileInfo fileinfo(filePath);
    QString fileName=fileinfo.fileName();
    //检查是否在列表当中
    foreach(const VideoInfo &info,m_videoInfos){
        if(info.path == filePath){
            return; //已存在则不添加
        }
    }
    //添加到数据列表
    qDebug()<<"开始添加到数据列表";
    VideoInfo info;
    info.name =fileName;
    info.path=filePath;
    m_videoInfos.append(info);
    //添加到界面列表
    QListWidgetItem *item = new QListWidgetItem(fileName);
    m_videoList->addItem(item);
}
void Widget::onVideoItemClicked(QListWidgetItem *item){//单击视频列表中的视频Item即可播放
    int index=m_videoList->row(item);
    if(index>=0 && index<m_videoInfos.size()){
        QString videoPath = m_videoInfos[index].path;
        m_player->setMedia(QUrl::fromLocalFile(videoPath));
        playorder=index;
        playstatus=1;
        QIcon Icon(":/new/prefix2/ing.png");
        m_playLabel->setPixmap(Icon.pixmap(64,64));
        m_player->play();
    }
}
void Widget::updateCurrentPlayingItem(){
    QUrl mediaurl=m_player->media().request().url();
    QString curpath=mediaurl.toLocalFile();
    //清除之前选中的状态
    qDebug()<<"开始更新当前播放的项目";
    for(int i=0;i<m_videoList->count();i++){
        QListWidgetItem *item=m_videoList->item(i);
        if(item){
            item->setBackground(QBrush());
            item->setForeground(QBrush(Qt::black));
        }
    }
    //高亮当前播放项
    for(int i=0;i<m_videoInfos.size();i++){
        if(m_videoInfos[i].path==curpath){
            QListWidgetItem *item=m_videoList->item(i);
            if(item){
                item->setBackground(QBrush(QColor(0,120,215)));
                item->setForeground(QBrush(Qt::white));
                m_videoList->setCurrentItem(item);
                m_videoList->scrollToItem(item);
            }
            break;
        }
    }
}
void Widget::updatePosition(qint64 position){
    m_positionSlider->setValue(position);
    //更新时间标签
    QTime currentTime(0,0,0);
    currentTime =currentTime.addMSecs(position);
    QTime totalTime(0,0,0);
    totalTime = totalTime.addMSecs(m_player->duration());
    m_timeLabel->setText(currentTime.toString("hh:mm:ss") + "/" +totalTime.toString("hh:mm:ss"));
}
void Widget::updateDuration(qint64 duration){
    m_positionSlider->setRange(0,duration);
}
void Widget::setPosition(int position){
    m_player->setPosition(position);
}
void Widget::mousePressEvent(QMouseEvent *event){
    if(m_playLabel->geometry().contains(event->pos())){
        if(playstatus==0)
       {
            QIcon icon(":/new/prefix2/ing.png");
            m_player->play();
            m_playLabel->setPixmap(icon.pixmap(64,64));
            playstatus=1;
       }else{
            QIcon icon(":/new/prefix2/play.png");
            m_playLabel->setPixmap(icon.pixmap(64,64));
            m_player->pause();
            playstatus=0;
        }
    }
    else if(m_volume->geometry().contains(event->pos())){
        if(volumestatus==0){
            QIcon icon(":/new/prefix2/highvolume.png");
            m_volume->setPixmap(icon.pixmap(32,32));
            m_player->setVolume(50);
            m_volumeSlider->setValue(50);
            volumestatus=1;
        }else{
            QIcon icon(":/new/prefix1/silentvolume.png");
            m_volume->setPixmap(icon.pixmap(32,32));
            volumestatus=0;
            m_player->setVolume(0);
            m_volumeSlider->setValue(0);
        }
    }else if(m_next->geometry().contains(event->pos())){
        if(playorder!=m_videoInfos.size()-1){
            playorder+=1;
            QString videoPath = m_videoInfos[playorder].path;
            m_player->setMedia(QUrl::fromLocalFile(videoPath));
            playstatus=1;
            QIcon Icon(":/new/prefix2/ing.png");
            m_playLabel->setPixmap(Icon.pixmap(64,64));
            m_player->play();
            updateCurrentPlayingItem();
        }else{
            playorder=0;
            QString videoPath = m_videoInfos[playorder].path;
            m_player->setMedia(QUrl::fromLocalFile(videoPath));
            playstatus=1;
            QIcon Icon(":/new/prefix2/ing.png");
            m_playLabel->setPixmap(Icon.pixmap(64,64));
            m_player->play();
            updateCurrentPlayingItem();
        }
    }else if(m_last->geometry().contains(event->pos())){
        if(playorder!=0){
            playorder-=1;
            QString videoPath = m_videoInfos[playorder].path;
            m_player->setMedia(QUrl::fromLocalFile(videoPath));
            playstatus=1;
            QIcon Icon(":/new/prefix2/ing.png");
            m_playLabel->setPixmap(Icon.pixmap(64,64));
            m_player->play();
            updateCurrentPlayingItem();
        }else{
            playorder=m_videoInfos.size()-1;
            QString videoPath = m_videoInfos[playorder].path;
            m_player->setMedia(QUrl::fromLocalFile(videoPath));
            playstatus=1;
            QIcon Icon(":/new/prefix2/ing.png");
            m_playLabel->setPixmap(Icon.pixmap(64,64));
            m_player->play();
            updateCurrentPlayingItem();
        }
    }
    QWidget::mousePressEvent(event);
}
bool Widget::eventFilter(QObject *watched, QEvent *event){
    if (watched == m_volume) {
            // 鼠标进入事件
            if (event->type() == QEvent::Enter) {
                updateVolumeSliderPosition();
                m_volumeSlider->show();
                return true; // 已处理事件
            }else if(event->type() == QEvent::Leave){//鼠标离开音量图标：判断是否移到
                QTimer::singleShot(100,this,[=](){
                    bool isOnSlider=m_volumeSlider->geometry().contains(mapFromGlobal(QCursor::pos()));
                    if(!isOnSlider){
                        m_volumeSlider->hide();
                    }
                });
                return true;
            }
    }
    else if(watched == m_volumeSlider){
            // 鼠标离开事件
        if(event->type()== QEvent::Leave){
            //检查鼠标是否在音量图标上
            bool isOnVolumeIcon = m_volume->geometry().contains(mapFromGlobal(QCursor::pos()));
            if(!isOnVolumeIcon){
                m_volumeSlider->hide();
            }
            return true;
        }
    }
    else if(watched == m_screensize){
        if(event->type()==QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent=static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() ==Qt::LeftButton && screenstatus==0){
                QIcon icon(":/new/prefix2/nofullScreen.png");
                m_screensize->setPixmap(icon.pixmap(32,32));
                screenstatus=1;
                showMaximized();
            }else if(mouseEvent->button() ==Qt::LeftButton && screenstatus==1){
                screenstatus=0;
                QIcon icon(":/new/prefix2/fullScreen.png");
                m_screensize->setPixmap(icon.pixmap(32,32));
                showNormal();
            }
        }
    }
    return QWidget::eventFilter(watched,event);
}
void Widget::updateVolumeSliderPosition(){
    QPoint volumePos=m_volume->mapTo(this,QPoint(0,0));
    // volumePos是m_volume在Widget中的左上角坐标
    // 2. 获取进度条的位置（用于计算上方偏移）
    QPoint progressBarPos = m_positionSlider->mapTo(this, QPoint(0, 0));
    int progressBarBottom = progressBarPos.y() + m_positionSlider->height(); // 进度条底部y坐标
    // 3. 计算音量条位置：
    // - 水平方向：与音量图标左对齐（x坐标相同）
    // - 垂直方向：位于进度条底部上方10像素（可调整间距）
    int sliderX = volumePos.x();
    int sliderY = progressBarBottom - m_volumeSlider->height() - 10; // 进度条上方10px
    // 设置音量条位置和大小
    m_volumeSlider->setGeometry(sliderX, sliderY, m_volumeSlider->width(), m_volumeSlider->height());
}
void Widget::changeEvent(QEvent *event){
    QWidget::changeEvent(event);
    if(event->type()==QEvent::WindowStateChange)
    {
        Qt::WindowStates currentState=windowState();
        if(currentState &Qt::WindowMaximized){
            screenstatus=1;
            QIcon icon(":/new/prefix2/nofullScreen.png");
            m_screensize->setPixmap(icon.pixmap(32,32));
        }else{
            screenstatus=0;
            QIcon icon(":/new/prefix2/fullScreen.png");
            m_screensize->setPixmap(icon.pixmap(32,32));
        }
    }
}
void Widget::resizeEvent(QResizeEvent *event){
    updateVolumeSliderPosition();
    QWidget::resizeEvent(event);
}
Widget::~Widget()
{
    delete ui;
}

