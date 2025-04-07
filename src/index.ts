/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:05:05
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-06 22:27:19
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
export { GME } from "./gme.js";
export * from "./types.js";

// ESM兼容的导入和导出
import _bindings from "bindings";
const bindings = _bindings;

const GMEWrapper = bindings("gme_native");

export { GMEWrapper };
export default GMEWrapper;
