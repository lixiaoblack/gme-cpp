/**
 * Copyright (c) 2025 Tencent. All rights reserved.
 * Module：   TMGPTT@GMESDK
 * Function： 腾讯云 GME 离线语音相关功能接口。
 */
#ifndef __GME_PTT_H__
#define __GME_PTT_H__

#include "av_type.h"

class ITMGPTT {
   public:
    virtual ~ITMGPTT() {
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                              离线语音相关
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 应用鉴权。
     *
     * 生成鉴权信息后，将鉴权赋值到 SDK 中。
     * @param userSig 鉴权信息。
     * @param userSigLen 鉴权信息长度。
     */
    virtual int ApplyPTTAuthbuffer(const char* userSig, int userSigLen) = 0;

    /**
     * 限制最大语音信息时长。
     *
     * 限制最大语音消息的长度，最大支持 180 秒。
     * @param msTime 语音时长，单位 ms，区间为 1000 < msTime < = 180000。
     */
    virtual int SetMaxMessageLength(int msTime) = 0;

    /**
     * 启动录音。
     *
     * 此接口用于启动录音。
     * @param filePath 存放的语音路径，文件后缀名必须为 .ogg。
     */
    virtual int StartRecording(const char* filePath) = 0;

    /**
     * 停止录音。
     *
     * 此接口用于停止录音，此接口为异步接口，停止录音后会有录音完成回调，成功之后录音文件才可用。
     */
    virtual int StopRecording() = 0;

    /**
     * 取消录音。
     *
     * 调用此接口取消录音，取消之后没有回调。
     */
    virtual int CancelRecording() = 0;

    /**
     * 上传语音文件。
     *
     * 此接口用于上传语音文件。
     * @param filePath 上传的语音路径，此路径为本地路径。
     */
    virtual int UploadRecordedFile(const char* filePath) = 0;

    /**
     * 下载语音文件。
     *
     * @param fileId 文件的 url 路径。
     * @param filePath 文件的本地保存路径，后缀名必须为 .ogg。
     */
    virtual int DownloadRecordedFile(const char* fileId, const char* filePath) = 0;

    /**
     * 播放语音。
     *
     * 此接口用于播放语音。
     * @param filePath 文件的本地保存路径。
     */
    virtual int PlayRecordedFile(const char* filePath) = 0;

    /**
     * 播放语音。
     *
     * 此接口用于播放语音。
     * @param filePath   文件的本地保存路径。
     * @param voiceType  变声音效类型。
     */
    virtual int PlayRecordedFile(const char* filePath, ITMG_VOICE_TYPE voiceType) = 0;

    /**
     * 停止播放语音。
     *
     * 此接口用于停止播放语音，停止播放语音也会有播放完成的回调。
     */
    virtual int StopPlayFile() = 0;

    /**
     * 将指定的语音文件识别成文字。
     *
     * 此接口用于将指定的语音文件识别成文字。
     * @param fileID 语音文件 url。
     */
    virtual int SpeechToText(const char* fileID) = 0;

    /**
     * 将指定的语音文件翻译成文字（指定语言）。
     *
     * 此接口可以指定语言进行识别，也可以将语音中识别到的信息翻译成指定的语言返回。
     * @param fileID 语音文件 语音文件 url，录音在服务器存放 90 天。
     * @param speechLanguage 识别出指定文字的语言参数。
     */
    virtual int SpeechToText(const char* fileID, const char* speechLanguage) = 0;

    /**
     * 将指定的语音文件翻译成文字（指定语言）。
     *
     * 此接口可以指定语言进行识别，也可以将语音中识别到的信息翻译成指定的语言返回。
     * @param fileID 语音文件 语音文件 url，录音在服务器存放90天。
     * @param speechLanguage 识别出指定文字的语言参数。
     * @param translateLanguage 翻译成指定文字的语言参数。
     */
    virtual int SpeechToText(const char* fileID, const char* speechLanguage, const char* translateLanguage) = 0;

    /**
     * 文本翻译接口。
     *
     * 调用此接口，可以将一段文本进行翻译，例如将一段中文翻译成英文。调用接口后通过回调返回翻译结果。
     * @param text 待翻译的文本，不可为空，最大长度 5000 字符。
     * @param sourceLanguage 指定待翻译文本的语言，可为空，后台自动检测语音。
     * @param translateLanguage 指定文本翻译后的语言，不可为空，以英文逗号作为间隔，如"cmn-Hans-CN,en-GB"、"cmn-Hans-CN"。
     */
    virtual int TranslateText(const char* text, const char* sourceLanguage, const char* translateLanguage) = 0;

    /**
     * 获取语音文件的大小。
     *
     * 通过此接口，获取语音文件的大小。
     * @param filePath 语音文件的路径，此路径为本地路径。
     */
    virtual int GetFileSize(const char* filePath) = 0;

    /**
     * 获取语音文件的时长。
     *
     * 此接口用于获取语音文件的时长，单位毫秒。
     * @param filePath 语音文件的路径，此路径为本地路径。
     */
    virtual int GetVoiceFileDuration(const char* filePath) = 0;

    /**
     * 启动流式语音识别。
     *
     * 此接口用于启动流式语音识别，同时在回调中会有实时的语音转文字返回，可以指定语言进行识别，也可以将语音中识别到的信息翻译成指定的语言返回。
     * @param filePath 存放的语音路径，文件后缀名必须为 .ogg。
     */
    virtual int StartRecordingWithStreamingRecognition(const char* filePath) = 0;

    /**
     * 启动流式语音识别。
     *
     * 此接口用于启动流式语音识别，同时在回调中会有实时的语音转文字返回，可以指定语言进行识别，也可以将语音中识别到的信息翻译成指定的语言返回。
     * @param filePath 存放的语音路径，文件后缀名必须为 .ogg。
     * @param speechLanguage 识别成指定文字的语言参数。
     */
    virtual int StartRecordingWithStreamingRecognition(const char* filePath, const char* speechLanguage) = 0;

    /**
     * 启动流式语音识别。
     *
     * 此接口用于启动流式语音识别，同时在回调中会有实时的语音转文字返回，可以指定语言进行识别，也可以将语音中识别到的信息翻译成指定的语言返回。
     * @param filePath 存放的语音路径，文件后缀名必须为 .ogg。
     * @param speechLanguage 识别成指定文字的语言参数。
     * @param translateLanguage 翻译成指定文字的语言参数。
     */
    virtual int StartRecordingWithStreamingRecognition(const char* filePath, const char* speechLanguage, const char* translateLanguage) = 0;

    /**
     * 设置 PTT 功能源语言。
     *
     * 通过此接口设置录制的 PTT 语音文件的语言，默认语音为中文。
     * @param sourceLanguage PTT 功能源语言。
     */
    virtual int SetPTTSourceLanguage(const char* sourceLanguage) = 0;

    /**
     * 文本转语音接口。
     *
     * 调用此接口，可以将一段文本转成对应的一段语音，调用接口后，通过回调返回语音 fileid，通过下载语音接口进行下载此语音。
     * @param serialNumber 序列号，接收到回调时返回该序列号。
     * @param text 原始文本，不可为空，最大长度 5000 字符。
     * @param voiceName 声音类型，提供英语及普通话的示例，如需其他语言请 提交工单 咨询。
     * @param languageCode 指定目标语言，不可为空。
     * @param speakingRate 音频语速，取值范围 [0.6-1.5]，1 代表正常速度。
     */
    virtual int TextToSpeech(int serialNumber, const char* text, const char* voiceName, const char* languageCode, float speakingRate) = 0;
};
#endif /* __GME_PTT_H__ */
