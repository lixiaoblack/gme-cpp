/**
 * Copyright (c) 2025 Tencent. All rights reserved.
 * Module：   TMGAudioCtrl@GMESDK
 * Function： 腾讯云 GME 音频控制相关功能接口。
 */
#ifndef __GME_AUDIO_CTRL_H__
#define __GME_AUDIO_CTRL_H__

#include "av_error.h"
#include "av_type.h"

class ITMGAudioFrameCallback {
   public:
    virtual ~ITMGAudioFrameCallback() {
    }

    /**
     * 本地采集并经过音频模块前处理后的音频数据回调。
     *
     * 当您设置完音频数据自定义回调之后，SDK 内部会把刚采集到并经过前处理（ANS、AEC、AGC）之后的数据，以 PCM 格式的形式通过本接口回调给您。
     * - 此接口回调出的音频时间帧长固定为0.02s，格式为 PCM 格式。
     * - 由时间帧长转化为字节帧长的公式为 `采样率 × 时间帧长 × 声道数 × 采样点位宽`。
     * - 以 GME 默认的音频录制格式48000采样率、单声道、16采样点位宽为例，字节帧长为 `48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节`。
     * @param frame PCM 格式的音频数据帧。
     * @note
     * 1. 请不要在此回调函数中做任何耗时操作，由于 SDK 每隔 20ms 就要处理一帧音频数据，如果您的处理时间超过 20ms，就会导致声音异常。
     * 2. 此接口回调出的音频数据是可读写的，也就是说您可以在回调函数中同步修改音频数据，但请保证处理耗时。
     * 3. 此接口回调出的音频数据已经经过了前处理(ANS、AEC、AGC），但**不包含**背景音、音效、混响等前处理效果，延迟较低。
     */
    virtual void OnCapturedAudioFrame(TMGAudioFrame* frame) {
    }

    /**
     * 本地采集并经过音频模块前处理、音效处理和混 BGM 后的音频数据回调。
     *
     * 当您设置完音频数据自定义回调之后，SDK 内部会把刚采集到并经过前处理、音效处理和混 BGM 之后的数据，在最终进行网络编码之前，以 PCM 格式的形式通过本接口回调给您。
     * - 此接口回调出的音频时间帧长固定为0.02s，格式为 PCM 格式。
     * - 由时间帧长转化为字节帧长的公式为`采样率 × 时间帧长 × 声道数 × 采样点位宽`。
     * - 以 GME 默认的音频录制格式48000采样率、单声道、16采样点位宽为例，字节帧长为`48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节`。
     * @param frame PCM 格式的音频数据帧。
     * @note
     * 1. 请不要在此回调函数中做任何耗时操作，由于 SDK 每隔 20ms 就要处理一帧音频数据，如果您的处理时间超过 20ms，就会导致声音异常。
     * 2. 此接口回调出的音频数据是可读写的，也就是说您可以在回调函数中同步修改音频数据，但请保证处理耗时。
     * 3. 此接口回调出的数据已经经过了前处理（ANS、AEC、AGC）、音效和混 BGM 处理，声音的延迟相比于 {@link OnCapturedAudioFrame} 要高一些。
     */
    virtual void OnLocalProcessedAudioFrame(TMGAudioFrame* frame) {
    }

    /**
     * 混音前的每一路远程用户的音频数据。
     *
     * 当您设置完音频数据自定义回调之后，SDK 内部会把远端的每一路原始数据，在最终混音之前，以 PCM 格式的形式通过本接口回调给您。
     * - 此接口回调出的音频时间帧长固定为0.02s，格式为 PCM 格式。
     * - 由时间帧长转化为字节帧长的公式为`采样率 × 时间帧长 × 声道数 × 采样点位宽`。
     * - 以 GME 默认的音频录制格式48000采样率、单声道、16采样点位宽为例，字节帧长为`48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节`。
     * @param frame PCM 格式的音频数据帧。
     * @param userId 用户标识。
     * @note 此接口回调出的音频数据是只读的，不支持修改。
     */
    virtual void OnPlayAudioFrame(TMGAudioFrame* frame, const char* userId) {
    }

