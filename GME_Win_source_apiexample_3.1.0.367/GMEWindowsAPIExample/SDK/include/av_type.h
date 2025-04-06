/**
 * Copyright (c) 2025 Tencent. All rights reserved.
 * Module：   GME 关键类型定义
 * Function： 常用的结构体、枚举值和常量值的定义。
 */
#ifndef __GME_TYPE_H__
#define __GME_TYPE_H__

#include <cstdint>

#if defined(_WIN32) || defined(__ORBIS__)
#ifdef _GMESDK_IMPLEMENT_
#define GMESDK_EXPORT __declspec(dllexport)
#else
#define GMESDK_EXPORT __declspec(dllimport)
#endif
#define GME_API extern "C" GMESDK_EXPORT
#define GMESDK_CALL __cdecl
#else
#define GME_API extern "C" __attribute__((visibility("default")))
#define GMESDK_CALL
#endif

#define DEVICEID_DEFAULT "0"

/////////////////////////////////////////////////////////////////////////////////
//
//                              日志级别
//
/////////////////////////////////////////////////////////////////////////////////

/**
 * 日志级别。
 */
enum ITMG_LOG_LEVEL {

    /// 关闭日志。
    ITMG_LOG_LEVEL_NONE = -1,

    /// 只输出 Error 级别日志。
    ITMG_LOG_LEVEL_ERROR = 1,

    /// 输出 Info、Error 级别日志。
    ITMG_LOG_LEVEL_INFO = 2,

    /// 输出 Debug、Info、Error 级别日志。
    ITMG_LOG_LEVEL_DEBUG = 3,

    /// 输出 Verbose、Debug、Info、Error 级别日志。
    ITMG_LOG_LEVEL_VERBOSE = 4,

};

/**
 * 应用场景。
 */
enum ITMG_APP_SCENE {

    /// 实时语聊场景，该场景下单个房间最多支持 300 人，最高支持 50 人同时发言，房间内用户默认为主播，均可自由发言。
    ITMG_APP_SCENE_RTC = 2,

    /// 音频直播、游戏国战语音场景，同一个房间中最多能容纳 10 万观众；房间内用户默认为观众，不能直接发言，需要在进房前/后调用 {@link SetAudioRole} 切换成主播。
    ITMG_APP_SCENE_LIVE = 3,

};

/**
 * 角色类型。
 */
enum ITMG_AUDIO_MEMBER_ROLE {

    /// 主播角色，可随时发布自己的音频流。
    ITMG_AUDIO_MEMBER_ROLE_ANCHOR = 0,

    /// 观众角色，只能收听主播的音频流，不能发布自己的音频流，如需发布，在进房前/后调用 {@link SetAudioRole} 切换成主播。
    ITMG_AUDIO_MEMBER_ROLE_AUDIENCE = 1,

};

/**
 * 房间类型。
 */
enum ITMG_ROOM_TYPE {

    /// 流畅音质，流畅优先、超低延迟实时语音，应用在游戏内开黑场景，适用于 FPS、MOBA 等类型的游戏。
    ITMG_ROOM_TYPE_FLUENCY = 1,

    /// 标准音质，音质较好，延时适中，适用于狼人杀、棋牌等休闲游戏的实时通话场景。
    ITMG_ROOM_TYPE_STANDARD = 2,

    /// 高清音质，超高音质，延时相对大一些，适用于音乐舞蹈类游戏以及语音互动类 App；适用于播放音乐、线上 K 歌等有高音质要求的场景。
    ITMG_ROOM_TYPE_HIGHQUALITY = 3,

};

/**
 * GME SDK 事件类型。
 */
enum ITMG_MAIN_EVENT_TYPE {

    /// 进入实时语音房间事件。
    ITMG_MAIN_EVENT_TYPE_ENTER_ROOM = 1,

    /// 退出实时语音房间事件 。
    ITMG_MAIN_EVENT_TYPE_EXIT_ROOM = 2,

    /// 房间因为网络等原因断开连接事件。
    ITMG_MAIN_EVENT_TYPE_ROOM_DISCONNECT = 3,

    /// 成员状态变化事件。
    ITMG_MAIN_EVENT_TYPE_USER_UPDATE = 4,

    /// 已废弃。
    ITMG_MAIN_EVENT_TYPE_NUMBER_OF_USERS_UPDATE = 7,

    /// 已废弃。
    ITMG_MAIN_EVENT_TYPE_NUMBER_OF_AUDIOSTREAMS_UPDATE = 8,

    /// 房间重连开始事件。
    ITMG_MAIN_EVENT_TYPE_RECONNECT_START = 11,

