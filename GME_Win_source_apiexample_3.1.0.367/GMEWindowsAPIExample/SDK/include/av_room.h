/**
 * Copyright (c) 2025 Tencent. All rights reserved.
 * Module：   TMGRoom@GMESDK
 * Function：腾讯云 GME 房间相关功能接口。
 */
#ifndef __GME_ROOM_H__
#define __GME_ROOM_H__

class ITMGRoom {
   public:
    virtual ~ITMGRoom() {
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                              音频房间相关
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 获取语音房间 ID。
     */
    virtual const char* GetRoomID() = 0;

    /**
     * 修改用户房间音频类型。
     *
     * 该接口修改房间音频类型，仅对本端生效。
     * @param roomType 房间类型。
     */
    virtual int ChangeRoomType(ITMG_ROOM_TYPE roomType) = 0;

    /**
     * 获取当前房间类型。
     */
    virtual int GetRoomType() = 0;

    /**
     * 开始跨房连麦通话。
     *
     * 调用此接口进行跨房连麦，此接口需要在进房成功后调用。调用该接口后，本端可以与目标房间的目标 userID 用户进行连麦交流。
     * @param targetRoomID    目标房间 ID。
     * @param targetUserID    目标用户 ID。
     * @param userSig         用户鉴权信息。
     * @param userSigLen      用户鉴权信息长度。
     */
    virtual int StartRoomSharing(const char* targetRoomID, const char* targetUserID, const char* userSig, int userSigLen) = 0;

    /**
     * 结束跨房连麦通话。
     *
     * 调用此接口停止跨房连麦，此接口在进房后调用，调用接口后，本端可以停止与目标房间的目标 userID 用户的连麦交流。
     */
    virtual int StopRoomSharing() = 0;

    /**
     * 快速切换房间。
     *
     * @param targetRoomID     目标房间 ID。
     * @param userSig          用户鉴权信息。
     * @param userSigLen       用户鉴权信息长度, 仅 C++ 接口有效。
     */
    virtual int SwitchRoom(const char* targetRoomID, const char* userSig, int userSigLen) = 0;

    /**
     * 发送自定义消息。
     *
     * @param data             自定义数据。
     * @param length           自定义数据长度,仅 C++ 有效。
     * @param repeatCount      重试次数。
     */
    virtual int SendCustomData(const char* data, int length, int repeatCount) = 0;

    /**
     * 停止发送自定义消息。
     */
    virtual int StopSendCustomData() = 0;

    /**
     * 设置接收语音距离范围。
     *
     * 通过此方法用于设置接收的语音范围（距离以游戏引擎为准），只支持在进房成功后调用。
     * 此方法必须配合 {@link UpdateSelfPosition} 更新声源方位联合使用。
     * 此方法只需调用一次即可生效，支持修改。
     * @param range             范围语音的接收范围。
     */
    virtual int UpdateAudioRecvRange(float range) = 0;

    /**
     * 设置 3D 音效衰减距离。
     *
     * 3D 音效中，音源音量的大小与音源距离有一定的衰减关系。单位距离超过 range 之后，音量衰减到几乎为零。
     * @param range             3D 音效的衰减距离。
     */
    virtual int UpdateSpatializerRecvRange(float range) = 0;

    /**
     * 更新玩家在指定房间的声源方位及坐标。
     *
     * 更新玩家在指定房间的声源方位，服务器通过该位置来做玩家之间声音可达性的判断，实现范围语音效果，只支持在进房成功后调用，且需要每帧调用，以 Unity 引擎为例，此接口需要在 Update 中调用。
     * 如果需要同时使用 3D 音效效果，此接口中的参数 axisForward、axisRight 及 axisUp 需要按照 3D 语音的要求进行设置。
     * @param position       自身在世界坐标系中的坐标，顺序是前、右、上。
     * @param axisForward    自身坐标系前轴的单位向量。
     * @param axisRight      自身坐标系右轴的单位向量。
     * @param axisUp         自身坐标系上轴的单位向量。
     */
    virtual int UpdateSelfPosition(float position[3], float axisForward[3], float axisRight[3], float axisUp[3]) = 0;

    /**
     * 更新指定远端用户的空间坐标。
     *
     * 修改房间内指定用户的空间坐标，实现 3D 语音效果，默认情况下用户的空间坐标会自动扩散给房间内的其他用户，你也可以调用该接口手动指定远端用户的空间坐标。
     * @param userID            需要设置空间坐标的用户 ID。
     * @param position          用户的空间坐标。
     */
    virtual int UpdateOtherPosition(const char* userID, float position[3]) = 0;

    /**
     * 使用 SEI 通道发送自定义消息。
     *
     * 对端可通过 ITMG_MAIN_EVENT_TYPE_RECV_SEI_MSG 事件接收消息，repeatCount > 1时，接收消息回调也可能会收到多次相同的消息，需要去重。
     * @param message       自定义消息，消息最大长度 1000 字节。
     * @param repeatCount   发送数据次数，范围 [1,3]，repeatCount > 3 时，发送次数也会改成 3。
     */
    virtual int SendSEIMsg(const char* message, int repeatCount) = 0;
};
#endif /* __GME_ROOM_H__ */
