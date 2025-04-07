/*
 * @Author: wanglx
 * @Date: 2025-04-06 09:31:09
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-07 21:27:06
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";
import electron from "vite-plugin-electron";
import renderer from "vite-plugin-electron-renderer";
import { resolve } from "path";

export default defineConfig({
  base: "./",
  build: {
    outDir: "dist/renderer",
    emptyOutDir: true,
    rollupOptions: {
      external: [/\.node$/],
    },
  },
  server: {
    port: 5173,
    watch: {
      usePolling: true,
    },
    hmr: {
      overlay: false,
    },
  },
  plugins: [
    vue(),
    electron([
      {
        entry: "electron/main/index.ts",
        vite: {
          build: {
            outDir: "dist/electron/main",
            rollupOptions: {
              external: ["electron", /\.node$/],
            },
          },
          server: {
            watch: {
              usePolling: true,
            },
          },
        },
      },
    ]),
    renderer(),
  ],
  resolve: {
    alias: {
      "@": resolve(__dirname, "src"),
    },
  },
});
