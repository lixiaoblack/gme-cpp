/**
 * Copyright (c) 2025 Tencent. All rights reserved.
 * Module：   TMGContext@GMESDK
 * Function： 腾讯云 GME 主功能接口。
 */
#ifndef __GME_SDK_H__
#define __GME_SDK_H__

#include "av_error.h"
#include "av_type.h"
#include "av_room.h"
#include "av_ptt.h"
#include "av_audio_ctrl.h"
#include "av_audio_effect_ctrl.h"
class ITMGContext;

/////////////////////////////////////////////////////////////////////////////////
//
//                              基础功能
//
/////////////////////////////////////////////////////////////////////////////////

/**
 * 获取 GME SDK 实例（单例模式）。
 *
 * 你可以按照下列方式获取 GME 实例，不要 delete ITMGContext*，该实例为单例。
 * <pre>
 * ITMGContext *gmeContext = ITMGContextGetInstance();
 * </pre>
 */
GME_API ITMGContext* GMESDK_CALL ITMGContextGetInstance();

/**
 * 本地计算鉴权码。
 *
 * 为了 key 的安全，建议使用者在后台计算鉴权码， 更多详情请参见[UserSig相关](https://cloud.tencent.com/document/product/607/101375)。
 * @param appId             在腾讯云申请的 GME 应用 appid。
 * @param roomId            语音房间 id，可以填空字符串。
 * @param userId            唯一标识一个用户，规则由应用开发者自行制定。
 * @param key               在腾讯云申请的 GME 应用对应的 key。
 * @param authBuffer        存储鉴权码的缓冲区。
 * @param authBufferLen     缓冲区长度 。
 */
GME_API int GMESDK_CALL QAVSDK_AuthBuffer_GenAuthBuffer(unsigned int appId, const char* roomId, const char* userId, const char* key, unsigned char* authBuffer, unsigned int authBufferLen);

class ITMGDelegate {
   public:
    virtual ~ITMGDelegate() {
    }

    /**
     * GME SDK 事件回调接口。
     * @param type  事件类型。
     * @param data  事件参数。
     */
    virtual void OnEvent(ITMG_MAIN_EVENT_TYPE type, const char* data) = 0;
};

class ITMGContext {
   protected:
    virtual ~ITMGContext() {
    }

   public:
    /**
     * 创建 GME 子实例。
     *
     * 您可以调用该接口创建 TMGContext 子实例，可以使用和主实例相同（也支持使用不同的）的 appid 和 userid 初始化子实例，并进入指定的音频房间。
     */
    virtual ITMGContext* CreateSubInstance() = 0;

    /**
     * 销毁子实例。
     *
     * 通过主实例调用该接口，销毁 SubInstance 子实例。
     * @param subInstance 子房间实例。
     */
    virtual void DestroySubInstance(ITMGContext* subInstance) = 0;

    /**
     * 设置 GME SDK 事件回调。
     *
     * GME SDK 内部到事件会通过该回调返回给上层应用。
     * @param delegate 事件回调对象。
     */
    virtual void SetTMGDelegate(ITMGDelegate* delegate) = 0;

    /**
     * 设置接入点（已废弃）。
     *
     * 通常不需要主动调用该接口，SDK 内部会根据 AppID 自动选择接入点，如果 AppID 是国内站申请的，就自动连接国内站环境；如果是 AppID 是国际站申请的，就自动连接国际站环境。
     * @param region 接入点，可选择 "REGION_CHN"（国内站），"REGION_INTERNATIONAL"（国际站），该接口需要在 {@link Init} 之前调用。
     */
    virtual void SetRegion(const char* region) = 0;

    /**
     * 初始化 GME SDK。
     *
     * 在使用 GME SDK 相关功能时需要先调用该接口进行初始化。
     * @param appId    在腾讯云申请的 GME 应用 appid。
     * @param userID   当前用户的 userId，相当于用户名，使用 UTF-8 编码，最大 127 个字符。
     */
    virtual int Init(const char* appId, const char* userID) = 0;

    /**
     * 触发事件回调。
     *
     * Poll 是 GME 的消息泵，应用需要周期性的调用 Poll 接口触发事件回调；如果没有调用 Poll ，将会导致整个 SDK 服务运行异常。
     */
    virtual void Poll() = 0;

    /**
     * 系统暂停。
     *
     * 当系统发生 Pause 事件时，需要同时通知引擎进行 Pause。如果不需要后台播放房间内声音，请调用 Pause 接口暂停整个 GME 服务。
     */
    virtual int Pause() = 0;

    /**
     * 系统恢复。
     *
     * 当系统发生 Resume 事件时，需要同时通知引擎进行 Resume；Resume 接口只恢复实时语音。
     */
    virtual int Resume() = 0;

    /**
     * 反初始化 GME SDK。
     *
     * 使用 GME SDK 相关功能后，调用该接口释放相关资源，如果游戏业务侧账号与 UserID 是绑定的，那切换游戏账号需要反初始化 GME，再用新的 UserID 初始化。
     */
    virtual int Uninit() = 0;

    /**
     * 设置应用场景。
     *
     * 默认为语聊场景，单个房间最多支持 300 人同时在线，最高支持 50 人同时发言，适用于 [语音聊天]、[在线狼人杀] [游戏开黑]等业务场景；
     * 如果单个房间内的人数超过 300 人，需要在调用 {@link EnterRoom} 接口前调用该接口切换成音频直播场景。
     */
    virtual int SetScene(ITMG_APP_SCENE scene) = 0;

