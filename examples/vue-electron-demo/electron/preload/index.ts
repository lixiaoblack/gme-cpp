/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:32:13
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-06 09:58:32
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
import { contextBridge, ipcRenderer } from "electron";

// 添加日志功能
const log = {
  debug: (...args: any[]) => ipcRenderer.send("log:debug", ...args),
  info: (...args: any[]) => ipcRenderer.send("log:info", ...args),
  error: (...args: any[]) => ipcRenderer.send("log:error", ...args),
};

// 暴露安全的 API 到渲染进程
contextBridge.exposeInMainWorld("gmeApi", {
  init: async (appId: string, userId: string) => {
    const result = await ipcRenderer.invoke("gme:init", appId, userId);
    log.info("init result:", result);
    return result;
  },
  enterRoom: async (roomId: string, openId: string) => {
    const result = await ipcRenderer.invoke("gme:enterRoom", roomId, openId);
    log.info("enterRoom result:", result);
    return result;
  },
  exitRoom: async () => {
    const result = await ipcRenderer.invoke("gme:exitRoom");
    log.info("exitRoom result:", result);
    return result;
  },
  enableMic: async (enabled: boolean) => {
    const result = await ipcRenderer.invoke("gme:enableMic", enabled);
    log.info("enableMic result:", result);
    return result;
  },
  enableSpeaker: async (enabled: boolean) => {
    const result = await ipcRenderer.invoke("gme:enableSpeaker", enabled);
    log.info("enableSpeaker result:", result);
    return result;
  },
  getSpeakerList: async () => {
    const result = await ipcRenderer.invoke("gme:getSpeakerList");
    log.info("getSpeakerList result:", result);
    return result;
  },
  selectSpeakerDevice: async (deviceId: string) => {
    const result = await ipcRenderer.invoke(
      "gme:selectSpeakerDevice",
      deviceId
    );
    log.info("selectSpeakerDevice result:", result);
    return result;
  },
  getCurrentSpeakerDevice: async () => {
    const result = await ipcRenderer.invoke("gme:getCurrentSpeakerDevice");
    log.info("getCurrentSpeakerDevice result:", result);
    return result;
  },
});

// 暴露日志功能到渲染进程
contextBridge.exposeInMainWorld("log", log);
