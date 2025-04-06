/*** 
 * @Author: wanglx
 * @Date: 2025-04-06 09:14:11
 * @LastEditors: wanglx
 * @LastEditTime: 2025-04-06 10:04:31
 * @Description: 
 * @
 * @Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#include <node_api.h>
#include <napi.h>
#include <string>
#include <cstring>

#ifdef GME_MOCK_MODE
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

ITMGContext* ITMGContextGetInstance() {
  static ITMGContext instance;
  return &instance;
}

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

enum ITMG_ROOM_TYPE {
  ITMG_ROOM_TYPE_FLUENCY = 1,
};

#else
#include "tmg_sdk.h"
#endif

class GMEWrapper : public Napi::ObjectWrap<GMEWrapper> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  GMEWrapper(const Napi::CallbackInfo& info);

private:
  static Napi::FunctionReference constructor;
  ITMGContext* context;

  Napi::Value Init(const Napi::CallbackInfo& info);
  Napi::Value Uninit(const Napi::CallbackInfo& info);
  Napi::Value Poll(const Napi::CallbackInfo& info);
  Napi::Value EnterRoom(const Napi::CallbackInfo& info);
  Napi::Value ExitRoom(const Napi::CallbackInfo& info);
  // 其他方法声明...
};

Napi::FunctionReference GMEWrapper::constructor;

GMEWrapper::GMEWrapper(const Napi::CallbackInfo& info) 
  : Napi::ObjectWrap<GMEWrapper>(info) {
  context = ITMGContextGetInstance();
}

Napi::Object GMEWrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "GMEWrapper", {
    InstanceMethod("init", &GMEWrapper::Init),
    InstanceMethod("uninit", &GMEWrapper::Uninit),
    InstanceMethod("poll", &GMEWrapper::Poll),
    InstanceMethod("enterRoom", &GMEWrapper::EnterRoom),
    InstanceMethod("exitRoom", &GMEWrapper::ExitRoom),
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

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return GMEWrapper::Init(env, exports);
}

NODE_API_MODULE(gme_native, Init) 