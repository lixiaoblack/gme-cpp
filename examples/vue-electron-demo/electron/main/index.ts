/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:31:42
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-06 10:07:20
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
import { app, BrowserWindow, ipcMain } from "electron";
import { release } from "node:os";
import { join } from "node:path";

// 使用 require 导入 GME SDK
const GMEWrapper = require("electron-gme-sdk");

// 禁用 Windows 7 的 GPU 加速
if (release().startsWith("6.1")) app.disableHardwareAcceleration();

// GME 配置
const GME_CONFIG = {
  appId: "1600074451",
  userId: "100560",
};

// 设置应用程序路径
process.env.DIST = join(__dirname, "../../renderer");
process.env.VITE_PUBLIC = app.isPackaged
  ? process.env.DIST
  : join(process.env.DIST, "../public");

let win: BrowserWindow | null = null;
const preload = join(__dirname, "../preload/index.js");

// 初始化 GME 实例
const gme = new GMEWrapper();

async function createWindow() {
  win = new BrowserWindow({
    width: 1024,
    height: 768,
    webPreferences: {
      preload,
      nodeIntegration: true,
      contextIsolation: true,
      sandbox: false,
    },
  });

  if (!app.isPackaged) {
    win.loadFile(join(process.env.DIST, "index.html"));
    win.webContents.openDevTools();
  } else {
    win.loadFile(join(process.env.DIST, "index.html"));
  }
}

app.whenReady().then(createWindow);

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

app.on("activate", () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

// GME 相关的 IPC 通信
ipcMain.handle("gme:init", async () => {
  try {
    console.log("Initializing GME with fixed config:", GME_CONFIG);
    const result = await gme.init(GME_CONFIG.appId, GME_CONFIG.userId);
    console.log("GME init result:", result);
    return result;
  } catch (error) {
    console.error("GME init error:", error);
    return {
      success: false,
      error: error instanceof Error ? error.message : String(error),
    };
  }
});

ipcMain.handle(
  "gme:enterRoom",
  async (event, roomId: string, openId: string) => {
    try {
      console.log("Entering room with roomId:", roomId, "openId:", openId);
      const result = await gme.enterRoom(String(roomId), String(openId));
      console.log("Enter room result:", result);
      return result;
    } catch (error) {
      console.error("Enter room error:", error);
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error),
      };
    }
  }
);

ipcMain.handle("gme:exitRoom", async () => {
  try {
    console.log("Exiting room");
    const result = await gme.exitRoom();
    console.log("Exit room result:", result);
    return result;
  } catch (error) {
    console.error("Exit room error:", error);
    return {
      success: false,
      error: error instanceof Error ? error.message : String(error),
    };
  }
});

ipcMain.handle("gme:enableMic", async (event, enabled: boolean) => {
  try {
    console.log("Setting mic enabled:", enabled);
    const result = await gme.enableMic(enabled);
    console.log("Enable mic result:", result);
    return result;
  } catch (error) {
    console.error("Enable mic error:", error);
    return {
      success: false,
      error: error instanceof Error ? error.message : String(error),
    };
  }
});

ipcMain.handle("gme:enableSpeaker", async (event, enabled: boolean) => {
  try {
    console.log("Setting speaker enabled:", enabled);
    const result = await gme.enableSpeaker(enabled);
    console.log("Enable speaker result:", result);
    return result;
  } catch (error) {
    console.error("Enable speaker error:", error);
    return {
      success: false,
      error: error instanceof Error ? error.message : String(error),
    };
  }
});
