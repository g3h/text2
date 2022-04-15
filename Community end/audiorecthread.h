#ifndef AUDIORECTHREAD_H
#define AUDIORECTHREAD_H

#include <QThread>
#include <QDebug>
#include <QObject>
#include <QMutex>

#include <QAudio>//一下这五个是QT处理音频的库
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include "audiothread.h"



class AudioRecThread: public QThread
{
        Q_OBJECT
public:
    AudioRecThread(QObject *parent=0);

    void Resume();
    void Pause();

    QAudioOutput *output;

    QIODevice *outputDevice;

protected:
      void run();

private:
    QMutex m_lock;

private slots:



signals:


};
#endif // AUDIORECTHREAD_H