    /**
     * 切换角色。
     *
     * 调用本接口可以实现用户在 【主播】 和 【观众】 两种角色之间来回切换。
     * 在语音互动直播或国战语音场景，只有主播才能发布音频流，听众只能接收音频流，如果观众需要发布音频流，需要调用该接口将角色切换成主播。
     */
    virtual int SetAudioRole(ITMG_AUDIO_MEMBER_ROLE role) = 0;

    /**
     * 设置范围语音模式。
     *
     * 进房前，通过调用此接口修改语音模式，影响下一次进房。
     * 进房后，通过调用此接口修改语音模式，将直接改变当前用户的语音模式。
     * 退房后，此参数不会自动重置为，所以一旦决定调用此方法，请在每次 {@link EnterRoom} 之前都调用此方法设置语音模式。
     */
    virtual int SetRangeAudioMode(ITMG_RANGE_AUDIO_MODE rangeAudioMode) = 0;

    /**
     * 设置范围语音的 TeamID。
     *
     * 进房前，通过此接口设置队伍号，影响下一次进房。
     * 进房后，可通过此接口修改队伍号，设置后立即生效。
     * 退房后，TeamID 不会自动重置为 0，所以一旦决定调用此语音模式，请在每次 {@link EnterRoom} 之前都调用此方法设置 TeamID。
     * 退房后再进房，请在退房成功回调回来之后再调用设置队伍号接口。
     */
    virtual int SetRangeAudioTeamID(int teamID) = 0;

    /**
     * 加入语音房间。
     *
     * 加入到指定的语音房间，本接口是异步的，进房结果通过 {@link ITMG_MAIN_EVENT_TYPE} 的 ITMG_MAIN_EVENT_TYPE_ENTER_ROOM 事件通知上层应用。
     * @param roomId 语音房间号，最大支持 64 字符。
     * @param roomType 房间类型。
     * @param userSig 用户鉴权信息。
     * @param userSigLen 用户鉴权信息长度。
     */
    virtual int EnterRoom(const char* room_id, ITMG_ROOM_TYPE room_type, const char* userSig, int userSigLen) = 0;

    /**
     * 退出语音房间。
     *
     * 通过调用此接口可以退出所在房间。本接口是异步的，返回值为 AV_OK 的时候代表异步调用成功。退房结果通过 {@link ITMG_MAIN_EVENT_TYPE} 的ITMG_MAIN_EVENT_TYPE_EXIT_ROOM 事件通知上层应用。
     * @note
     * 如果应用中有退房后立即进房的场景，在接口调用流程上，开发者无需要等待 ExitRoom 的回调通知，只需直接调用接口。
     */
    virtual int ExitRoom() = 0;

    /**
     * 是否已经在语音房间内。
     *
     * 通过调用此接口可以判断是否已经进入房间，返回值为 bool 类型。
     */
    virtual bool IsRoomEntered() = 0;

    /**
     * 设置打印日志等级。
     *
     * 用于设置日志等级，建议保持默认等级，需要在 Init 之前调用。
     * @param levelWrite 写文件日志等级。
     * @param levelPrint 打印日志等级。
     */
    virtual int SetLogLevel(ITMG_LOG_LEVEL levelWrite, ITMG_LOG_LEVEL levelPrint) = 0;

    /**
     * 设置日志文件保存路径。
     *
     * 用于设置打印日志路径，需要在 Init 之前调用。
     * @param logDir 日志文件存储路径。
     */
    virtual int SetLogPath(const char* logDir) = 0;

    /**
     * 获取日志文件保存路径。
     */
    virtual const char* GetLogPath() = 0;

    /**
     * 获取 GME SDK 版本。
     */
    virtual const char* GetSDKVersion() = 0;

    /**
     * 设置 App 版本号。
     *
     * APP 可以将自己的版本号设置给 GME SDK，用于内部信息上报。
     * @param appVersion 应用版本信息。
     */
    virtual void SetAppVersion(const char* appVersion) = 0;

    /**
     * 显示仪表盘。
     *
     * 仪表盘是状态统计和事件消息浮层 view，方便调试。
     * @param show  是否显示仪表盘。
     */
    virtual int ShowDebugView(bool show) = 0;

    /**
     * 获取房间相关的类实例。
     */
    virtual ITMGRoom* GetRoom() = 0;

    /**
     * 获取离线语音相关的类实例。
     */
    virtual ITMGPTT* GetPTT() = 0;

    /**
     * 获取音频控制相关的类实例。
     */
    virtual ITMGAudioCtrl* GetAudioCtrl() = 0;

    /**
     * 获取音频音效控制相关的类实例。
     */
    virtual ITMGAudioEffectCtrl* GetAudioEffectCtrl() = 0;

    /**
     * 设置最大混音路数。
     *
     * 使用此接口可以设置最大混音路数，在进房前调用。
     * @param count 最大混音路数。
     */
    virtual int SetRecvMixStreamCount(int count) = 0;

    /**
     * 设置高级功能参数。
     *
     * @param key 参数名。
     * @param value 参数值。
     */
    virtual int SetAdvanceParams(const char* key, const char* value) = 0;

    /**
     * 获取高级功能参数值。
     */
    virtual const char* GetAdvanceParams(const char* key) = 0;

    /**
     * 查询麦克风权限。
     */
    virtual ITMG_MIC_PERMISSION CheckMicPermission() = 0;
};
#endif /* __GME_SDK_H__ */