    /**
     * 将各路待播放音频混合之后并在最终提交系统播放之前的数据回调。
     *
     * 当您设置完音频数据自定义回调之后，SDK 内部会把各路待播放的音频混合之后的音频数据，在提交系统播放之前，以 PCM 格式的形式通过本接口回调给您。
     * - 此接口回调出的音频时间帧长固定为0.02s，格式为 PCM 格式。
     * - 由时间帧长转化为字节帧长的公式为 `采样率 × 时间帧长 × 声道数 × 采样点位宽`。
     * - 以 GME 默认的音频录制格式48000采样率、单声道、16采样点位宽为例，字节帧长为 `48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节`。
     * @param frame PCM 格式的音频数据帧。
     * @note
     * 1. 请不要在此回调函数中做任何耗时操作，由于 SDK 每隔 20ms 就要处理一帧音频数据，如果您的处理时间超过 20ms，就会导致声音异常。
     * 2. 此接口回调出的音频数据是可读写的，也就是说您可以在回调函数中同步修改音频数据，但请保证处理耗时。
     * 3. 此接口回调出的是对各路待播放音频数据的混合，但其中并不包含耳返的音频数据。
     */
    virtual void OnMixedPlayAudioFrame(TMGAudioFrame* frame) {
    }

    /**
     * SDK 所有音频混合后的音频数据（包括采集到的和待播放的）。
     *
     * 当您设置完音频数据自定义回调之后，SDK 内部会把所有采集到的和待播放的音频数据混合起来，以 PCM 格式的形式通过本接口回调给您，便于您进行自定义录制。
     * - 此接口回调出的音频时间帧长固定为0.02s，格式为 PCM 格式。
     * - 由时间帧长转化为字节帧长的公式为 `采样率 × 时间帧长 × 声道数 × 采样点位宽`。
     * - 以 GME 默认的音频录制格式48000采样率、单声道、16采样点位宽为例，字节帧长为 `48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节`。
     * @param frame PCM 格式的音频数据帧。
     * @note
     * 1. 此接口回调出的是SDK所有音频数据的混合数据，包括：经过 3A 前处理、特效叠加以及背景音乐混音后的本地音频，所有远端音频，但不包括耳返音频。
     * 2. 此接口回调出的音频数据不支持修改。
     */
    virtual void OnMixedAllAudioFrame(TMGAudioFrame* frame) {
    }
};

