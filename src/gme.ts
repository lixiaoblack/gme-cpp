import { GMEConfig, GMEInstance, GMEResult, GMEVoiceResult } from "./types.js";
import _bindings from "bindings";
const bindings = _bindings;

const native = bindings("gme_native");

export class GME implements GMEInstance {
  private wrapper: any;
  private pollInterval?: NodeJS.Timeout;
  private appId: string;

  constructor(config: GMEConfig) {
    this.wrapper = new native.GMEWrapper();
    this.appId = config.appId;
    this.startPolling();
  }

  private startPolling(): void {
    this.pollInterval = setInterval(() => {
      this.poll();
    }, 100);
  }

  private stopPolling(): void {
    if (this.pollInterval) {
      clearInterval(this.pollInterval);
      this.pollInterval = undefined;
    }
  }

  public poll(): void {
    this.wrapper.poll();
  }

  public async init(appId: string, userId: string): Promise<GMEResult> {
    try {
      if (!appId || !userId) {
        throw new Error("AppID and UserID are required");
      }
      console.log("Initializing with appId:", appId, "userId:", userId);
      const result = this.wrapper.init(appId, userId);
      console.log("Native init result:", result);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      console.error("Native init error:", error);
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async uninit(): Promise<GMEResult> {
    try {
      this.stopPolling();
      const result = this.wrapper.uninit();
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async enterRoom(roomId: string, openId: string): Promise<GMEResult> {
    try {
      const result = this.wrapper.enterRoom(roomId, openId, 1); // 1 表示实时语音房间类型
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async exitRoom(): Promise<GMEResult> {
    try {
      const result = this.wrapper.exitRoom();
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async getDevicesList(): Promise<GMEVoiceResult> {
    try {
      const result = this.wrapper.getDevicesList(1); // 1 表示麦克风设备
      return {
        success: true,
        data: JSON.parse(result),
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async getCurrentDevice(deviceType: number): Promise<GMEVoiceResult> {
    try {
      const result = this.wrapper.getCurrentDevice(deviceType);
      return {
        success: true,
        data: result,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async setCurrentDevice(
    deviceType: number,
    deviceId: string
  ): Promise<GMEResult> {
    try {
      const result = this.wrapper.setCurrentDevice(deviceType, deviceId);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async enableMic(enabled: boolean): Promise<GMEResult> {
    try {
      const result = this.wrapper.enableMic(enabled);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async enableSpeaker(enabled: boolean): Promise<GMEResult> {
    try {
      const result = this.wrapper.enableSpeaker(enabled);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async getVolume(): Promise<GMEVoiceResult> {
    try {
      const result = this.wrapper.getVolume();
      return {
        success: true,
        data: result,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async setVolume(volume: number): Promise<GMEResult> {
    try {
      const result = this.wrapper.setVolume(volume);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async playEffect(
    effectId: number,
    filePath: string
  ): Promise<GMEResult> {
    try {
      const result = this.wrapper.playEffect(effectId, filePath);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async stopEffect(effectId: number): Promise<GMEResult> {
    try {
      const result = this.wrapper.stopEffect(effectId);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async stopAllEffects(): Promise<GMEResult> {
    try {
      const result = this.wrapper.stopAllEffects();
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async startRecordingWithTranslation(
    language: string
  ): Promise<GMEResult> {
    try {
      const result = this.wrapper.startRecordingWithTranslation(language);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async stopRecording(): Promise<GMEResult> {
    try {
      const result = this.wrapper.stopRecording();
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }

  public async setVoiceType(voiceType: number): Promise<GMEResult> {
    try {
      const result = this.wrapper.setVoiceType(voiceType);
      return {
        success: result === 0,
        error: result !== 0 ? result : undefined,
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }
}
