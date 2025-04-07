import { join } from "path";
// 在 Electron 环境中加载原生模块
let native;
try {
    // 尝试从 node_modules 中加载
    const modulePath = join(process.cwd(), "node_modules", "electron-gme-sdk", "build", "Release", "gme_native");
    console.log("Trying to load native module from:", modulePath);
    native = require(modulePath);
}
catch (error) {
    console.error("Failed to load from node_modules, trying relative path:", error);
    // 如果失败，尝试从相对路径加载
    const relativePath = join(__dirname, "..", "build", "Release", "gme_native");
    console.log("Trying relative path:", relativePath);
    native = require(relativePath);
}
export class GME {
    constructor(config) {
        this.wrapper = new native.GMEWrapper();
        this.appId = config.appId;
        this.startPolling();
    }
    startPolling() {
        this.pollInterval = setInterval(() => {
            this.poll();
        }, 100);
    }
    stopPolling() {
        if (this.pollInterval) {
            clearInterval(this.pollInterval);
            this.pollInterval = undefined;
        }
    }
    poll() {
        this.wrapper.poll();
    }
    async init(appId, userId) {
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
        }
        catch (error) {
            console.error("Native init error:", error);
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async uninit() {
        try {
            this.stopPolling();
            const result = this.wrapper.uninit();
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async enterRoom(roomId, openId) {
        try {
            const result = this.wrapper.enterRoom(roomId, openId, 1); // 1 表示实时语音房间类型
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async exitRoom() {
        try {
            const result = this.wrapper.exitRoom();
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async getDevicesList() {
        try {
            const result = this.wrapper.getDevicesList(1); // 1 表示麦克风设备
            return {
                success: true,
                data: JSON.parse(result),
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async getCurrentDevice(deviceType) {
        try {
            const result = this.wrapper.getCurrentDevice(deviceType);
            return {
                success: true,
                data: result,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async setCurrentDevice(deviceType, deviceId) {
        try {
            const result = this.wrapper.setCurrentDevice(deviceType, deviceId);
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async enableMic(enabled) {
        try {
            const result = this.wrapper.enableMic(enabled);
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async enableSpeaker(enabled) {
        try {
            const result = this.wrapper.enableSpeaker(enabled);
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async getVolume() {
        try {
            const result = this.wrapper.getVolume();
            return {
                success: true,
                data: result,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async setVolume(volume) {
        try {
            const result = this.wrapper.setVolume(volume);
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async playEffect(effectId, filePath) {
        try {
            const result = this.wrapper.playEffect(effectId, filePath);
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async stopEffect(effectId) {
        try {
            const result = this.wrapper.stopEffect(effectId);
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async stopAllEffects() {
        try {
            const result = this.wrapper.stopAllEffects();
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async startRecordingWithTranslation(language) {
        try {
            const result = this.wrapper.startRecordingWithTranslation(language);
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async stopRecording() {
        try {
            const result = this.wrapper.stopRecording();
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
    async setVoiceType(voiceType) {
        try {
            const result = this.wrapper.setVoiceType(voiceType);
            return {
                success: result === 0,
                error: result !== 0 ? result : undefined,
            };
        }
        catch (error) {
            return {
                success: false,
                error: error instanceof Error ? error.message : String(error),
            };
        }
    }
}
//# sourceMappingURL=gme.js.map