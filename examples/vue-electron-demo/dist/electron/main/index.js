"use strict";
const electron = require("electron");
const node_os = require("node:os");
const node_path = require("node:path");
const node_child_process = require("node:child_process");
if (process.platform === "win32") {
  node_child_process.spawn("chcp", ["65001"], { shell: true });
}
if (node_os.release().startsWith("6.1")) electron.app.disableHardwareAcceleration();
process.env.DIST = node_path.join(__dirname, "../../renderer");
process.env.VITE_PUBLIC = electron.app.isPackaged ? process.env.DIST : node_path.join(process.env.DIST, "../public");
let win = null;
async function createWindow() {
  win = new electron.BrowserWindow({
    width: 1024,
    height: 768,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
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
