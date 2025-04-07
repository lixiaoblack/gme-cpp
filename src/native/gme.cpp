/*** 
 * @Author: wanglx
 * @Date: 2025-04-06 09:14:11
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-07 21:09:57
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

class GMEWrapper : public Napi::ObjectWrap<GMEWrapper>, public ITMGDelegate {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  GMEWrapper(const Napi::CallbackInfo& info);
  virtual void OnEvent(ITMG_MAIN_EVENT_TYPE type, const char* data) override;

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
};

Napi::FunctionReference GMEWrapper::constructor;

GMEWrapper::GMEWrapper(const Napi::CallbackInfo& info) 
  : Napi::ObjectWrap<GMEWrapper>(info) {
  context = ITMGContextGetInstance();
  audioCtrl = context->GetAudioCtrl();
  context->SetTMGDelegate(this);
}

void GMEWrapper::OnEvent(ITMG_MAIN_EVENT_TYPE type, const char* data) {
  printf("GME OnEvent: type=%d, data=%s\n", type, data);
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
  
  try {
    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "String expected for roomId and openId");
      return result;
    }

    std::string roomId = info[0].As<Napi::String>();
    std::string openId = info[1].As<Napi::String>();
    
    // 生成鉴权信息
    unsigned char authBuffer[256] = {0};
    int ret = QAVSDK_AuthBuffer_GenAuthBuffer(
      1400000000, // 替换为实际的 AppID
      roomId.c_str(),
      openId.c_str(),
      "YOUR_SECRET_KEY", // 替换为实际的密钥
      authBuffer,
      256
    );
    
    if (ret != 0) {
      printf("Failed to generate auth buffer, ret=%d\n", ret);
      Napi::Object result = Napi::Object::New(env);
      result.Set("success", false);
      result.Set("error", "Failed to generate auth buffer");
      return result;
    }

    // 进入房间
    ret = context->EnterRoom(roomId.c_str(), ITMG_ROOM_TYPE_FLUENCY, (char*)authBuffer, 256);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", ret == 0);
    result.Set("error", ret == 0 ? "" : "Failed to enter room");
    return result;
  } catch (const std::exception& e) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", e.what());
    return result;
  } catch (...) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", "Unknown error occurred");
    return result;
  }
}

Napi::Value GMEWrapper::ExitRoom(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  try {
    int ret = context->ExitRoom();
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", ret == 0);
    result.Set("error", ret == 0 ? "" : "Failed to exit room");
    return result;
  } catch (const std::exception& e) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", e.what());
    return result;
  } catch (...) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("success", false);
    result.Set("error", "Unknown error occurred");
    return result;
  }
}

Napi::Value GMEWrapper::GetSpeakerList(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object result = Napi::Object::New(env);

  try {
    if (!audioCtrl) {
      printf("GetSpeakerList: audioCtrl is null\n");
      result.Set("success", false);
      result.Set("error", "Audio controller not initialized");
      return result;
    }

    int count = audioCtrl->GetSpeakerListCount();
    printf("GetSpeakerList: speaker count = %d\n", count);
    
    if (count <= 0) {
      result.Set("success", false);
      result.Set("error", "No speaker devices found");
      return result;
    }

    TMGAudioDeviceInfo* devices = new TMGAudioDeviceInfo[count];
    if (!devices) {
      result.Set("success", false);
      result.Set("error", "Failed to allocate memory for devices");
      return result;
    }

    std::memset(devices, 0, sizeof(TMGAudioDeviceInfo) * count);
    printf("GetSpeakerList: allocated memory for %d devices\n", count);
    
    int ret = audioCtrl->GetSpeakerList(devices, count);
    printf("GetSpeakerList: GetSpeakerList returned %d\n", ret);

    if (ret != 0) {
      delete[] devices;
      printf("GetSpeakerList: Failed to get speaker list, error code: %d\n", ret);
      result.Set("success", false);
      result.Set("error", "Failed to get speaker list");
      return result;
    }

    Napi::Array deviceList = Napi::Array::New(env, count);
    for (int i = 0; i < count; i++) {
      Napi::Object device = Napi::Object::New(env);
      device.Set("deviceName", std::string(devices[i].device_name));
      device.Set("deviceId", std::string(devices[i].device_id));
      deviceList[i] = device;
    }

    delete[] devices;

    result.Set("success", true);
    result.Set("devices", deviceList);
    return result;

  } catch (const std::exception& e) {
    result.Set("success", false);
    result.Set("error", e.what());
    return result;
  } catch (...) {
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