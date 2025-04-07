/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:05:05
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-07 22:39:46
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
export { GME } from "./gme.js";
export * from "./types.js";

import { join } from "path";

interface NativeModule {
  GMEWrapper: new () => any;
}

// 在 Electron 环境中加载原生模块
let native: NativeModule;
try {
  // 尝试从 node_modules 中加载
  const modulePath = join(
    process.cwd(),
    "node_modules",
    "electron-gme-sdk",
    "build",
    "Release",
    "gme_native"
  );
  console.log("Trying to load native module from:", modulePath);
  native = require(modulePath);
} catch (error) {
  console.error(
    "Failed to load from node_modules, trying relative path:",
    error
  );
  // 如果失败，尝试从相对路径加载
  const relativePath = join(__dirname, "..", "build", "Release", "gme_native");
  console.log("Trying relative path:", relativePath);
  native = require(relativePath);
}

// 导出 GMEWrapper 类
export default native.GMEWrapper;
