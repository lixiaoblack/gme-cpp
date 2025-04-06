var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g = Object.create((typeof Iterator === "function" ? Iterator : Object).prototype);
    return g.next = verb(0), g["throw"] = verb(1), g["return"] = verb(2), typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (g && (g = 0, op[0] && (_ = 0)), _) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
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
var GMEWrapper = require("electron-gme-sdk");
// 禁用 Windows 7 的 GPU 加速
if (release().startsWith("6.1"))
    app.disableHardwareAcceleration();
// GME 配置
var GME_CONFIG = {
    appId: "1600074451",
    userId: "100560",
};
// 设置应用程序路径
process.env.DIST = join(__dirname, "../../renderer");
process.env.VITE_PUBLIC = app.isPackaged
    ? process.env.DIST
    : join(process.env.DIST, "../public");
var win = null;
var preload = join(__dirname, "../preload/index.js");
// 初始化 GME 实例
var gme = new GMEWrapper();
function createWindow() {
    return __awaiter(this, void 0, void 0, function () {
        return __generator(this, function (_a) {
            win = new BrowserWindow({
                width: 1024,
                height: 768,
                webPreferences: {
                    preload: preload,
                    nodeIntegration: true,
                    contextIsolation: true,
                    sandbox: false,
                },
            });
            if (!app.isPackaged) {
                win.loadFile(join(process.env.DIST, "index.html"));
                win.webContents.openDevTools();
            }
            else {
                win.loadFile(join(process.env.DIST, "index.html"));
            }
            return [2 /*return*/];
        });
    });
}
app.whenReady().then(createWindow);
app.on("window-all-closed", function () {
    if (process.platform !== "darwin") {
        app.quit();
    }
});
app.on("activate", function () {
    if (BrowserWindow.getAllWindows().length === 0) {
        createWindow();
    }
});
// GME 相关的 IPC 通信
ipcMain.handle("gme:init", function () { return __awaiter(void 0, void 0, void 0, function () {
    var result, error_1;
    return __generator(this, function (_a) {
        switch (_a.label) {
            case 0:
                _a.trys.push([0, 2, , 3]);
                console.log("Initializing GME with fixed config:", GME_CONFIG);
                return [4 /*yield*/, gme.init(GME_CONFIG.appId, GME_CONFIG.userId)];
            case 1:
                result = _a.sent();
                console.log("GME init result:", result);
                return [2 /*return*/, result];
            case 2:
                error_1 = _a.sent();
                console.error("GME init error:", error_1);
                return [2 /*return*/, {
                        success: false,
                        error: error_1 instanceof Error ? error_1.message : String(error_1),
                    }];
            case 3: return [2 /*return*/];
        }
    });
}); });
ipcMain.handle("gme:enterRoom", function (event, roomId, openId) { return __awaiter(void 0, void 0, void 0, function () {
    var result, error_2;
    return __generator(this, function (_a) {
        switch (_a.label) {
            case 0:
                _a.trys.push([0, 2, , 3]);
                console.log("Entering room with roomId:", roomId, "openId:", openId);
                return [4 /*yield*/, gme.enterRoom(String(roomId), String(openId))];
            case 1:
                result = _a.sent();
                console.log("Enter room result:", result);
                return [2 /*return*/, result];
            case 2:
                error_2 = _a.sent();
                console.error("Enter room error:", error_2);
                return [2 /*return*/, {
                        success: false,
                        error: error_2 instanceof Error ? error_2.message : String(error_2),
                    }];
            case 3: return [2 /*return*/];
        }
    });
}); });
ipcMain.handle("gme:exitRoom", function () { return __awaiter(void 0, void 0, void 0, function () {
    var result, error_3;
    return __generator(this, function (_a) {
        switch (_a.label) {
            case 0:
                _a.trys.push([0, 2, , 3]);
                console.log("Exiting room");
                return [4 /*yield*/, gme.exitRoom()];
            case 1:
                result = _a.sent();
                console.log("Exit room result:", result);
                return [2 /*return*/, result];
            case 2:
                error_3 = _a.sent();
                console.error("Exit room error:", error_3);
                return [2 /*return*/, {
                        success: false,
                        error: error_3 instanceof Error ? error_3.message : String(error_3),
                    }];
            case 3: return [2 /*return*/];
        }
    });
}); });
ipcMain.handle("gme:enableMic", function (event, enabled) { return __awaiter(void 0, void 0, void 0, function () {
    var result, error_4;
    return __generator(this, function (_a) {
        switch (_a.label) {
            case 0:
                _a.trys.push([0, 2, , 3]);
                console.log("Setting mic enabled:", enabled);
                return [4 /*yield*/, gme.enableMic(enabled)];
            case 1:
                result = _a.sent();
                console.log("Enable mic result:", result);
                return [2 /*return*/, result];
            case 2:
                error_4 = _a.sent();
                console.error("Enable mic error:", error_4);
                return [2 /*return*/, {
                        success: false,
                        error: error_4 instanceof Error ? error_4.message : String(error_4),
                    }];
            case 3: return [2 /*return*/];
        }
    });
}); });
ipcMain.handle("gme:enableSpeaker", function (event, enabled) { return __awaiter(void 0, void 0, void 0, function () {
    var result, error_5;
    return __generator(this, function (_a) {
        switch (_a.label) {
            case 0:
                _a.trys.push([0, 2, , 3]);
                console.log("Setting speaker enabled:", enabled);
                return [4 /*yield*/, gme.enableSpeaker(enabled)];
            case 1:
                result = _a.sent();
                console.log("Enable speaker result:", result);
                return [2 /*return*/, result];
            case 2:
                error_5 = _a.sent();
                console.error("Enable speaker error:", error_5);
                return [2 /*return*/, {
                        success: false,
                        error: error_5 instanceof Error ? error_5.message : String(error_5),
                    }];
            case 3: return [2 /*return*/];
        }
    });
}); });
