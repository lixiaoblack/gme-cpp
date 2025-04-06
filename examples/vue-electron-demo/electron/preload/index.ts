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

// 暴露安全的 API 到渲染进程
contextBridge.exposeInMainWorld("gmeApi", {
  init: async () => {
    return await ipcRenderer.invoke("gme:init");
  },
  enterRoom: async (roomId: string, openId: string) => {
    return await ipcRenderer.invoke("gme:enterRoom", roomId, openId);
  },
  exitRoom: async () => {
    return await ipcRenderer.invoke("gme:exitRoom");
  },
  enableMic: async (enabled: boolean) => {
    return await ipcRenderer.invoke("gme:enableMic", enabled);
  },
  enableSpeaker: async (enabled: boolean) => {
    return await ipcRenderer.invoke("gme:enableSpeaker", enabled);
  },
});
