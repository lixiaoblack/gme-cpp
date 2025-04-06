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
      <el-form-item label="扬声器设备">
        <el-select
          v-model="form.selectedSpeaker"
          placeholder="请选择扬声器设备"
          @change="handleSpeakerDeviceChange"
          :disabled="!initialized"
        >
          <el-option
            v-for="device in speakerDevices"
            :key="device.deviceId"
            :label="device.deviceName"
            :value="device.deviceId"
          />
        </el-select>
        <el-button
          type="primary"
          size="small"
          style="margin-left: 10px"
          @click="refreshSpeakerDevices"
          :disabled="!initialized"
        >
          刷新设备列表
        </el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script setup lang="ts">
import { ref, reactive, onMounted, onBeforeMount } from "vue";
import { ElMessage } from "element-plus";

// 添加生命周期钩子
onBeforeMount(() => {
  console.log("Component before mount");
});

onMounted(() => {
  console.log("Component mounted");
  console.log("Window gmeApi:", window.gmeApi); // 检查 gmeApi 是否正确注入
});

declare global {
  interface Window {
    gmeApi: {
      init: (appId: string, userId: string) => Promise<any>;
      enterRoom: (roomId: string, openId: string) => Promise<any>;
      exitRoom: () => Promise<any>;
      enableMic: (enabled: boolean) => Promise<any>;
      enableSpeaker: (enabled: boolean) => Promise<any>;
      getSpeakerList: () => Promise<any>;
      selectSpeakerDevice: (deviceId: string) => Promise<any>;
      getCurrentSpeakerDevice: () => Promise<any>;
    };
    log: {
      debug: (...args: any[]) => void;
      info: (...args: any[]) => void;
      error: (...args: any[]) => void;
    };
  }
}

const initialized = ref(false);
const inRoom = ref(false);
const speakerDevices = ref<Array<{ deviceId: string; deviceName: string }>>([]);

const form = reactive({
  roomId: "",
  openId: "",
  micEnabled: false,
  speakerEnabled: false,
  selectedSpeaker: "",
});

const handleInit = async () => {
  window.log.info("handleInit called");

  if (!form.openId) {
    window.log.info("openId is empty");
    ElMessage.warning("请输入用户ID");
    return;
  }

  try {
    window.log.info("before init call");
    const result = await window.gmeApi.init("1600074451", form.openId);
    window.log.info("init result:", result);

    if (result.success) {
      initialized.value = true;
      ElMessage.success("GME 初始化成功");
      // 初始化成功后获取扬声器设备列表
      await refreshSpeakerDevices();
    } else {
      ElMessage.error(`GME 初始化失败: ${result.error}`);
    }
  } catch (error) {
    window.log.error("init error:", error);
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

// 刷新扬声器设备列表
const refreshSpeakerDevices = async () => {
  try {
    window.log.info("refreshSpeakerDevices: start");
    const result = await window.gmeApi.getSpeakerList();
    window.log.info("getSpeakerList result:", result);

    if (result.success) {
      speakerDevices.value = result.devices;
      // 如果当前没有选中的设备，自动选择第一个设备
      if (!form.selectedSpeaker && result.devices.length > 0) {
        form.selectedSpeaker = result.devices[0].deviceId;
        await handleSpeakerDeviceChange(form.selectedSpeaker);
      }
      ElMessage.success("获取扬声器列表成功");
    } else {
      window.log.error("Failed to get speaker list:", result.error);
      ElMessage.error(`获取扬声器列表失败: ${result.error}`);
    }
  } catch (error) {
    window.log.error("refreshSpeakerDevices error:", error);
    ElMessage.error(`获取扬声器列表出错: ${error}`);
  }
};

// 切换扬声器设备
const handleSpeakerDeviceChange = async (deviceId: string) => {
  try {
    const result = await window.gmeApi.selectSpeakerDevice(deviceId);
    if (result.success) {
      ElMessage.success("切换扬声器设备成功");
    } else {
      ElMessage.error(`切换扬声器设备失败: ${result.error}`);
    }
  } catch (error) {
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
