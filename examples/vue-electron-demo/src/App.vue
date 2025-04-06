<template>
  <div class="container">
    <h1>GME SDK Demo</h1>
    <el-form :model="form" label-width="120px">
      <el-form-item label="房间ID">
        <el-input v-model="form.roomId" />
      </el-form-item>
      <el-form-item label="用户ID">
        <el-input v-model="form.openId" />
      </el-form-item>
      <el-form-item>
        <el-button type="primary" @click="handleInit">初始化</el-button>
        <el-button
          type="success"
          @click="handleEnterRoom"
          :disabled="!initialized || !form.roomId"
          >进入房间</el-button
        >
        <el-button type="danger" @click="handleExitRoom" :disabled="!inRoom"
          >退出房间</el-button
        >
      </el-form-item>
      <el-form-item>
        <el-switch
          v-model="form.micEnabled"
          :disabled="!inRoom"
          @change="handleMicToggle"
          active-text="麦克风开启"
          inactive-text="麦克风关闭"
        />
      </el-form-item>
      <el-form-item>
        <el-switch
          v-model="form.speakerEnabled"
          :disabled="!inRoom"
          @change="handleSpeakerToggle"
          active-text="扬声器开启"
          inactive-text="扬声器关闭"
        />
      </el-form-item>
    </el-form>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive } from "vue";
import { ElMessage } from "element-plus";

declare global {
  interface Window {
    gmeApi: {
      init: (appId: string, userId: string) => Promise<any>;
      enterRoom: (roomId: string, openId: string) => Promise<any>;
      exitRoom: () => Promise<any>;
      enableMic: (enabled: boolean) => Promise<any>;
      enableSpeaker: (enabled: boolean) => Promise<any>;
    };
  }
}

const initialized = ref(false);
const inRoom = ref(false);

const form = reactive({
  roomId: "",
  openId: "",
  micEnabled: false,
  speakerEnabled: false,
});

const handleInit = async () => {
  if (!form.openId) {
    ElMessage.warning("请输入用户ID");
    return;
  }

  try {
    const result = await window.gmeApi.init("1600074451", form.openId);
    if (result.success) {
      initialized.value = true;
      ElMessage.success("GME 初始化成功");
    } else {
      ElMessage.error(`GME 初始化失败: ${result.error}`);
    }
  } catch (error) {
    ElMessage.error(`发生错误: ${error}`);
  }
};

const handleEnterRoom = async () => {
  if (!form.roomId || !form.openId) {
    ElMessage.warning("请输入房间ID和用户ID");
    return;
  }

  try {
    const result = await window.gmeApi.enterRoom(form.roomId, form.openId);
    if (result.success) {
      inRoom.value = true;
      ElMessage.success("成功进入房间");
    } else {
      ElMessage.error(`进入房间失败: ${result.error}`);
    }
  } catch (error) {
    ElMessage.error(`发生错误: ${error}`);
  }
};

const handleExitRoom = async () => {
  try {
    const result = await window.gmeApi.exitRoom();
    if (result.success) {
      inRoom.value = false;
      form.micEnabled = false;
      form.speakerEnabled = false;
      ElMessage.success("成功退出房间");
    } else {
      ElMessage.error(`退出房间失败: ${result.error}`);
    }
  } catch (error) {
    ElMessage.error(`发生错误: ${error}`);
  }
};

const handleMicToggle = async (enabled: boolean) => {
  try {
    const result = await window.gmeApi.enableMic(enabled);
    if (result.success) {
      ElMessage.success(`麦克风${enabled ? "开启" : "关闭"}成功`);
    } else {
      form.micEnabled = !enabled;
      ElMessage.error(`麦克风操作失败: ${result.error}`);
    }
  } catch (error) {
    form.micEnabled = !enabled;
    ElMessage.error(`发生错误: ${error}`);
  }
};

const handleSpeakerToggle = async (enabled: boolean) => {
  try {
    const result = await window.gmeApi.enableSpeaker(enabled);
    if (result.success) {
      ElMessage.success(`扬声器${enabled ? "开启" : "关闭"}成功`);
    } else {
      form.speakerEnabled = !enabled;
      ElMessage.error(`扬声器操作失败: ${result.error}`);
    }
  } catch (error) {
    form.speakerEnabled = !enabled;
    ElMessage.error(`发生错误: ${error}`);
  }
};
</script>

<style scoped>
.container {
  max-width: 800px;
  margin: 0 auto;
  padding: 20px;
}

h1 {
  text-align: center;
  margin-bottom: 30px;
}

.el-form-item {
  margin-bottom: 20px;
}
</style>
