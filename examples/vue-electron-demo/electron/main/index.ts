/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:31:42
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-07 21:15:41
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
import { app, BrowserWindow } from "electron";
import { release } from "node:os";
import { join } from "node:path";
import { spawn } from "node:child_process";

// 设置控制台编码为 UTF-8
if (process.platform === "win32") {
  spawn("chcp", ["65001"], { shell: true });
}

// 禁用 Windows 7 的 GPU 加速
if (release().startsWith("6.1")) app.disableHardwareAcceleration();

// 设置应用程序路径
process.env.DIST = join(__dirname, "../../renderer");
process.env.VITE_PUBLIC = app.isPackaged
  ? process.env.DIST
  : join(process.env.DIST, "../public");

let win: BrowserWindow | null = null;

async function createWindow() {
  win = new BrowserWindow({
    width: 1024,
    height: 768,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
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
