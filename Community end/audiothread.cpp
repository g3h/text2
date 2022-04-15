#include "audiothread.h"

AudioThread::AudioThread(QObject *parent)
    :QThread(parent)
{
    QAudioFormat format;//定义音频处理的类型
    format.setSampleRate(8000);//采集频率为1s 8000次
    format.setChannelCount(1);//设定为1个省道
    format.setSampleSize(16);//设定采样大小，8也行，但是要发送端和接收端对应
    format.setCodec("audio/pcm");//设定为PCM编码
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);//设定小尾的数据类型

    input = new QAudioInput(format,this);
    inputDevice = input->start();
    //input开始读入输入的音频信号，写入QIODevice，这里是inputDevice

}

void AudioThread::run()
{
    while(1)
        {
          m_lock.lock();
           m_lock.unlock();
         }
}

void AudioThread::Pause()
{
    //m_lock.lock();
     input->stop();inputDevice->reset();
      qDebug()<<"this is a Pause ////udpTest"<<endl;
       qDebug()<<"start state="<<input->state();

}

void AudioThread::Resume()
{
    //m_lock.unlock();
    input->start();

}
