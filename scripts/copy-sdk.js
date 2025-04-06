const fs = require("fs-extra");
const path = require("path");

const sdkSourceDir = path.join(__dirname, "../GME_Win_sdk_3.1.0.367");
const sdkDestDir = path.join(__dirname, "../sdk");

// 复制 SDK 文件到 sdk 目录
fs.copySync(sdkSourceDir, sdkDestDir, {
  filter: (src) => {
    // 只复制需要的文件和目录
    return (
      fs.statSync(src).isDirectory() ||
      src.endsWith(".dll") ||
      src.endsWith(".lib") ||
      src.endsWith(".h")
    );
  },
});
