#include "audiorecthread.h"

AudioRecThread::AudioRecThread(QObject *parent)
    :QThread(parent)
{
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    output = new QAudioOutput(format,this);

    outputDevice =output->start();//开始播放

}

void AudioRecThread::run()
{
    while(1)
        {
          m_lock.lock();
          m_lock.unlock();
         }
}

void AudioRecThread::Pause()
{
    m_lock.lock();
}

void AudioRecThread::Resume()
{
    m_lock.unlock();
}
