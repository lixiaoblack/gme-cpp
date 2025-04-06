/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:05:05
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-06 09:33:05
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
export { GME } from "./gme";
export * from "./types";

const addon = require("bindings")("gme_native");

// 导出 GMEWrapper 类
module.exports = addon.GMEWrapper;
