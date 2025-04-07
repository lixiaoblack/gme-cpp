/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:31:42
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-07 20:56:05
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
import { app, BrowserWindow, ipcMain } from "electron";
import { release } from "node:os";
import { join } from "node:path";
import { spawn } from "node:child_process";

// 设置控制台编码为 UTF-8
if (process.platform === "win32") {
  spawn("chcp", ["65001"], { shell: true });
}

// 添加日志处理
ipcMain.on("log:debug", (event, ...args) => {
  console.log("[Renderer Debug]", ...args);
});

ipcMain.on("log:info", (event, ...args) => {
  console.log("[Renderer Info]", ...args);
});

ipcMain.on("log:error", (event, ...args) => {
  console.error("[Renderer Error]", ...args);
});

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
      webSecurity: false,
    },
  });

  if (!app.isPackaged) {
    win.loadURL("http://localhost:5173");
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
ipcMain.handle("gme:init", async (event, appId: string, userId: string) => {
  try {
    const result = await gme.init(appId, userId);
    return result;
  } catch (error) {
    console.error("Init error:", error);
    return { success: false, error: error.message };
  }
});

ipcMain.handle(
  "gme:enterRoom",
  async (event, roomId: string, openId: string) => {
    try {
      const result = await gme.enterRoom(roomId, openId);
      return result;
    } catch (error) {
      console.error("EnterRoom error:", error);
      return { success: false, error: error.message };
    }
  }
);

ipcMain.handle("gme:exitRoom", async () => {
  try {
    const result = await gme.exitRoom();
    return result;
  } catch (error) {
    console.error("ExitRoom error:", error);
    return { success: false, error: error.message };
  }
});

ipcMain.handle("gme:enableMic", async (event, enabled: boolean) => {
  try {
    const result = await gme.enableMic(enabled);
    return result;
  } catch (error) {
    console.error("EnableMic error:", error);
    return { success: false, error: error.message };
  }
});

ipcMain.handle("gme:enableSpeaker", async (event, enabled: boolean) => {
  try {
    const result = await gme.enableSpeaker(enabled);
    return result;
  } catch (error) {
    console.error("EnableSpeaker error:", error);
    return { success: false, error: error.message };
  }
});

ipcMain.handle("gme:getSpeakerList", async () => {
  try {
    const result = await gme.getSpeakerList();
    return result;
  } catch (error) {
    console.error("GetSpeakerList error:", error);
    return { success: false, error: error.message };
  }
});

ipcMain.handle("gme:selectSpeakerDevice", async (event, deviceId: string) => {
  try {
    const result = await gme.selectSpeakerDevice(deviceId);
    return result;
  } catch (error) {
    console.error("SelectSpeakerDevice error:", error);
    return { success: false, error: error.message };
  }
});

ipcMain.handle("gme:getCurrentSpeakerDevice", async () => {
  try {
    const result = await gme.getCurrentSpeakerDevice();
    return result;
  } catch (error) {
    console.error("GetCurrentSpeakerDevice error:", error);
    return { success: false, error: error.message };
  }
});
