﻿/**
 * Copyright (c) 2025 Tencent. All rights reserved.
 * Module：   TMGAudioEffectCtrl@GMESDK
 * Function： 腾讯云 GME 音效相关功能接口。
 */
#ifndef __GME_AUDIO_EFFECT_CTRL_H__
#define __GME_AUDIO_EFFECT_CTRL_H__

#include "av_type.h"

/**
 * 录制的音频内容类型。
 *
 * 调用音频录制接口{@link StartRecord}时可以选择需要录制的音频内容。
 */
enum ITMG_AUDIO_RECORDING_CONTENT {

    /// 录制本地+远端音频。
    ITMG_AUDIO_RECORDING_CONTENT_ALL = 0,

    /// 只录制本地音频。
    ITMG_AUDIO_RECORDING_CONTENT_LOCAL = 1,

    /// 只录制远端音频。
    ITMG_AUDIO_RECORDING_CONTENT_REMOTE = 2,

};

/**
 * 混响特效。
 *
 * 混响特效可以作用于人声之上，通过声学算法对声音进行叠加处理，模拟出各种不同环境下的临场感受。
 */
enum ITMG_KARAOKE_TYPE {

    /// 关闭特效。
    ITMG_KARAOKE_TYPE_0 = 0,

    /// KTV
    ITMG_KARAOKE_TYPE_1 = 1,

    /// 小房间。
    ITMG_KARAOKE_TYPE_2 = 2,

    /// 大会堂。
    ITMG_KARAOKE_TYPE_3 = 3,

    /// 低沉。
    ITMG_KARAOKE_TYPE_4 = 4,

    /// 洪亮。
    ITMG_KARAOKE_TYPE_5 = 5,

    /// 金属声。
    ITMG_KARAOKE_TYPE_6 = 6,

    /// 磁性。
    ITMG_KARAOKE_TYPE_7 = 7,

    /// 空灵。
    ITMG_KARAOKE_TYPE_8 = 8,

    /// 录音棚。
    ITMG_KARAOKE_TYPE_9 = 9,

    /// 悠扬。
    ITMG_KARAOKE_TYPE_10 = 10,

    /// 录音棚2。
    ITMG_KARAOKE_TYPE_11 = 11

};

