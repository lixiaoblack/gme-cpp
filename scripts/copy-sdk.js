const fs = require("fs");
const path = require("path");

const sdkPath = path.join(__dirname, "..", "GME_Win_sdk_3.1.0.367");

// 如果 SDK 目录不存在，则创建它
if (!fs.existsSync(sdkPath)) {
  fs.mkdirSync(sdkPath, { recursive: true });
}

// 复制 SDK 文件
const copyDir = (src, dest) => {
  if (!fs.existsSync(src)) {
    console.error(`Source directory ${src} does not exist`);
    return;
  }

  if (!fs.existsSync(dest)) {
    fs.mkdirSync(dest, { recursive: true });
  }

  const entries = fs.readdirSync(src, { withFileTypes: true });

  for (const entry of entries) {
    const srcPath = path.join(src, entry.name);
    const destPath = path.join(dest, entry.name);

    if (entry.isDirectory()) {
      copyDir(srcPath, destPath);
    } else {
      fs.copyFileSync(srcPath, destPath);
    }
  }
};

// 复制 SDK 文件
const sdkSrcPath = path.join(__dirname, "..", "..", "GME_Win_sdk_3.1.0.367");
if (fs.existsSync(sdkSrcPath)) {
  copyDir(sdkSrcPath, sdkPath);
  console.log("SDK files copied successfully");
} else {
  console.warn(
    "SDK source directory not found. Please make sure the SDK files are available."
  );
}
