"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __exportStar = (this && this.__exportStar) || function(m, exports) {
    for (var p in m) if (p !== "default" && !Object.prototype.hasOwnProperty.call(exports, p)) __createBinding(exports, m, p);
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.GME = void 0;
/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:05:05
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-06 22:27:19
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
var gme_1 = require("./gme");
Object.defineProperty(exports, "GME", { enumerable: true, get: function () { return gme_1.GME; } });
__exportStar(require("./types"), exports);
const addon = require("bindings")("gme_native");
// 导出 GMEWrapper 类
module.exports = addon.GMEWrapper;
//# sourceMappingURL=index.js.map