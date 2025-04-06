"use strict";
const electron = require("electron");
const node_os = require("node:os");
const node_path = require("node:path");
const node_child_process = require("node:child_process");
if (process.platform === "win32") {
  node_child_process.spawn("chcp", ["65001"], { shell: true });
}
electron.ipcMain.on("log:debug", (event, ...args) => {
  console.log("[Renderer Debug]", ...args);
});
electron.ipcMain.on("log:info", (event, ...args) => {
  console.log("[Renderer Info]", ...args);
});
electron.ipcMain.on("log:error", (event, ...args) => {
  console.error("[Renderer Error]", ...args);
});
const GMEWrapper = require("electron-gme-sdk");
if (node_os.release().startsWith("6.1")) electron.app.disableHardwareAcceleration();
process.env.DIST = node_path.join(__dirname, "../../renderer");
process.env.VITE_PUBLIC = electron.app.isPackaged ? process.env.DIST : node_path.join(process.env.DIST, "../public");
let win = null;
const preload = node_path.join(__dirname, "../preload/index.js");
const gme = new GMEWrapper();
async function createWindow() {
  win = new electron.BrowserWindow({
    width: 1024,
    height: 768,
    webPreferences: {
      preload,
      nodeIntegration: true,
      contextIsolation: true,
      sandbox: false,
      webSecurity: false
    }
  });
  if (!electron.app.isPackaged) {
    win.loadURL("http://localhost:5173");
    win.webContents.openDevTools();
  } else {
    win.loadFile(node_path.join(process.env.DIST, "index.html"));
  }
}
electron.app.whenReady().then(createWindow);
electron.app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    electron.app.quit();
  }
});
electron.app.on("activate", () => {
  if (electron.BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});
electron.ipcMain.handle("gme:init", async (event, appId, userId) => {
  try {
    const result = await gme.init(appId, userId);
    return result;
  } catch (error) {
    console.error("Init error:", error);
    return { success: false, error: error.message };
  }
});
electron.ipcMain.handle(
  "gme:enterRoom",
  async (event, roomId, openId) => {
    try {
      const result = await gme.enterRoom(roomId, openId);
      return result;
    } catch (error) {
      console.error("EnterRoom error:", error);
      return { success: false, error: error.message };
    }
  }
);
electron.ipcMain.handle("gme:exitRoom", async () => {
  try {
    const result = await gme.exitRoom();
    return result;
  } catch (error) {
    console.error("ExitRoom error:", error);
    return { success: false, error: error.message };
  }
});
electron.ipcMain.handle("gme:enableMic", async (event, enabled) => {
  try {
    const result = await gme.enableMic(enabled);
    return result;
  } catch (error) {
    console.error("EnableMic error:", error);
    return { success: false, error: error.message };
  }
});
electron.ipcMain.handle("gme:enableSpeaker", async (event, enabled) => {
  try {
    const result = await gme.enableSpeaker(enabled);
    return result;
  } catch (error) {
    console.error("EnableSpeaker error:", error);
    return { success: false, error: error.message };
  }
});
electron.ipcMain.handle("gme:getSpeakerList", async () => {
  try {
    const result = await gme.getSpeakerList();
    return result;
  } catch (error) {
    console.error("GetSpeakerList error:", error);
    return { success: false, error: error.message };
  }
});
electron.ipcMain.handle("gme:selectSpeakerDevice", async (event, deviceId) => {
  try {
    const result = await gme.selectSpeakerDevice(deviceId);
    return result;
  } catch (error) {
    console.error("SelectSpeakerDevice error:", error);
    return { success: false, error: error.message };
  }
});
electron.ipcMain.handle("gme:getCurrentSpeakerDevice", async () => {
  try {
    const result = await gme.getCurrentSpeakerDevice();
    return result;
  } catch (error) {
    console.error("GetCurrentSpeakerDevice error:", error);
    return { success: false, error: error.message };
  }
});
