#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include <QDebug>
#include <QObject>
#include <QMutex>

#include <QAudio>//一下这五个是QT处理音频的库
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>

struct video
{
    char audiodata[1024];
    int lens;
};


class AudioThread: public QThread
{
        Q_OBJECT
public:
    AudioThread(QObject *parent=0);

    void Resume();
    void Pause();


    QAudioInput *input;

    QIODevice *inputDevice;

protected:
      void run();

private:
    QMutex m_lock;//线程顺序执行锁

private slots:



signals:


};

#endif // AUDIOTHREAD_H
