/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:04:45
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-06 09:05:20
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
export interface GMEConfig {
  appId: string;
}

export interface GMEResult {
  success: boolean;
  error?: number | string;
}

export interface GMEVoiceResult extends GMEResult {
  data?: any;
}

export interface GMEInstance {
  // 基础功能
  init(appId: string, userId: string): Promise<GMEResult>;
  uninit(): Promise<GMEResult>;
  poll(): void;

  // 房间管理
  enterRoom(roomId: string, openId: string): Promise<GMEResult>;
  exitRoom(): Promise<GMEResult>;

  // 设备管理
  getDevicesList(): Promise<GMEVoiceResult>;
  getCurrentDevice(deviceType: number): Promise<GMEVoiceResult>;
  setCurrentDevice(deviceType: number, deviceId: string): Promise<GMEResult>;

  // 音频控制
  enableMic(enabled: boolean): Promise<GMEResult>;
  enableSpeaker(enabled: boolean): Promise<GMEResult>;
  getVolume(): Promise<GMEVoiceResult>;
  setVolume(volume: number): Promise<GMEResult>;

  // 音效相关
  playEffect(effectId: number, filePath: string): Promise<GMEResult>;
  stopEffect(effectId: number): Promise<GMEResult>;
  stopAllEffects(): Promise<GMEResult>;

  // 语音转文字
  startRecordingWithTranslation(language: string): Promise<GMEResult>;
  stopRecording(): Promise<GMEResult>;

  // 变声特效
  setVoiceType(voiceType: number): Promise<GMEResult>;
}

export interface GMENativeInterface {
  // 基础功能
  TME_Poll: () => void;
  TME_Init: (appId: string, channel: number) => number;
  TME_UnInit: () => number;

  // 房间管理
  TME_EnterRoom: (roomId: string, openId: string, roomType: number) => number;
  TME_ExitRoom: () => number;

  // 设备管理
  TME_GetDevicesList: (deviceType: number) => string;
  TME_GetCurrentDevice: (deviceType: number) => string;
  TME_SetCurrentDevice: (deviceType: number, deviceId: string) => number;

  // 音频控制
  TME_EnableMic: (enabled: boolean) => number;
  TME_EnableSpeaker: (enabled: boolean) => number;
  TME_GetVolume: () => number;
  TME_SetVolume: (volume: number) => number;

  // 音效相关
  TME_PlayEffect: (effectId: number, filePath: string) => number;
  TME_StopEffect: (effectId: number) => number;
  TME_StopAllEffects: () => number;

  // 语音转文字
  TME_StartRecordingWithTranslation: (language: string) => number;
  TME_StopRecording: () => number;

  // 变声特效
  TME_SetVoiceType: (voiceType: number) => number;
}
