declare module "electron-gme-sdk" {
  interface GMEResult {
    success: boolean;
    error?: string;
    devices?: Array<{
      deviceId: string;
      deviceName: string;
    }>;
  }

  class GMEWrapper {
    constructor();
    init(appId: string, userId: string): Promise<GMEResult>;
    enterRoom(roomId: string, openId: string): Promise<GMEResult>;
    exitRoom(): Promise<GMEResult>;
    enableMic(enabled: boolean): Promise<GMEResult>;
    enableSpeaker(enabled: boolean): Promise<GMEResult>;
    getSpeakerList(): Promise<GMEResult>;
    selectSpeakerDevice(deviceId: string): Promise<GMEResult>;
    getCurrentSpeakerDevice(): Promise<GMEResult>;
  }

  export default GMEWrapper;
}
