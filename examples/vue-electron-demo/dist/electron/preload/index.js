"use strict";
const electron = require("electron");
electron.contextBridge.exposeInMainWorld("gmeApi", {
  init: async () => {
    return await electron.ipcRenderer.invoke("gme:init");
  },
  enterRoom: async (roomId, openId) => {
    return await electron.ipcRenderer.invoke("gme:enterRoom", roomId, openId);
  },
  exitRoom: async () => {
    return await electron.ipcRenderer.invoke("gme:exitRoom");
  },
  enableMic: async (enabled) => {
    return await electron.ipcRenderer.invoke("gme:enableMic", enabled);
  },
  enableSpeaker: async (enabled) => {
    return await electron.ipcRenderer.invoke("gme:enableSpeaker", enabled);
  }
});
