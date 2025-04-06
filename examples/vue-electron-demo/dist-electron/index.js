"use strict";
const electron = require("electron");
const node_os = require("node:os");
const node_path = require("node:path");
const require$$0$1 = require("fs");
const require$$0 = require("path");
var commonjsGlobal = typeof globalThis !== "undefined" ? globalThis : typeof window !== "undefined" ? window : typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : {};
var lib = {};
var types = {};
Object.defineProperty(types, "__esModule", { value: true });
var gme$1 = {};
function commonjsRequire(path) {
  throw new Error('Could not dynamically require "' + path + '". Please configure the dynamicRequireTargets or/and ignoreDynamicRequires option of @rollup/plugin-commonjs appropriately for this require call to work.');
}
var bindings = { exports: {} };
var sep = require$$0.sep || "/";
var fileUriToPath_1 = fileUriToPath;
function fileUriToPath(uri) {
  if ("string" != typeof uri || uri.length <= 7 || "file://" != uri.substring(0, 7)) {
    throw new TypeError("must pass in a file:// URI to convert to a file path");
  }
  var rest = decodeURI(uri.substring(7));
  var firstSlash = rest.indexOf("/");
  var host = rest.substring(0, firstSlash);
  var path = rest.substring(firstSlash + 1);
  if ("localhost" == host) host = "";
  if (host) {
    host = sep + sep + host;
  }
  path = path.replace(/^(.+)\|/, "$1:");
  if (sep == "\\") {
    path = path.replace(/\//g, "\\");
  }
  if (/^.+\:/.test(path)) ;
  else {
    path = sep + path;
  }
  return host + path;
}
(function(module, exports) {
  var fs = require$$0$1, path = require$$0, fileURLToPath = fileUriToPath_1, join = path.join, dirname = path.dirname, exists = fs.accessSync && function(path2) {
    try {
      fs.accessSync(path2);
    } catch (e) {
      return false;
    }
    return true;
  } || fs.existsSync || path.existsSync, defaults = {
    arrow: process.env.NODE_BINDINGS_ARROW || " → ",
    compiled: process.env.NODE_BINDINGS_COMPILED_DIR || "compiled",
    platform: process.platform,
    arch: process.arch,
    nodePreGyp: "node-v" + process.versions.modules + "-" + process.platform + "-" + process.arch,
    version: process.versions.node,
    bindings: "bindings.node",
    try: [
      // node-gyp's linked version in the "build" dir
      ["module_root", "build", "bindings"],
      // node-waf and gyp_addon (a.k.a node-gyp)
      ["module_root", "build", "Debug", "bindings"],
      ["module_root", "build", "Release", "bindings"],
      // Debug files, for development (legacy behavior, remove for node v0.9)
      ["module_root", "out", "Debug", "bindings"],
      ["module_root", "Debug", "bindings"],
      // Release files, but manually compiled (legacy behavior, remove for node v0.9)
      ["module_root", "out", "Release", "bindings"],
      ["module_root", "Release", "bindings"],
      // Legacy from node-waf, node <= 0.4.x
      ["module_root", "build", "default", "bindings"],
      // Production "Release" buildtype binary (meh...)
      ["module_root", "compiled", "version", "platform", "arch", "bindings"],
      // node-qbs builds
      ["module_root", "addon-build", "release", "install-root", "bindings"],
      ["module_root", "addon-build", "debug", "install-root", "bindings"],
      ["module_root", "addon-build", "default", "install-root", "bindings"],
      // node-pre-gyp path ./lib/binding/{node_abi}-{platform}-{arch}
      ["module_root", "lib", "binding", "nodePreGyp", "bindings"]
    ]
  };
  function bindings2(opts) {
    if (typeof opts == "string") {
      opts = { bindings: opts };
    } else if (!opts) {
      opts = {};
    }
    Object.keys(defaults).map(function(i2) {
      if (!(i2 in opts)) opts[i2] = defaults[i2];
    });
    if (!opts.module_root) {
      opts.module_root = exports.getRoot(exports.getFileName());
    }
    if (path.extname(opts.bindings) != ".node") {
      opts.bindings += ".node";
    }
    var requireFunc = typeof __webpack_require__ === "function" ? __non_webpack_require__ : commonjsRequire;
    var tries = [], i = 0, l = opts.try.length, n, b, err;
    for (; i < l; i++) {
      n = join.apply(
        null,
        opts.try[i].map(function(p) {
          return opts[p] || p;
        })
      );
      tries.push(n);
      try {
        b = opts.path ? requireFunc.resolve(n) : requireFunc(n);
        if (!opts.path) {
          b.path = n;
        }
        return b;
      } catch (e) {
        if (e.code !== "MODULE_NOT_FOUND" && e.code !== "QUALIFIED_PATH_RESOLUTION_FAILED" && !/not find/i.test(e.message)) {
          throw e;
        }
      }
    }
    err = new Error(
      "Could not locate the bindings file. Tried:\n" + tries.map(function(a) {
        return opts.arrow + a;
      }).join("\n")
    );
    err.tries = tries;
    throw err;
  }
  module.exports = exports = bindings2;
  exports.getFileName = function getFileName(calling_file) {
    var origPST = Error.prepareStackTrace, origSTL = Error.stackTraceLimit, dummy = {}, fileName;
    Error.stackTraceLimit = 10;
    Error.prepareStackTrace = function(e, st) {
      for (var i = 0, l = st.length; i < l; i++) {
        fileName = st[i].getFileName();
        if (fileName !== __filename) {
          if (calling_file) {
            if (fileName !== calling_file) {
              return;
            }
          } else {
            return;
          }
        }
      }
    };
    Error.captureStackTrace(dummy);
    dummy.stack;
    Error.prepareStackTrace = origPST;
    Error.stackTraceLimit = origSTL;
    var fileSchema = "file://";
    if (fileName.indexOf(fileSchema) === 0) {
      fileName = fileURLToPath(fileName);
    }
    return fileName;
  };
  exports.getRoot = function getRoot(file) {
    var dir = dirname(file), prev;
    while (true) {
      if (dir === ".") {
        dir = process.cwd();
      }
      if (exists(join(dir, "package.json")) || exists(join(dir, "node_modules"))) {
        return dir;
      }
      if (prev === dir) {
        throw new Error(
          'Could not find module root given file: "' + file + '". Do you have a `package.json` file? '
        );
      }
      prev = dir;
      dir = join(dir, "..");
    }
  };
})(bindings, bindings.exports);
var bindingsExports = bindings.exports;
Object.defineProperty(gme$1, "__esModule", { value: true });
gme$1.GME = void 0;
const native = bindingsExports("gme_native");
class GME {
  constructor(config) {
    this.wrapper = new native.GMEWrapper();
    this.startPolling();
  }
  startPolling() {
    this.pollInterval = setInterval(() => {
      this.poll();
    }, 100);
  }
  stopPolling() {
    if (this.pollInterval) {
      clearInterval(this.pollInterval);
      this.pollInterval = void 0;
    }
  }
  poll() {
    this.wrapper.poll();
  }
  async init() {
    try {
      const result = this.wrapper.init();
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async uninit() {
    try {
      this.stopPolling();
      const result = this.wrapper.uninit();
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async enterRoom(roomId, openId) {
    try {
      const result = this.wrapper.enterRoom(roomId, openId, 1);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async exitRoom() {
    try {
      const result = this.wrapper.exitRoom();
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async getDevicesList() {
    try {
      const result = this.wrapper.getDevicesList(1);
      return {
        success: true,
        data: JSON.parse(result)
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async getCurrentDevice(deviceType) {
    try {
      const result = this.wrapper.getCurrentDevice(deviceType);
      return {
        success: true,
        data: result
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async setCurrentDevice(deviceType, deviceId) {
    try {
      const result = this.wrapper.setCurrentDevice(deviceType, deviceId);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async enableMic(enabled) {
    try {
      const result = this.wrapper.enableMic(enabled);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async enableSpeaker(enabled) {
    try {
      const result = this.wrapper.enableSpeaker(enabled);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async getVolume() {
    try {
      const result = this.wrapper.getVolume();
      return {
        success: true,
        data: result
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async setVolume(volume) {
    try {
      const result = this.wrapper.setVolume(volume);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async playEffect(effectId, filePath) {
    try {
      const result = this.wrapper.playEffect(effectId, filePath);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async stopEffect(effectId) {
    try {
      const result = this.wrapper.stopEffect(effectId);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async stopAllEffects() {
    try {
      const result = this.wrapper.stopAllEffects();
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async startRecordingWithTranslation(language) {
    try {
      const result = this.wrapper.startRecordingWithTranslation(language);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async stopRecording() {
    try {
      const result = this.wrapper.stopRecording();
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
  async setVoiceType(voiceType) {
    try {
      const result = this.wrapper.setVoiceType(voiceType);
      return {
        success: result === 0,
        error: result !== 0 ? result : void 0
      };
    } catch (error) {
      return {
        success: false,
        error: error instanceof Error ? error.message : String(error)
      };
    }
  }
}
gme$1.GME = GME;
(function(exports) {
  var __createBinding = commonjsGlobal && commonjsGlobal.__createBinding || (Object.create ? function(o, m, k, k2) {
    if (k2 === void 0) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() {
        return m[k];
      } };
    }
    Object.defineProperty(o, k2, desc);
  } : function(o, m, k, k2) {
    if (k2 === void 0) k2 = k;
    o[k2] = m[k];
  });
  var __exportStar = commonjsGlobal && commonjsGlobal.__exportStar || function(m, exports2) {
    for (var p in m) if (p !== "default" && !Object.prototype.hasOwnProperty.call(exports2, p)) __createBinding(exports2, m, p);
  };
  Object.defineProperty(exports, "__esModule", { value: true });
  __exportStar(types, exports);
  __exportStar(gme$1, exports);
  const gme_1 = gme$1;
  exports.default = gme_1.GME;
})(lib);
if (node_os.release().startsWith("6.1")) electron.app.disableHardwareAcceleration();
process.env.DIST = node_path.join(__dirname, "../../renderer");
process.env.VITE_PUBLIC = electron.app.isPackaged ? process.env.DIST : node_path.join(process.env.DIST, "../public");
let win = null;
const preload = node_path.join(__dirname, "../preload/index.js");
const gme = new lib.GME({
  appId: "1400000000"
  // 替换为你的实际 AppID
});
async function createWindow() {
  win = new electron.BrowserWindow({
    width: 1024,
    height: 768,
    webPreferences: {
      preload,
      nodeIntegration: true,
      contextIsolation: false
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
  return await gme.init();
});
electron.ipcMain.handle(
  "gme:enterRoom",
  async (event, roomId, openId) => {
    return await gme.enterRoom(roomId, openId);
  }
);
electron.ipcMain.handle("gme:exitRoom", async () => {
  return await gme.exitRoom();
});
electron.ipcMain.handle("gme:enableMic", async (event, enabled) => {
  return await gme.enableMic(enabled);
});
electron.ipcMain.handle("gme:enableSpeaker", async (event, enabled) => {
  return await gme.enableSpeaker(enabled);
});
