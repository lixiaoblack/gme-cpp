# electron-gme-sdk

这是一个用于在 Electron 应用中集成腾讯 GME SDK 的 npm 包。

## 安装

```bash
npm install electron-gme-sdk
# 或
yarn add electron-gme-sdk
```

## 使用方法

```typescript
import { app } from 'electron';
import * as path from 'path';
import GME from 'electron-gme-sdk';

// 创建 GME 实例
const gme = new GME({
  sdkPath: path.join(__dirname, 'path/to/gmesdk.dll'),
  appId: 'your-app-id-here'
});

async function main() {
  try {
    // 初始化 GME
    const initResult = await gme.init();
    if (!initResult.success) {
      console.error('GME 初始化失败:', initResult.error);
      return;
    }
    console.log('GME 初始化成功');

    // 在这里添加你的业务逻辑
    // ...

    // 程序退出时反初始化
    app.on('before-quit', async () => {
      const uninitResult = await gme.uninit();
      if (!uninitResult.success) {
        console.error('GME 反初始化失败:', uninitResult.error);
      }
      console.log('GME 反初始化成功');
    });

  } catch (error) {
    console.error('发生错误:', error);
  }
}

app.whenReady().then(main);
```

## API 文档

### GME 类

#### 构造函数

```typescript
new GME(config: GMEConfig)
```

配置参数：
- `sdkPath`: GME SDK DLL 文件的路径
- `appId`: 腾讯云 GME 应用 ID

#### 方法

- `init(): Promise<GMEResult>` - 初始化 GME SDK
- `uninit(): Promise<GMEResult>` - 反初始化 GME SDK
- `poll(): void` - GME 轮询函数（内部自动调用，通常不需要手动调用）

### 类型定义

```typescript
interface GMEConfig {
  sdkPath: string;
  appId: string;
}

interface GMEResult {
  success: boolean;
  error?: number | string;
}
```

## 注意事项

1. 确保 GME SDK DLL 文件路径正确
2. 使用前需要有有效的腾讯云 GME AppID
3. 该包依赖于 `ffi-napi` 和 `ref-napi`，确保你的 Node.js 版本兼容
4. 目前仅支持 Windows 平台

## 开发

```bash
# 安装依赖
npm install

# 构建
npm run build

# 运行示例
cd examples
ts-node basic-usage.ts
```

## 许可证

ISC 