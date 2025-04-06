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
    init(appId: string, userId: string): Promise<GMEResult>;
    uninit(): Promise<GMEResult>;
    poll(): void;
    enterRoom(roomId: string, openId: string): Promise<GMEResult>;
    exitRoom(): Promise<GMEResult>;
    getDevicesList(): Promise<GMEVoiceResult>;
    getCurrentDevice(deviceType: number): Promise<GMEVoiceResult>;
    setCurrentDevice(deviceType: number, deviceId: string): Promise<GMEResult>;
    enableMic(enabled: boolean): Promise<GMEResult>;
    enableSpeaker(enabled: boolean): Promise<GMEResult>;
    getVolume(): Promise<GMEVoiceResult>;
    setVolume(volume: number): Promise<GMEResult>;
    playEffect(effectId: number, filePath: string): Promise<GMEResult>;
    stopEffect(effectId: number): Promise<GMEResult>;
    stopAllEffects(): Promise<GMEResult>;
    startRecordingWithTranslation(language: string): Promise<GMEResult>;
    stopRecording(): Promise<GMEResult>;
    setVoiceType(voiceType: number): Promise<GMEResult>;
}
export interface GMENativeInterface {
    TME_Poll: () => void;
    TME_Init: (appId: string, channel: number) => number;
    TME_UnInit: () => number;
    TME_EnterRoom: (roomId: string, openId: string, roomType: number) => number;
    TME_ExitRoom: () => number;
    TME_GetDevicesList: (deviceType: number) => string;
    TME_GetCurrentDevice: (deviceType: number) => string;
    TME_SetCurrentDevice: (deviceType: number, deviceId: string) => number;
    TME_EnableMic: (enabled: boolean) => number;
    TME_EnableSpeaker: (enabled: boolean) => number;
    TME_GetVolume: () => number;
    TME_SetVolume: (volume: number) => number;
    TME_PlayEffect: (effectId: number, filePath: string) => number;
    TME_StopEffect: (effectId: number) => number;
    TME_StopAllEffects: () => number;
    TME_StartRecordingWithTranslation: (language: string) => number;
    TME_StopRecording: () => number;
    TME_SetVoiceType: (voiceType: number) => number;
}
