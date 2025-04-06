"use strict";
const electron = require("electron");
const node_os = require("node:os");
const node_path = require("node:path");
const GMEWrapper = require("electron-gme-sdk");
if (node_os.release().startsWith("6.1")) electron.app.disableHardwareAcceleration();
const GME_CONFIG = {
  appId: "1600074451",
  userId: "100560"
};
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
      sandbox: false
    }
  });
  if (!electron.app.isPackaged) {
    win.loadFile(node_path.join(process.env.DIST, "index.html"));
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
electron.ipcMain.handle("gme:init", async () => {
  try {
    console.log("Initializing GME with fixed config:", GME_CONFIG);
    const result = await gme.init(GME_CONFIG.appId, GME_CONFIG.userId);
    console.log("GME init result:", result);
    return result;
  } catch (error) {
    console.error("GME init error:", error);
    return {
      success: false,
      error: error instanceof Error ? error.message : String(error)
    };
  }
});
electron.ipcMain.handle(
  "gme:enterRoom",
  async (event, roomId, openId) => {
    try {
      console.log("Entering room with roomId:", roomId, "openId:", openId);
      const result = await gme.enterRoom(String(roomId), String(openId));
      console.log("Enter room result:", result);
      return result;
    } catch (error) {
      console.error("Enter room error:", error);
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
);
electron.ipcMain.handle("gme:exitRoom", async () => {
  try {
    console.log("Exiting room");
    const result = await gme.exitRoom();
    console.log("Exit room result:", result);
    return result;
  } catch (error) {
    console.error("Exit room error:", error);
    return {
      success: false,
      error: error instanceof Error ? error.message : String(error)
    };
  }
});
electron.ipcMain.handle("gme:enableMic", async (event, enabled) => {
  try {
    console.log("Setting mic enabled:", enabled);
    const result = await gme.enableMic(enabled);
    console.log("Enable mic result:", result);
    return result;
  } catch (error) {
    console.error("Enable mic error:", error);
    return {
      success: false,
      error: error instanceof Error ? error.message : String(error)
    };
  }
});
electron.ipcMain.handle("gme:enableSpeaker", async (event, enabled) => {
  try {
    console.log("Setting speaker enabled:", enabled);
    const result = await gme.enableSpeaker(enabled);
    console.log("Enable speaker result:", result);
    return result;
  } catch (error) {
    console.error("Enable speaker error:", error);
    return {
      success: false,
      error: error instanceof Error ? error.message : String(error)
    };
  }
});