    /// 房间重连成功事件。
    ITMG_MAIN_EVENT_TYPE_RECONNECT_SUCCESS = 12,

    /// 快速切换房间结果事件。
    ITMG_MAIN_EVENT_TYPE_SWITCH_ROOM = 13,

    /// 角色设置成功事件。
    ITMG_MAIN_EVENT_TYPE_SET_AUDIO_ROLE = 14,

    /// 已废弃。
    ITMG_MAIN_EVENT_TYPE_CHANGE_ROOM_TYPE = 21,

    /// 已废弃。
    ITMG_MAIN_EVENT_TYPE_AUDIO_DATA_EMPTY = 22,

    /// 请求跨房通话的结果事件。
    ITMG_MAIN_EVENT_TYPE_ROOM_SHARING_START = 23,

    /// 结束跨房通话的结果回调。
    ITMG_MAIN_EVENT_TYPE_ROOM_SHARING_STOP = 24,

    /// 音频录制结束事件。
    ITMG_MAIN_EVENT_TYPE_RECORD_COMPLETED = 30,

    /// iOS 静音键检查结果，[仅 iOS 平台有效]。
    ITMG_MAIN_EVENT_TYPE_IOS_MUTE_SWITCH_RESULT = 34,

    /// 默认扬声器发生改变事件，[仅 Windows 平台有效]。
    ITMG_MAIN_EVENT_TYPE_SPEAKER_DEFAULT_DEVICE_CHANGED = 1008,

    /// 新增扬声器设备事件，[仅 Windows 平台有效]。
    ITMG_MAIN_EVENT_TYPE_SPEAKER_NEW_DEVICE = 1009,

    /// 移除扬声器设备事件，[仅 Windows 平台有效]。
    ITMG_MAIN_EVENT_TYPE_SPEAKER_LOST_DEVICE = 1010,

    /// 新增麦克风设备事件，[仅 Windows 平台有效]。
    ITMG_MAIN_EVENT_TYPE_MIC_NEW_DEVICE = 1011,

    /// 移除麦克风设备事件，[仅 Windows 平台有效]。
    ITMG_MAIN_EVENT_TYPE_MIC_LOST_DEVICE = 1012,

    /// 默认麦克风设备发生改变事件，[仅 Windows 平台有效]。
    ITMG_MAIN_EVENT_TYPE_MIC_DEFAULT_DEVICE_CHANGED = 1013,

    /// 房间内用户音量发生变化事件。
    ITMG_MAIN_EVENT_TYPE_USER_VOLUMES = 1020,

    /// 网络通话质量事件。
    ITMG_MAIN_EVENT_TYPE_NETWORK_QUALITY = 1021,

    /// 房间通话质量监控事件（已废弃），请使用 ITMG_MAIN_EVENT_TYPE_NETWORK_QUALITY。
    ITMG_MAIN_EVENT_TYPE_CHANGE_ROOM_QUALITY = 1022,

    /// 房间内用户频谱数据。
    ITMG_MAIN_EVENT_TYPE_USER_SPECTRUMS = 1023,

    /// 房间内用户 VAD 状态回调事件。
    ITMG_MAIN_EVENT_TYPE_USER_VAD = 1024,

    /// 麦克风测试回调事件 [仅桌面端有效]。
    ITMG_MAIN_EVENT_TYPE_TEST_MIC_VOLUME = 1030,

    /// 扬声器测试回调事件 [仅桌面端有效]。
    ITMG_MAIN_EVENT_TYPE_TEST_SPEAKER_VOLUME = 1031,

    /// BGM 开始播放事件。
    ITMG_MAIN_EVENT_TYPE_MUSIC_PLAY_START = 1087,

    /// BGM 播放进度事件。
    ITMG_MAIN_EVENT_TYPE_MUSIC_PLAY_PROGRESS = 1088,

    /// BGM 暂停播放事件。
    ITMG_MAIN_EVENT_TYPE_MUSIC_PLAY_PAUSE = 1089,

    /// BGM 恢复播放事件。
    ITMG_MAIN_EVENT_TYPE_MUSIC_PLAY_RESUME = 1090,

    /// BGM 播放完成事件。
    ITMG_MAIN_EVENT_TYPE_MUSIC_PLAY_FINISH = 1091,

    /// 自定义数据事件。
    ITMG_MAIN_EVENT_TYPE_CUSTOMDATA_UPDATE = 1092,

    /// SEI 自定义数据事件。
    ITMG_MAIN_EVENT_TYPE_RECV_SEI_MSG = 1093,

