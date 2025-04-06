/**
 * Copyright (c) 2025 Tencent. All rights reserved.
 * Module：   GME 错误码表。
 * Function： 用于通知用户在使用GME过程中出现的错误。
 */
#ifndef __GME_ERROR_H__
#define __GME_ERROR_H__
namespace nsgme {
namespace av {

/**
 * 错误码。
 */
typedef enum AVError {

    /// 无错误。
    AV_OK = 0,

    /// 未知错误。
    AV_ERR_UNKNOWN = 1,

    /// 频繁操作。接口支持并发，但频繁操作会导致性能下降，请控制操作频率。
    AV_ERR_FREQUENT_OPERATION = 1000,

    /// 重复操作，已经在进行某种操作，再次去做同样的操作，则返回这个错误。
    AV_ERR_REPETITIVE_OPERATION = 1001,

    /// 互斥操作，已经在进行某种操作，再次去做同类型的其他操作，则返回这个错误。
    AV_ERR_EXCLUSIVE_OPERATION = 1002,

    /// 已经处于所要状态，无需再操作。如设备已经打开，再次去打开，就返回这个错误码。
    AV_ERR_HAS_IN_THE_STATE = 1003,

    /// 错误参数，请检查调 API 调用时传入的参数的有效性。
    AV_ERR_INVALID_ARGUMENT = 1004,

    /// 请求服务器超时。
    AV_ERR_TIMEOUT = 1005,

    /// 当前 API 暂未实现。
    AV_ERR_NOT_IMPLEMENTED = 1006,

    /// API 调用被拒绝。
    AV_ERR_REFUSED = 1007,

    /// license 不合法，调用失败。
    AV_ERR_INVALID_LICENSE = 1008,

    /// 断开连接。
    AV_ERR_DISCONNECTED = 1009,

    /// GME SDK 实例没有初始化，请先调用 Init 接口初始化 GME SDK。
    AV_ERR_CONTEXT_NOT_START = 1101,

    /// 房间不存在，当前操作需要在进房成功后调用。
    AV_ERR_ROOM_NOT_EXIST = 1201,

    /// 没有退出房间，当前操作需要在退出语音房间后调用。
    AV_ERR_ROOM_NOT_EXITED = 1202,

    /// 设备不存在，请检查音频设备是否有效。
    AV_ERR_DEVICE_NOT_EXIST = 1301,

    /// 服务器无法处理您的请求。
    AV_ERR_SERVER_FAIL = 10001,

    /// 录制参数为空，请检查传入的参数是否正确。
    AV_ERR_VOICE_RECORDER_PARAM_INVALID = 4097,

    /// 没有麦克风权限，请检查是否开启麦克风权限。
    AV_ERR_VOICE_RECORDER_NO_MIC_PERMISSION = 4098,

    /// 录制已经启动，请先停止录制。
    AV_ERR_VOICE_RECORDER_ALREADY_STARTED = 4099,

    /// 没有录到任何内容，可能是未开启采集，或录制时间太短。
    AV_ERR_VOICE_RECORD_AUDIO_TOO_SHORT = 4100,

    /// 打开文件失败，请确保文件路径正确，并检查是否有文件读写权限。
    AV_ERR_VOICE_RECORDER_OPENFILE_ERROR = 4101,

    /// 参数中设置的格式后缀不支持，请参考官方文档，录制成指定格式。
    AV_ERR_VOICE_RECORD_FORMAT_NOT_SUPPORTED = 4105,

    /// 录制失败，SDK 内部异常，可以提供日志给开发者。
    AV_ERR_VOICE_RECORDER_INNER_ERROR = 4106,

    /// 打开文件失败，请确保文件路径正确，并检查是否有文件读写权限。
    AV_ERR_VOICE_PLAYER_OPENFILE_ERROR = 20484,

    /// 待播放文件的格式不支持，解码失败。
    AV_ERR_VOICE_PLAYER_FORMAT_NOT_SUPPORTED = 20486,

    /// 请求媒体服务器列表失败，请检查网络是否正常，或网络防火墙是否放行 UDP。
    AV_ERR_REQUEST_IP_TIMEOUT = 7001,

    /// 连接媒体服务器超时。
    AV_ERR_CONNECT_SERVER_TIMEOUT = 7002,

    /// 进入语音房间失败。
    AV_ERR_ENTER_ROOM_FAILED = 7004,

    /// UserSig 校验失败，请检查调用 {@link EnterRoom} 操作时传入的 UserSig 是否有效。
    AV_ERR_AUTH_FIALD = 7006,

    /// 进房请求被拒绝，请检查是否连续调用 {@link EnterRoom} 进入相同 Id 的房间。
    AV_ERR_IN_OTHER_ROOM = 7007,

    /// 实时语音服务不可用，请检查：套餐包剩余分钟数是否大于 0，腾讯云账号是否欠费。
    AV_ERR_DISSOLVED_OVERUSER = 7008,

    /// 语音消息服务不可用，请检查：语音消息服务是否已开启，腾讯云账号是否欠费。
    AV_ERR_VOICE_MESSAGE_SERVICE_INVALID = 7009,

    /// 语音转文本服务不可用，请检查：语音转文本服务是否已开启，腾讯云账号是否欠费。
    AV_ERR_SPEECH_TO_TEXT_SERVICE_INVALID = 7010,

    /// 文本翻译服务不可用，请检查：文本翻译服务是否已开启，腾讯云账号是否欠费。
    AV_ERR_TRANSLATE_TEXT_SERVICE_INVALID = 7011,

    /// 文本转语音服务不可用，请检查：文文本转语音服务是否已开启，腾讯云账号是否欠费。
    AV_ERR_TEXT_TO_SPEECH_SERVICE_INVALID = 7012,

    /// 打开 BGM 音频文件失败，请检查传入的文件路径是否正确。
    AV_ERR_EFFECT_OPENFILE_FAILED = 4051,

    /// BMG 音频文件解码失败，请检查文件格式是否支持。
    AV_ERR_EFFECT_DECODER_FAILED = 4053,

    /// 数量超过限定值，如同时预加载两首背景音乐。
    AV_ERR_EFFECT_STATE_ILLIGAL = 4057,

    /// 创建录制文件失败，请检查传入的文件路径是否有效。
    AV_ERR_RECORD_CREATEFILE_FAILED = 5001,

    /// 不支持的录制文件格式，请检查传入的参数是否正确。
    AV_ERR_RECORD_FILE_FORAMT_NOTSUPPORT = 5002,

    /// 启动录制失败。
    AV_ERR_RECORD_START_FAILED = 5003,

    /// 音频设备操作被禁止。
    AV_ERR_OPERATOR_DEVICE_REFUSED = 6999,

    /// 上传文件时，文件访问错误，请检查待上传文件的路径是否正确。
    AV_ERR_VOICE_UPLOAD_FILE_ACCESSERROR = 8193,

    /// UserSig 校验失败，请检 UserSig 参数是否正确填写。
    AV_ERR_VOICE_UPLOAD_SIGN_CHECK_FAIL = 8194,

    /// 文件上传过程中出现服务器错误，请稍后重试。
    AV_ERR_VOICE_UPLOAD_COS_INTERNAL_FAIL = 8195,

    /// 获取文件上传 Token 时出现网络错误，请检查网络连接是否正常。
    AV_ERR_VOICE_UPLOAD_GET_TOKEN_NETWORK_FAIL = 8196,

    /// 获取文件上传 Token 时出现服务器错误，请稍后重试。
    AV_ERR_VOICE_UPLOAD_SYSTEM_INNER_ERROR = 8197,

    /// 上传文件时出现网络错误，请检查网络连接是否正常。
    AV_ERR_VOICE_UPLOAD_COS_NETWORK_FAIL = 8199,

    /// 下载文件时，传入的下载 URL 无效。
    AV_ERR_VOICE_DOWNLOAD_FILE_URL_INVALID = 12288,

    /// 下载文件时，文件访问错误，请检查传入的文件保存路径的合法性。
    AV_ERR_VOICE_DOWNLOAD_FILE_ACCESS_ERROR = 12289,

    /// 下载文件时 UserSig 校验失败，请检 UserSig 参数是否正确填写。
    AV_ERR_VOICE_DOWNLOAD_SIGN_CHECK_FAIL = 12290,

    /// 文件下载过程中出现服务器错误，请稍后重试。
    AV_ERR_VOICE_DOWNLOAD_COS_INTERNAL_FAIL = 12291,

    /// 获取文件下载 Token 时出现网络错误，请检查网络连接是否正常。
    AV_ERR_VOICE_DOWNLOAD_GET_SIGN_NETWORK_FAIL = 12293,

    /// 获取文件下载 Token 时出现服务器错误，请稍后重试。
    AV_ERR_VOICE_DOWNLOAD_SYSTEM_INNER_ERROR = 12294,

    /// 下载文件时出现网络错误，请检查网络连接是否正常。
    AV_ERR_VOICE_DOWNLOAD_COS_NETWORK_FAIL = 12298,

    /// 同时下载的文件数量超过限定值，支持同时下载的文件最大个数为 50。
    AV_ERR_VOICE_DOWNLOAD_OVER_LIMIT = 12299,

    /// 系统问题导致转文本失败。
    AV_ERR_VOICE_S2T_SYSTEM_INTERNAL_ERROR = 32769,

    /// 网络问题链接服务失败。
    AV_ERR_VOICE_S2T_NETWORK_FAIL = 32770,

    /// 网络问题转文本失败。
    AV_ERR_VOICE_S2T_RSP_DATA_DECODE_FAIL = 32772,

    /// 转文本鉴权失败。
    AV_ERR_VOICE_S2T_SIGN_CHECK_FAIL = 32776,

    /// 语音转文本参数错误。
    AV_ERR_VOICE_S2T_PARAM_NULL = 32784,

    /// 语音转文本云 asr 错误。
    AV_ERR_VOICE_S2T_AUTO_SPEECH_REC_ERROR = 32785,

    /// 语音转文本正在进行中，重复操作。
    AV_ERR_VOICE_STREAMIN_RUNING_ERROR = 32786,

    /// 文件在腾讯云 cos 上找不到。
    AV_ERR_VOICE_FILE_COS_NOT_FOUND = 32789,

    /// 垮房连麦参数无效。
    AV_ERR_SHARE_ROOM_INVALID_VALUE = 700004,

    /// 垮房连麦超时。
    AV_ERR_SHARE_ROOM_TIMEOUT = 700008,

    /// 当前是观众角色，不能请求或断开跨房连麦，需要先切换到主播。
    AV_ERR_SHARE_ROOM_TYPE_ERR = 700016,

} AVError;
}  // namespace av
}  // namespace nsgme
#endif /* __GME_ERROR_H__ */
