import { GMEConfig, GMEInstance, GMEResult, GMEVoiceResult } from "./types";
export declare class GME implements GMEInstance {
    private wrapper;
    private pollInterval?;
    private appId;
    constructor(config: GMEConfig);
    private startPolling;
    private stopPolling;
    poll(): void;
    init(appId: string, userId: string): Promise<GMEResult>;
    uninit(): Promise<GMEResult>;
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