class ITMGAudioEffectCtrl {
   public:
    virtual ~ITMGAudioEffectCtrl() {
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                              音频特效和背景音乐
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 开始播放背景音乐。
     *
     * 每个音乐都需要指定具体的 ID，可以通过该 ID 开始、停止播放音乐以及设置音量等。
     * @param soundId     音乐 ID，大于 0 的整数。
     * @param filePath    音乐文件路径。
     * @param loopCount   播放次数，-1 表示循环播放， 1 - N 表示具体播放次数。
     * @note
     * 1. 如果要多次播放同一首背景音乐，请不要每次播放都分配一个新的 ID，我们推荐使用相同的 ID。
     * 2. 若您希望同时播放多首不同的音乐，请为不同的音乐分配不同的 ID 进行播放。
     * 3. 如果使用同一个 ID 播放不同音乐，SDK 会先停止播放旧的音乐，再播放新的音乐。
     */
    virtual int StartPlayMusic(long soundId, const char* filePath, int loopCount) = 0;

    /**
     * 停止播放背景音乐。
     *
     * @param soundId    音乐 ID。
     */
    virtual int StopPlayMusic(long soundId) = 0;

    /**
     * 暂停播放背景音乐。
     *
     * @param soundId    音乐 ID。
     */
    virtual int PausePlayMusic(long soundId) = 0;

    /**
     * 恢复播放背景音乐。
     *
     * @param soundId    音乐 ID。
     */
    virtual int ResumePlayMusic(long soundId) = 0;

    /**
     * 背景音乐是否播放结束。
     *
     * @param soundId    音乐 ID。
     */
    virtual bool IsMusicPlayEnd(long soundId) = 0;

    /**
     * 设置所有背景音乐的本地音量和远端音量的大小。
     *
     * 该接口可以设置所有背景音乐的本地音量和远端音量。
     * - 本地音量：即主播本地可以听到的背景音乐的音量大小。
     * - 远端音量：即观众端可以听到的背景音乐的音量大小。
     * @param volume 音量大小，取值范围为 [0，200]，默认值：100。
     * @note 如果将 volume 设置成 100 之后感觉音量还是太小，可以将 volume 最大设置成 200，但超过 100 的 volume 会有爆音的风险，请谨慎操作。
     */
    virtual int SetAllMusicVolume(int volume) = 0;

    /**
     * 设置某一首背景音乐的远端音量的大小。
     *
     * 该接口可以细粒度地控制每一首背景音乐的远端音量，也就是观众端可听到的背景音乐的音量大小。
     * @param soundId    音乐 ID。
     * @param volume     音量大小，取值范围为 [0，200]，默认值：100。
     * @note 如果将 volume 设置成 100 之后感觉音量还是太小，可以将 volume 最大设置成 200，但超过 100 的 volume 会有爆音的风险，请谨慎操作。
     */
    virtual int SetMusicPublishVolume(long soundId, int volume) = 0;

    /**
     * 获取某一首背景音乐的远端音量大小。
     *
     * 该接口可以细粒度地获取每一首背景音乐的远端音量，也就是观众端可听到的背景音乐的音量大小。
     * @param soundId    音乐 ID。
     */
    virtual int GetMusicPublishVolume(long soundId) = 0;

    /**
     * 设置某一首背景音乐的本地音量的大小。
     *
     * 该接口可以细粒度地控制每一首背景音乐的本地音量，也就是主播本地可以听到的背景音乐的音量大小。
     * @param soundId    音乐 ID。
     * @param volume     音量大小，取值范围为 [0，200]，默认值：100。
     * @note 如果将 volume 设置成 100 之后感觉音量还是太小，可以将 volume 最大设置成 200，但超过 100 的 volume 会有爆音的风险，请谨慎操作。
     */
    virtual int SetMusicPlayoutVolume(long soundId, int volume) = 0;

    /**
     * 获取某一首背景音乐的本地音量的大小。
     *
     * 该接口可以细粒度地获取每一首背景音乐的本地音量，也就是主播本地可以听到的背景音乐的音量大小。
     * @param soundId    音乐 ID。
     */
    virtual int GetMusicPlayoutVolume(long soundId) = 0;

    /**
     * 调整背景音乐的音调高低。
     *
     * @param soundId     音乐 ID。
     * @param pitch       音调，默认值是 0.0f，范围是：[-1 ~ 1] 之间的浮点数。
     */
    virtual int SetMusicPitch(long soundId, float pitch) = 0;

    /**
     * 获取背景音乐的总时长（单位：毫秒）。
     *
     * @param soundId    音乐 ID。
     */
    virtual int GetMusicDurationInMS(long soundId) = 0;

    /**
     * 获取背景音乐的播放进度（单位：毫秒）。
     *
     * @param soundId    音乐 ID。
     */
    virtual int GetMusicCurrentPosInMS(long soundId) = 0;

    /**
     * 设置背景音乐的播放进度（单位：毫秒）。
     *
     * @param soundId    音乐 ID。
     * @param time       播放进度，单位：毫秒。
     */
    virtual int SeekMusicToPosInTime(long soundId, int time) = 0;

    /**
     * 是否将 sound_id 对应的背景音乐发布到远端。
     *
     * @param soundId    音乐 ID。
     * @param enable     是否发布。
     */
    virtual int EnableMusicPublish(long soundId, bool enable) = 0;

    /**
     * 是否在本地播放 sound_id 对应的背景音乐。
     *
     * @param soundId    音乐 ID。
     * @param enable     是否播放。
     */
    virtual int EnableMusicPlayout(long soundId, bool enable) = 0;

    /**
     * 设置人声的变声特效。
     *
     * 通过该接口您可以设置人声的变声特效，具体特效请参见枚举定义 {@link ITMG_VOICE_TYPE}。
     * @param type    音乐 ID。
     * @note 设置的效果在退出房间后会自动失效，如果下次进房还需要对应特效，需要调用此接口再次进行设置。
     */
    virtual int SetVoiceType(ITMG_VOICE_TYPE type) = 0;

    /**
     * 设置人声的混响特效。
     *
     * 通过该接口您可以设置人声的混响特效，具体特效请参见枚举定义 {@link ITMG_KARAOKE_TYPE}。
     * @param type    音乐 ID。
     * @note 设置的效果在退出房间后会自动失效，如果下次进房还需要对应特效，需要调用此接口再次进行设置。
     */
    virtual int SetKaraokeType(ITMG_KARAOKE_TYPE type) = 0;

    /**
     * 开始录制音频。
     *
     * 通过该接口您可以将实时语音房间内的音频数据录制音频文件，可录制的音频数据类型请参见枚举定义 {@link ITMG_AUDIO_RECORDING_CONTENT}。
     * @param filePath    音频文件路径。
     * @param content     可以录制的音频数据类型。
     */
    virtual int StartRecord(const char* filePath, ITMG_AUDIO_RECORDING_CONTENT content) = 0;

    /**
     * 停止录制音频。
     */
    virtual int StopRecord() = 0;

    /**
     * 开启系统声音采集。
     *
     * @param playerPath  您可以指定该参数为空值（nullptr），代表让 SDK 采集整个系统的声音。
     * 在 Windows平台下，您也可以将参数 playerPath 设置为某个应用程序的可执行文件（如 QQMuisc.exe ）的绝对路径，
     * 此时 SDK 只会采集该应用程序的声音（仅支持 32 位版本的 SDK）。
     */
    virtual int StartSystemAudioLoopback(const char* playerPath) = 0;

    /**
     * 停止系统声音采集。
     */
    virtual int StopSystemAudioLoopback() = 0;

    /**
     * 设置系统声音的采集音量。
     *
     * @param volume  设置的音量大小，范围是：[0，200]，默认值为 100。
     */
    virtual int SetSystemAudioLoopbackVolume(int volume) = 0;
};
#endif /* __GME_AUDIO_EFFECT_CTRL_H__ */
