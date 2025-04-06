/*** 
 * @Author: wanglx
 * @Date: 2025-04-06 09:14:11
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-06 22:41:28
 * @Description: 
 * @
 * @Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#include <node_api.h>
#include <napi.h>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>

// GME SDK 头文件
#include "av_type.h"
#include "av_audio_ctrl.h"
#include "av_audio_effect_ctrl.h"
#include "av_error.h"
#include "av_ptt.h"
#include "av_room.h"
#include "tmg_sdk.h"

#ifdef GME_MOCK_MODE

// 前向声明
class ITMGAudioCtrl;
ITMGAudioCtrl* ITMGAudioCtrlGetInstance();

// 添加音频设备信息结构体
struct TMGAudioDeviceInfo {
  char device_name[2048];
  char device_id[2048];
};

// Mock GME SDK for macOS
class ITMGContext {
public:
  virtual int Init(const char* appId, const char* userId) { 
    if (!appId || !userId) {
      printf("Mock Init failed: invalid parameters\n");
      return -1;
    }
    printf("Mock Init with appId: %s, userId: %s\n", appId, userId);
    return 0; 
  }
  virtual void Poll() {
    // printf("Mock Poll called\n");
  }
  virtual int Uninit() { 
    printf("Mock Uninit called\n");
    return 0; 
  }
  virtual int EnterRoom(const char* roomId, int roomType, const char* authBuffer, int authBufferLen) { 
    if (!roomId || !authBuffer) {
      printf("Mock EnterRoom failed: invalid parameters\n");
      return -1;
    }
    printf("Mock EnterRoom with roomId: %s\n", roomId);
    return 0; 
  }
  virtual int ExitRoom() { 
    printf("Mock ExitRoom called\n");
    return 0; 
  }
};

class ITMGAudioCtrl {
public:
  virtual int GetSpeakerListCount() {
    return 2;  // 模拟返回2个扬声器设备
  }

  virtual int GetSpeakerList(TMGAudioDeviceInfo* ppDeviceInfoList, int nCount) {
    if (!ppDeviceInfoList || nCount <= 0) return -1;
    
    // 模拟返回两个扬声器设备
    if (nCount >= 2) {
      strcpy(ppDeviceInfoList[0].device_name, "Default Speaker");
      strcpy(ppDeviceInfoList[0].device_id, "default");
      strcpy(ppDeviceInfoList[1].device_name, "扬声器 (Realtek High Definition Audio)");
      strcpy(ppDeviceInfoList[1].device_id, "realtek");
      return 2;
    } else {
      strcpy(ppDeviceInfoList[0].device_name, "Default Speaker");
      strcpy(ppDeviceInfoList[0].device_id, "default");
      return 1;
    }
  }
  
  virtual int SelectSpeaker(const char* pSpeakerID) {
    if (!pSpeakerID) return -1;
    printf("Mock SelectSpeaker called with deviceId: %s\n", pSpeakerID);
    return 0;
  }
  
  virtual int GetCurrentSpeaker(TMGAudioDeviceInfo* pDeviceInfo) {
    if (!pDeviceInfo) return -1;
    // 模拟返回当前扬声器设备
    strcpy(pDeviceInfo->device_name, "Default Speaker");
    strcpy(pDeviceInfo->device_id, "default");
    return 0;
  }
};

ITMGContext* ITMGContextGetInstance() {
  static ITMGContext instance;
  return &instance;
}

ITMGAudioCtrl* ITMGAudioCtrlGetInstance() {
  static ITMGAudioCtrl instance;
  return &instance;
}

enum ITMG_ROOM_TYPE {
  ITMG_ROOM_TYPE_FLUENCY = 1,
};

int QAVSDK_AuthBuffer_GenAuthBuffer(unsigned int appId, const char* roomId, const char* openId, 
                                   const char* key, unsigned char* authBuffer, unsigned int authBufferLen) {
  if (!roomId || !openId || !key || !authBuffer || authBufferLen == 0) {
    return -1;
  }
  // Mock auth buffer generation
  const char* mockData = "mock_auth_buffer";
  strncpy((char*)authBuffer, mockData, authBufferLen);
  return 0;
}

#else
// 在非mock模式下不需要重复包含tmg_sdk.h，因为已经在上面包含了
#endif

class GMEWrapper : public Napi::ObjectWrap<GMEWrapper> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  GMEWrapper(const Napi::CallbackInfo& info);

private:
  static Napi::FunctionReference constructor;
  ITMGContext* context;
  ITMGAudioCtrl* audioCtrl;

  Napi::Value Init(const Napi::CallbackInfo& info);
  Napi::Value Uninit(const Napi::CallbackInfo& info);
  Napi::Value Poll(const Napi::CallbackInfo& info);
  Napi::Value EnterRoom(const Napi::CallbackInfo& info);
  Napi::Value ExitRoom(const Napi::CallbackInfo& info);
  Napi::Value GetSpeakerList(const Napi::CallbackInfo& info);
  Napi::Value SelectSpeakerDevice(const Napi::CallbackInfo& info);
  Napi::Value GetCurrentSpeakerDevice(const Napi::CallbackInfo& info);
  // 其他方法声明...
};

Napi::FunctionReference GMEWrapper::constructor;

GMEWrapper::GMEWrapper(const Napi::CallbackInfo& info) 
  : Napi::ObjectWrap<GMEWrapper>(info) {
  context = ITMGContextGetInstance();
  audioCtrl = context->GetAudioCtrl();
}

Napi::Object GMEWrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "GMEWrapper", {
    InstanceMethod("init", &GMEWrapper::Init),
    InstanceMethod("uninit", &GMEWrapper::Uninit),
    InstanceMethod("poll", &GMEWrapper::Poll),
    InstanceMethod("enterRoom", &GMEWrapper::EnterRoom),
    InstanceMethod("exitRoom", &GMEWrapper::ExitRoom),
    InstanceMethod("getSpeakerList", &GMEWrapper::GetSpeakerList),
    InstanceMethod("selectSpeakerDevice", &GMEWrapper::SelectSpeakerDevice),
    InstanceMethod("getCurrentSpeakerDevice", &GMEWrapper::GetCurrentSpeakerDevice),
    // 其他方法注册...
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("GMEWrapper", func);
  return exports;
}

Napi::Value GMEWrapper::Init(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  try {
    if (info.Length() < 2) {
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "Wrong number of arguments. Expected appId and userId");
      return result;
    }

    if (!info[0].IsString() || !info[1].IsString()) {
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "Wrong arguments. Expected string for both appId and userId");
      return result;
    }

    std::string appId = info[0].As<Napi::String>().Utf8Value();
    std::string userId = info[1].As<Napi::String>().Utf8Value();

    if (appId.empty() || userId.empty()) {
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "AppId and userId cannot be empty");
      return result;
    }

    printf("Initializing GME with appId: %s, userId: %s\n", appId.c_str(), userId.c_str());

    int result = context->Init(appId.c_str(), userId.c_str());
    printf("GME initialization result: %d\n", result);

    Napi::Object returnValue = Napi::Object::New(env);
    returnValue.Set("success", result == 0);
    returnValue.Set("error", result == 0 ? "" : "GME initialization failed");

    return returnValue;
  } catch (const std::exception& e) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", e.what());
    return result;
  } catch (...) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", "Unknown error occurred during initialization");
    return result;
  }
}

Napi::Value GMEWrapper::Uninit(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  int result = context->Uninit();
  return Napi::Number::New(env, result);
}

Napi::Value GMEWrapper::Poll(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  context->Poll();
  return env.Undefined();
}

Napi::Value GMEWrapper::EnterRoom(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
    Napi::TypeError::New(env, "String expected for roomId and openId").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string roomId = info[0].As<Napi::String>();
  std::string openId = info[1].As<Napi::String>();
  
  // 生成鉴权信息
  unsigned char authBuffer[256] = {0};
  QAVSDK_AuthBuffer_GenAuthBuffer(1400000000, // 替换为实际的 AppID
                                 roomId.c_str(),
                                 openId.c_str(),
                                 "YOUR_SECRET_KEY", // 替换为实际的密钥
                                 authBuffer,
                                 256);

  int result = context->EnterRoom(roomId.c_str(), ITMG_ROOM_TYPE_FLUENCY, (char*)authBuffer, 256);
  return Napi::Number::New(env, result);
}

Napi::Value GMEWrapper::ExitRoom(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  int result = context->ExitRoom();
  return Napi::Number::New(env, result);
}

Napi::Value GMEWrapper::GetSpeakerList(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  try {
    if (!audioCtrl) {
      printf("GetSpeakerList: audioCtrl is null\n");
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "Audio controller not initialized");
      return result;
    }

    // 获取扬声器数量
    int count = audioCtrl->GetSpeakerListCount();
    printf("GetSpeakerList: speaker count = %d\n", count);
    
    if (count <= 0) {
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "No speaker devices found");
      return result;
    }

    // 分配内存并初始化
    TMGAudioDeviceInfo* devices = new TMGAudioDeviceInfo[count];
    memset(devices, 0, sizeof(TMGAudioDeviceInfo) * count);
    printf("GetSpeakerList: allocated memory for %d devices\n", count);
    
    // 获取扬声器列表 - 注意参数顺序：ppDeviceInfoList, nCount
    int ret = audioCtrl->GetSpeakerList(devices, count);
    printf("GetSpeakerList: GetSpeakerList(devices=%p, count=%d) returned %d\n", devices, count, ret);

    // 检查返回值 - 0表示成功
    if (ret != 0) {
      delete[] devices;
      printf("GetSpeakerList: Failed to get speaker list, error code: %d\n", ret);
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "Failed to get speaker list");
      return result;
    }

    // 创建返回数组
    Napi::Array deviceList = Napi::Array::New(env, count);
    for (int i = 0; i < count; i++) {
      printf("GetSpeakerList: Device %d - Name: %s, ID: %s\n", i, devices[i].device_name, devices[i].device_id);
      Napi::Object device = Napi::Object::New(env);
      device.Set("deviceName", devices[i].device_name);
      device.Set("deviceId", devices[i].device_id);
      deviceList[i] = device;
    }

    // 释放内存
    delete[] devices;
    printf("GetSpeakerList: memory freed\n");

    // 返回结果
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", true);
    result.Set("devices", deviceList);
    return result;
  } catch (const std::exception& e) {
    printf("GetSpeakerList: Exception caught - %s\n", e.what());
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", e.what());
    return result;
  } catch (...) {
    printf("GetSpeakerList: Unknown exception caught\n");
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", "Unknown error occurred");
    return result;
  }
}

Napi::Value GMEWrapper::SelectSpeakerDevice(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  try {
    if (info.Length() < 1 || !info[0].IsString()) {
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "Device ID must be a string");
      return result;
    }

    std::string deviceId = info[0].As<Napi::String>().Utf8Value();
    int ret = audioCtrl->SelectSpeaker(deviceId.c_str());

    Napi::Object result = Napi::Object::New(env);
    result.Set("success", ret == 0);
    result.Set("error", ret == 0 ? "" : "Failed to select speaker device");
    return result;
  } catch (const std::exception& e) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", e.what());
    return result;
  }
}

Napi::Value GMEWrapper::GetCurrentSpeakerDevice(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  try {
    TMGAudioDeviceInfo device;
    int ret = audioCtrl->GetCurrentSpeaker(&device);

    Napi::Object result = Napi::Object::New(env);
    if (ret == 0) {
      result.Set("success", true);
      Napi::Object deviceInfo = Napi::Object::New(env);
      deviceInfo.Set("deviceName", device.device_name);
      deviceInfo.Set("deviceId", device.device_id);
      result.Set("device", deviceInfo);
    } else {
      result.Set("success", false);
      result.Set("error", "Failed to get current speaker device");
    }
    return result;
  } catch (const std::exception& e) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", e.what());
    return result;
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return GMEWrapper::Init(env, exports);
}

NODE_API_MODULE(gme_native, Init) 