    /// 语音消息录制完成事件。
    ITMG_MAIN_EVENT_TYPE_PTT_RECORD_COMPLETE = 5001,

    /// 语音消息文件上传完成事件。
    ITMG_MAIN_EVENT_TYPE_PTT_UPLOAD_COMPLETE = 5002,

    /// 语音消息文件下载完成事件。
    ITMG_MAIN_EVENT_TYPE_PTT_DOWNLOAD_COMPLETE = 5003,

    /// 语音消息播放完成事件。
    ITMG_MAIN_EVENT_TYPE_PTT_PLAY_COMPLETE = 5004,

    /// 语音消息转文本完成事件。
    ITMG_MAIN_EVENT_TYPE_PTT_SPEECH2TEXT_COMPLETE = 5005,

    /// 流式语音转文本完成事件。
    ITMG_MAIN_EVENT_TYPE_PTT_STREAMINGRECOGNITION_COMPLETE = 5006,

    /// 流式语音转文本进行中。
    ITMG_MAIN_EVENT_TYPE_PTT_STREAMINGRECOGNITION_IS_RUNNING = 5007,

    /// 文本转语音完成事件。
    ITMG_MAIN_EVENT_TYPE_PTT_TEXT2SPEECH_COMPLETE = 5008,

    /// 文本翻译完成事件。
    ITMG_MAIN_EVENT_TYPE_PTT_TRANSLATE_TEXT_COMPLETE = 5009,

    /// 控制远端用户音频设备事件。
    ITMG_MAIN_EVENT_TYPE_ROOM_MANAGEMENT_OPERATOR = 6000,

};

/**
 * 自定义数据相关子事件。
 */
enum ITMG_CUSTOMDATA_SUB_EVENT {

    /// 自定义数据事件。
    ITMG_CUSTOMDATA_AV_SUB_EVENT_UPDATE = 0,

};

/**
 * 范围语音相关状态。
 */
enum ITMG_RANGE_AUDIO_MODE {

    /// 默认模式：音频发送给房间内所有玩家，接收房间类所有玩家的音频。
    ITMG_RANGE_AUDIO_MODE_SND_ALL_REC_ALL = 1,

    /// 主持人模式：音频发送给房间内所有玩家，接收附近范围内和同队玩家的音频。
    ITMG_RANGE_AUDIO_MODE_SND_ALL_REC_BOTH = 2,

    /// 小队模式：音频只发送给同队玩家，只接收同队玩家的音频。
    ITMG_RANGE_AUDIO_MODE_SND_TEAM_REC_TEAM = 3,

    /// 音频只发送给同队玩家，只接收附近范围内的音频。
    ITMG_RANGE_AUDIO_MODE_SND_TEAM_REC_PROX = 4,

    /// 音频只发送给同队玩家，接收附近范围内和同队玩家的音频。
    ITMG_RANGE_AUDIO_MODE_SND_TEAM_REC_BOTH = 5,

    /// 音频只发送给附近范围内的玩家，只接收同队玩家的音频。
    ITMG_RANGE_AUDIO_MODE_SND_PROX_REC_TEAM = 6,

    /// 范围模式：音频只发送给附近范围内的玩家，只接收附近范围内的音频。
    ITMG_RANGE_AUDIO_MODE_SND_PROX_REC_PROX = 7,

    /// 音频只发送给附近范围内的玩家，接收附近范围内和同队玩家的音频。
    ITMG_RANGE_AUDIO_MODE_SND_PROX_REC_BOTH = 8,

    /// 音频发送给附近范围内和同队玩家的音频，只接收同队玩家的音频。
    ITMG_RANGE_AUDIO_MODE_SND_BOTH_REC_TEAM = 9,

    /// 音频发送给附近范围内和同队玩家的音频，只接收附近范围内的音频。
    ITMG_RANGE_AUDIO_MODE_SND_BOTH_REC_PROX = 10,

    /// 范围+小队模式：音频发送给附近范围内和同队玩家的音频，接收附近范围内和同队玩家的音频。
    ITMG_RANGE_AUDIO_MODE_SND_BOTH_REC_BOTH = 11,

};

/**
 * 成员状态变化子事件。
 */
enum ITMG_EVENT_ID_USER_UPDATE {

    /// 有用户进入语音房间事件。
    ITMG_EVENT_ID_USER_ENTER = 1,

    /// 有用户退出语音房间事件。
    ITMG_EVENT_ID_USER_EXIT = 2,

    /// 有用户开启麦克风事件。
    ITMG_EVENT_ID_USER_MIC_OPENED = 3,

    /// 有用户关闭麦克风事件。
    ITMG_EVENT_ID_USER_MIC_CLOSED = 4,