class ITMGAudioCtrl {
   public:
    virtual ~ITMGAudioCtrl() {
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                              音频控制
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 开启或关闭麦克风。
     *
     * 此接口用来开启/关闭麦克风，并推送音频流到房间，需要在**接收到进房成功事件后**调用，加入房间默认不会打开麦克风。
     * {@link EnableMic}(boolean) = {@link EnableAudioCaptureDevice}(boolean) + {@link EnableAudioSend}(boolean)。
     * @param enable true：开启  false：关闭。
     */
    virtual int EnableMic(bool enable) = 0;

    /**
     * 麦克风状态获取。
     *
     * 此接口用于获取麦克风状态，返回值 0 为关闭麦克风状态，返回值 1 为打开麦克风状态。
     */
    virtual int GetMicState() = 0;

    /**
     * 开启或关闭扬声器。
     *
     * 此接口用于开启/关闭扬声器，并接收远端的音频流，需要在**接收到进房成功事件后**调用，加入房间默认不会打开扬声器。
     * {@link EnableSpeaker}(boolean) = {@link EnableAudioPlayDevice}(boolean) + {@link EnableAudioRecv}(boolean)。
     * @param enable true：开启  false：关闭。
     */
    virtual int EnableSpeaker(bool enable) = 0;

    /**
     * 扬声器状态获取。
     *
     * 此接口用于扬声器状态获取。返回值 0 为关闭扬声器状态，返回值 1 为打开扬声器状态。
     */
    virtual int GetSpeakerState() = 0;

    /**
     * 开启音频采集设备。
     *
     * 此接口用来开启/关闭采集设备，需要在**接收到进房成功事件后**调用，加入房间默认不打开采集设备。
     * @param enable true：开启  false：关闭。
     */
    virtual int EnableAudioCaptureDevice(bool enable) = 0;

    /**
     * 获取采集设备状态。
     *
     * 此接口用于采集设备状态获取。
     */
    virtual bool IsAudioCaptureDeviceEnabled() = 0;

    /**
     * 开启或关闭播放设备。
     *
     * 此接口用于开启关闭播放设备，需要在**接收到进房成功事件后**调用，加入房间默认不打开播放设备。
     * @param enable true：开启  false：关闭。
     */
    virtual int EnableAudioPlayDevice(bool enable) = 0;

    /**
     * 获取播放设备状态。
     *
     * 此接口用于获取播放设备状态。
     */
    virtual bool IsAudioPlayDeviceEnabled() = 0;

    /**
     * 打开关闭音频上行。
     *
     * 此接口用于打开/关闭音频上行，需要在**接收到进房成功事件后**调用，用来控制否是将采集到的音频数据发送给远端用户。如果采集设备已经打开，那么会发送采集到的音频数据。如果采集设备没有打开，那么仍旧无声。采集设备的打开关闭请参见接口  {@link
     * EnableAudioCaptureDevice}。
     * @param enable true：打开  false：关闭。
     */
    virtual int EnableAudioSend(bool enable) = 0;

    /**
     * 获取音频上行状态。
     *
     * 此接口用于音频上行状态获取。
     */
    virtual bool IsAudioSendEnabled() = 0;

    /**
     * 打开或关闭音频下行。
     *
     * 此接口用于打开/关闭音频下行，需要在**接收到进房成功事件后**调用，用于控制是否接收远端用户的音频数据。如果播放设备已经打开，那么会播放房间里其他人的音频数据。如果播放设备没有打开，那么仍旧无声。播放设备的打开关闭参见接口请参见接口 {@link
     * EnableAudioPlayDevice}。
     * @param enable true：打开  false：关闭。
     */
    virtual int EnableAudioRecv(bool enable) = 0;

    /**
     * 获取音频下行状态。
     *
     * 此接口用于音频下行状态获取。
     */
    virtual bool IsAudioRecvEnabled() = 0;

    /**
     * 设置麦克风音量的增益。
     *
     * 此接口用于设置麦克风音量的增益。
     * @param volume 音量增益大小，取值 [0，200]，其中 100 为默认值，相当于音量大小不增不减。
     */
    virtual int SetMicVolume(int volume) = 0;

    /**
     * 获取麦克风音量的增益。
     *
     * 获取使用 {@link SetMicVolume} 设置的麦克风音量增益大小。
     */
    virtual int GetMicVolume() = 0;

    /**
     * 设置扬声器音量增益。
     *
     * 此接口用于设置扬声器音量的增益。
     * @param volume 音量增益大小，取值 [0，200]，其中 100 为默认值，相当于音量大小不增不减。
     */
    virtual int SetSpeakerVolume(int vol) = 0;

    /**
     * 获取扬声器音量增益。
     *
     * 获取使用 {@link SetSpeakerVolume} 设置的扬声器音量增益大小。
     */
    virtual int GetSpeakerVolume() = 0;

    /**
     * 动态设置房间内某成员音量增益。
     *
     * @param userID      用户 ID。
     * @param volume      音量增益大小，取值 [0，200]，其中 100 为默认值，相当于音量大小不增不减。
     */
    virtual int SetSpeakerVolumeByUserID(const char* userID, int volume) = 0;

    /**
     * 获取设置的房间内成员音量增益。
     *
     * 获取通过{@link SetSpeakerVolumeByUserID}接口设置的音量增益。
     * @param userID      用户 ID。
     */
    virtual int GetSpeakerVolumeByUserID(const char* userID) = 0;

    /**
     * 开启/关闭耳返。
     *
     * @param enable      是否开启耳返。
     */
    virtual int EnableLoopBack(bool enable) = 0;

    /**
     * 禁言房间中某个用户。
     *
     * 将某个用户加入音频数据黑名单，即不接受该用户的语音， 只对本端生效，不会影响其他端。返回值为 0 表示调用成功。
     * @param userID      用户 ID。
     */
    virtual int AddAudioBlackList(const char* userID) = 0;

    /**
     * 解除对房间中某个用户的禁言。
     *
     * 将某个用户移除音频数据黑名单。返回值为0表示调用成功。
     * @param userID      用户 ID。
     */
    virtual int RemoveAudioBlackList(const char* userID) = 0;

    /**
     * 查询某个用户是否被禁言。
     *
     * @param userID      用户 ID。
     */
    virtual bool IsUserIDInAudioBlackList(const char* userID) = 0;

    /**
     * 初始化 3D 音效处理器。
     *
     * 注意此接口已经废除，无需调用。
     */
    virtual int InitSpatializer(const char* modelPath) = 0;

    /**
     * 开启或关闭 3D 音效。
     *
     * 此函数用于开启或关闭 3D 音效。开启之后可以听到 3D 音效。
     * @param enable           是否开启 3D 音效。
     */
    virtual int EnableSpatializer(bool enable) = 0;

    /**
     * 获取当前 3D 音效状态。
     *
     * 此函数用于获取当前 3D 音效状态。
     */
    virtual bool IsEnableSpatializer() = 0;

    /**
     * 获取麦克风设备数量（仅适用于桌面端）。
     */
    virtual int GetMicListCount() = 0;

    /**
     * 枚举麦克风设备（仅适用于桌面端）。
     *
     * 此接口用来枚举麦克风设备。配合{@link GetMicListCount}接口使用。
     * @param ppDeviceInfoList      设备信息数组指针。
     * @param nCount                设备信息数字长度。
     */
    virtual int GetMicList(TMGAudioDeviceInfo* ppDeviceInfoList, int nCount) = 0;

    /**
     * 选择麦克风设备（仅适用于桌面端）。
     *
     * 此接口用来选中麦克风设备。如果不调用或者传入 "0"，则选中系统默认设备。GetMicList 接口中返回的第0个设备 id
     * 为默认设备，未选中设备时通话设备为默认设备，选中后业务层维护通话设备。如果该通话设备被拔出，则此时通话设备为默认设备，拔出的通话设备插入后，此时通话设备恢复为插入的通话设备。
     * @param micId      麦克风设备 ID，设备 ID 来自于{@link GetMicList}返回的设备信息列表。
     */
    virtual int SelectMic(const char* micId) = 0;

    /**
     * 获取当前使用的麦克风设备（仅适用于桌面平台）。
     *
     * 此接口用来获取当前使用的麦克风设备。
     * @param pDeviceInfo      设备信息指针。
     */
    virtual int GetCurrentMic(TMGAudioDeviceInfo* pDeviceInfo) = 0;

    /**
     * 获取扬声器设备数量（仅适用于桌面端）。
     */
    virtual int GetSpeakerListCount() = 0;

    /**
     * 枚举扬声器设备（仅适用于桌面端）。
     *
     * 此接口用来枚举扬声器设备。配合{@link GetSpeakerListCount}接口使用。
     * @param ppDeviceInfoList      设备信息数组指针。
     * @param nCount                设备信息数字长度。
     */
    virtual int GetSpeakerList(TMGAudioDeviceInfo* ppDeviceInfoList, int nCount) = 0;

    /**
     * 选择扬声器设备（仅适用于桌面端）。
     *
     * 此接口用来选中播放设备。如果不调用或者传入 "0"，则选中系统默认播放设备。
     * @param pSpeakerID      扬声器设备 ID，设备 ID 来自于{@link GetSpeakerList}返回设备信息列表。
     */
    virtual int SelectSpeaker(const char* pSpeakerID) = 0;

    /**
     * 获取当前使用的扬声器设备（仅适用于桌面端）。
     *
     * 此接口用来获取当前使用的扬声器设备。
     * @param pDeviceInfo      设备信息指针。
     */
    virtual int GetCurrentSpeaker(TMGAudioDeviceInfo* pDeviceInfo) = 0;

    /**
     * 启用音频自定义采集模式。
     *
     * 开启该模式后，SDK 不在运行原有的音频采集流程，即不再继续从麦克风采集音频数据，而是只保留音频编码和发送能力。
     * 您需要通过 {@link SendCustomAudioData} 不断地向 SDK 塞入自己采集的音频数据。
     * @param enable 是否启用，默认值：false。
     * @note 由于回声抵消（AEC）需要严格的控制声音采集和播放的时间，所以开启自定义音频采集后，AEC 能力可能会失效。
     */
    virtual int EnableCustomAudioCapture(bool enable) = 0;

    /**
     * 向 SDK 投送自己采集的音频数据。
     *
     * 请您精准地按每帧时长的间隔调用本接口，数据投送间隔不均匀时极易触发声音卡顿。
     * 参数 {@link TMGAudioFrame} 推荐下列填写方式（其他字段不需要填写）：
     * - data：音频帧 buffer。音频帧数据只支持 PCM 格式，支持[5ms ~ 100ms]帧长，推荐使用 20ms 帧长，长度计算方法：【48000采样率、单声道的帧长度：48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节】。
     * - sample_rate：采样率，支持：16000、24000、32000、44100、48000。
     * - channel：声道数（如果是立体声，数据是交叉的），单声道：1； 双声道：2。
     * - timestamp：时间戳，单位为毫秒（ms），请使用音频帧在采集时被记录下来的时间戳（可以在采集到一帧音频帧之后，通过调用 {@link generateCustomPTS} 获取时间戳）。
     * - bits_type：位深，目前版本只支持16bit--PCM_BITS_TYPE_FIXED_16。
     * @param frame 音频数据。
     */
    virtual int SendCustomAudioData(TMGAudioFrame* frame) = 0;

    /**
     * 开启音频自定义播放。
     *
     * @note 需要您在进入房间前设置才能生效，暂不支持进入房间后再设置。
     * 如果您需要外接一些特定的音频设备，或者希望自己掌控音频的播放逻辑，您可以通过该接口启用音频自定义播放。
     * 启用音频自定义播放后，SDK 将不再调用系统的音频接口播放数据，您需要通过 {@link GetCustomAudioRenderingFrame} 获取 SDK 要播放的音频帧并自行播放。
     * @param enable 是否启用音频自定义播放，默认为关闭状态。
     */
    virtual int EnableCustomAudioRendering(bool enable) = 0;

    /**
     * 获取可播放的音频数据。
     *
     * 调用该接口之前，您需要先通过 {@link EnableCustomAudioRendering} 开启音频自定义播放。
     * 参数 {@link TMGAudioFrame} 推荐下列填写方式（其他字段不需要填写）：
     * - sample_rate：采样率，必填，支持 16000、24000、32000、44100、48000。
     * - channel：声道数，必填，单声道请填1，双声道请填2，双声道时数据是交叉的。
     * - data：用于获取音频数据的 buffer。需要您根据一帧音频帧的帧长度分配好 data 的内存大小。获取的 PCM 数据支持 10ms 或 20ms 两种帧长，推荐使用 20ms 的帧长。
     * - bits_type：位深，目前版本只支持16bit--PCM_BITS_TYPE_FIXED_16
     * 计算公式为：采样率 x 播放时长 x 声道数量 x 2（每个样点的字节数）， 例如： 48000采样率、单声道、且播放时长为 20ms 的一帧音频帧的 buffer 大小为 48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节。
     * @param frame 音频数据帧。
     * @note
     *   1. 参数 frame 中的 sample_rate、channel 需提前设置好，同时分配好所需读取帧长的 data 空间。
     *   2. SDK 内部会根据 sample_rate 和 channel 自动填充 data 数据。
     *   3. 建议由系统的音频播放线程直接驱动该函数的调用，在播放完一帧音频之后，即调用该接口获取下一帧可播放的音频数据。
     */
    virtual int GetCustomAudioRenderingFrame(TMGAudioFrame* frame) = 0;

    /**
     * 开启音量回调。
     *
     * @param interval 音量回调周期，单位（秒）。
     * @note
     *   通过 ITMG_MAIN_EVENT_TYPE_USER_VOLUMES 事件返回用户音量数据。
     */
    virtual int TrackingVolume(float interval) = 0;

    /**
     * 停止音量回调。
     */
    virtual int StopTrackingVolume() = 0;

    /**
     * 获取实时麦克风音量。
     */
    virtual int GetMicLevel() = 0;

    /**
     * 获取实时扬声器音量。
     */
    virtual int GetSpeakerLevel() = 0;

    /**
     * 获取音频上行实时音量。
     */
    virtual int GetSendStreamLevel() = 0;

    /**
     * 获取房间内其他成员下行实时音量。
     *
     * @param userID 房间其他成员的 ID。
     */
    virtual int GetRecvStreamLevel(const char* userID) = 0;

    /**
     * 开始麦克风测试 [仅桌面端有效]。
     *
     * @param interval 麦克风音量的回调间隔。
     * @note
     *   通过 ITMG_MAIN_EVENT_TYPE_TEST_MIC_VOLUME 事件返回测试结果。
     */
    virtual int StartMicDeviceTest(int interval) = 0;

    /**
     * 结束麦克风测试 [仅桌面端有效]。
     */
    virtual int StopMicDeviceTest() = 0;

    /**
     * 开始扬声器测试 [仅桌面端有效]。
     *
     * @param filePath 声音文件的路径。
     * @note
     *   通过ITMG_MAIN_EVENT_TYPE_TEST_SPEAKER_VOLUME事件返回测试结果。
     */
    virtual int StartSpeakerDeviceTest(const char* filePath) = 0;

    /**
     * 结束扬声器测试 [仅桌面端有效]。
     */
    virtual int StopSpeakerDeviceTest() = 0;

    /**
     * 设置音频数据自定义回调。
     *
     * 设置该回调之后，SDK 内部会把音频数据（PCM 格式）回调出来，包括：
     * - {@link OnCapturedAudioFrame}：本地麦克风采集到的音频数据回调。
     * - {@link OnLocalProcessedAudioFrame}：本地采集并经过音频模块前处理后的音频数据回调。
     * - {@link OnPlayAudioFrame}：混音前的每一路远程用户的音频数据。
     * - {@link OnMixedPlayAudioFrame}：各路音频混合之后并最终要由系统播放出的音频数据回调。
     * - {@link OnMixedAllAudioFrame}：所有音频混合后的音频数据（包括采集到的和待播放的）回调。
     *
     * @note 设置回调为空即代表停止自定义音频回调，反之，设置回调不为空则代表启动自定义音频回调。
     */
    virtual int SetAudioFrameCallback(ITMGAudioFrameCallback* callback) = 0;
};
#endif /* __GME_AUDIO_CTRL_H__ */
