import { existsSync, mkdirSync, readdirSync, copyFileSync } from "fs";
import { join, dirname } from "path";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const sdkPath = join(__dirname, "..", "GME_Win_sdk_3.1.0.367");

// 如果 SDK 目录不存在，则创建它
if (!existsSync(sdkPath)) {
  mkdirSync(sdkPath, { recursive: true });
}

// 复制 SDK 文件
const copyDir = (src, dest) => {
  if (!existsSync(src)) {
    console.error(`Source directory ${src} does not exist`);
    return;
  }

  if (!existsSync(dest)) {
    mkdirSync(dest, { recursive: true });
  }

  const entries = readdirSync(src, { withFileTypes: true });

  for (const entry of entries) {
    const srcPath = join(src, entry.name);
    const destPath = join(dest, entry.name);

    if (entry.isDirectory()) {
      copyDir(srcPath, destPath);
    } else {
      copyFileSync(srcPath, destPath);
    }
  }
};

// 复制 SDK 文件
const sdkSrcPath = join(__dirname, "..", "..", "GME_Win_sdk_3.1.0.367");
if (existsSync(sdkSrcPath)) {
  copyDir(sdkSrcPath, sdkPath);
  console.log("SDK files copied successfully");
} else {
  console.warn(
    "SDK source directory not found. Please make sure the SDK files are available."
  );
}