    /// 有用户发送音频数据事件，返回此时房间内说话的 UserID，通过此事件可以判断用户是否说话，并展示声纹效果。
    ITMG_EVENT_ID_USER_HAS_AUDIO = 5,

    /// 有用户停止发送音频数据事件，返回此时房间内停止说话的 UserID。
    ITMG_EVENT_ID_USER_NO_AUDIO = 6,

};

/**
 * PCM - 位深度。
 */
enum ITMG_PCM_BITS_TYPE {

    /// 16bit
    ITMG_PCM_BITS_TYPE_FIXED_16 = 0,

    /// 32bit
    ITMG_PCM_BITS_TYPE_FLOAT_32 = 1

};

/**
 * 网络质量, 表征网络质量的好坏，您可以通过该数值在用户界面上展示每个用户的网络质量。
 */
enum ITMG_NETWORK_QUALITY {

    /// 未定义。
    ITMG_NETWORK_QUALITY_UNKNOWN = 0,

    /// 当前网络非常好。
    ITMG_NETWORK_QUALITY_EXCELLENT = 1,

    /// 当前网络比较好。
    ITMG_NETWORK_QUALITY_GOOD = 2,

    /// 当前网络一般。
    ITMG_NETWORK_QUALITY_POOR = 3,

    /// 当前网络较差。
    ITMG_NETWORK_QUALITY_BAD = 4,

    /// 当前网络很差。
    ITMG_NETWORK_QUALITY_VBAD = 5,

    /// 当前网络不满足 GME SDK 的最低要求。
    ITMG_NETWORK_QUALITY_DOWN = 6

};

/**
 * 音频帧数据。
 */
struct TMGAudioFrame {
    /// 音频数据。
    char* data;

    /// 音频数据的长度。
    uint32_t length;

    /// 采样率。
    uint32_t sample_rate;

    /// 声道数。
    uint32_t channel;

    /// 时间戳，单位 ms。
    uint64_t timestamp;

    /// 位深度。
    ITMG_PCM_BITS_TYPE bits_type;
};

/**
 * 音频设备信息。
 *
 * 此数据结构在 {@link GetMicList} {@link GetSpeakerList} 接口中使用。
 */
struct TMGAudioDeviceInfo {
    /// 设备 ID。
    char device_id[2048];

    /// 设备名称。
    char device_name[2048];
};

/**
 * 麦克风权限类型。
 */
enum ITMG_MIC_PERMISSION {

    /// 麦克风已授权。
    ITMG_PERMISSION_GRANTED = 0,

    /// 麦克风被禁用。
    ITMG_PERMISSION_Denied = 1,

    /// 权限未知，请请求权限。
    ITMG_PERMISSION_NotDetermined = 2,

};

/**
 * 控制远端用户音频设备子事件。
 */
enum ITMG_ROOM_MANAGEMENT_OPERATOR {

    /// 控制麦克风事件。
    ITMG_ROOM_MANAGEMENT_MIC_OP = 0,

    /// 禁止操作麦克风及扬声器事件。
    ITMG_ROOM_MANAGERMENT_FORBID_OP = 1,

};

/**
 * 变声特效。
 *
 * 变声特效可以作用于人声之上，通过声学算法对人声进行二次处理，以获得与原始声音所不同的音色。
 */
enum ITMG_VOICE_TYPE {

    /// 关闭特效。
    ITMG_VOICE_TYPE_ORIGINAL_SOUND = 0,

    /// 幼稚园。
    ITMG_VOICE_TYPE_KINDER_GARTEN = 1,

    /// 萝莉。
    ITMG_VOICE_TYPE_LOLITA = 2,

    /// 大叔。
    ITMG_VOICE_TYPE_UNCLE = 3,

    /// 重金属。
    ITMG_VOICE_TYPE_HEAVY_MENTAL = 4,

    /// 感冒。
    ITMG_VOICE_TYPE_INFLUENZA = 5,

    /// 歪果仁。
    ITMG_VOICE_TYPE_DIALECT = 6,

    /// 困兽。
    ITMG_VOICE_TYPE_CAGED_ANIMAL = 7,

    /// 死肥宅。
    ITMG_VOICE_TYPE_DEAD_FATBOY = 8,

    /// 强电流。
    ITMG_VOICE_TYPE_STRONG_CURRENT = 9,

    /// 重机器。
    ITMG_VOICE_TYPE_HEAVY_MACHINE = 10,

    /// 空灵。
    ITMG_VOICE_TYPE_INTANGIBLE = 11

};

#endif /* __GME_TYPE_H__ */
