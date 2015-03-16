#include "ui_videoplayer.h"

VideoPlayer::VideoPlayer(QString title, int source) {

    this->source = source;

    group = new QGroupBox(title);
    layout = new QGridLayout();

    vlcArgs = VlcCommon::args();
    vlcArgs.append("--ffmpeg-hw");
    vlcArgs.append("--network-caching=0");
    //vlcArgs.append("--rtsp-frame-buffer-size=100");

    vlcInstance = new VlcInstance(vlcArgs);

    videoWidget = new VlcWidgetVideo();
    videoWidget->setMaximumSize(640, 480);
    videoWidget->setMinimumSize(320, 240);
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mediaPlayer = new VlcMediaPlayer(vlcInstance);
    mediaPlayer->setVideoWidget(videoWidget);

    stateLabel = new QLabel("");
    QFont pt7;
    pt7.setPointSize(7);
    stateLabel->setFont(pt7);

    layout->addWidget(videoWidget, 0, 0);
    layout->addWidget(stateLabel, 1, 0, Qt::AlignRight|Qt::AlignTop);

    group->setLayout(layout);

    refreshTimer.setInterval(RTP_REFRESH_INTERVAL*1000);
    refreshTimer.start();
    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));

    connect(mediaPlayer, SIGNAL(stateChanged()), this, SLOT(stateChanged()));
}
VideoPlayer::~VideoPlayer() {}

void VideoPlayer::update() {

    mediaPlayer->stop();
    media = new VlcMedia(QString("stream%1.sdp").arg(source), true, vlcInstance);
    mediaPlayer->open(media);
}

void VideoPlayer::refresh() {

    mediaPlayer->open(media);
}

void VideoPlayer::stop() {

    mediaPlayer->stop();
}

void VideoPlayer::stateChanged() {

    switch (mediaPlayer->state()) {
    case Vlc::Idle:
        stateLabel->setText("Idle"); break;
    case Vlc::Opening:
        stateLabel->setText("Opening"); break;
    case Vlc::Buffering:
        stateLabel->setText("Buffering..."); break;
    case Vlc::Playing:
        stateLabel->setText("Streaming..."); break;
    case Vlc::Paused:
        stateLabel->setText("Paused"); break;
    case Vlc::Stopped:
        stateLabel->setText("Stopped"); break;
    case Vlc::Ended:
        stateLabel->setText("Stream unavailable"); break;
    case Vlc::Error:
        stateLabel->setText("Error"); break;
    }
}
