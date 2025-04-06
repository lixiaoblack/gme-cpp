"use strict";
const electron = require("electron");
const log = {
  debug: (...args) => electron.ipcRenderer.send("log:debug", ...args),
  info: (...args) => electron.ipcRenderer.send("log:info", ...args),
  error: (...args) => electron.ipcRenderer.send("log:error", ...args)
};
electron.contextBridge.exposeInMainWorld("gmeApi", {
  init: async (appId, userId) => {
    const result = await electron.ipcRenderer.invoke("gme:init", appId, userId);
    log.info("init result:", result);
    return result;
  },
  enterRoom: async (roomId, openId) => {
    const result = await electron.ipcRenderer.invoke("gme:enterRoom", roomId, openId);
    log.info("enterRoom result:", result);
    return result;
  },
  exitRoom: async () => {
    const result = await electron.ipcRenderer.invoke("gme:exitRoom");
    log.info("exitRoom result:", result);
    return result;
  },
  enableMic: async (enabled) => {
    const result = await electron.ipcRenderer.invoke("gme:enableMic", enabled);
    log.info("enableMic result:", result);
    return result;
  },
  enableSpeaker: async (enabled) => {
    const result = await electron.ipcRenderer.invoke("gme:enableSpeaker", enabled);
    log.info("enableSpeaker result:", result);
    return result;
  },
  getSpeakerList: async () => {
    const result = await electron.ipcRenderer.invoke("gme:getSpeakerList");
    log.info("getSpeakerList result:", result);
    return result;
  },
  selectSpeakerDevice: async (deviceId) => {
    const result = await electron.ipcRenderer.invoke(
      "gme:selectSpeakerDevice",
      deviceId
    );
    log.info("selectSpeakerDevice result:", result);
    return result;
  },
  getCurrentSpeakerDevice: async () => {
    const result = await electron.ipcRenderer.invoke("gme:getCurrentSpeakerDevice");
    log.info("getCurrentSpeakerDevice result:", result);
    return result;
  }
});
electron.contextBridge.exposeInMainWorld("log", log);
