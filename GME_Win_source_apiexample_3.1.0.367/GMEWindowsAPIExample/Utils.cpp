#include "stdafx.h"

#include "Utils.h"

CUtils::CUtils() {}

CUtils::~CUtils() {}

std::string CUtils::WCharToUTF8(const std::wstring& wstr) {
  std::string astr;
  int dwSize = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
  if (dwSize) {
    astr.resize(dwSize);
    ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), const_cast<char*>(astr.data()), dwSize, NULL, NULL);
  }
  return astr;
}

CStringA CUtils::WCharToUTF8(const CStringW& wstr) {
  CStringA astr;
  int dwSize = ::WideCharToMultiByte(CP_UTF8, 0, wstr, wstr.GetLength(), NULL, 0, NULL, NULL);
  if (dwSize) {
    ::WideCharToMultiByte(CP_UTF8, 0, wstr, wstr.GetLength(), astr.GetBufferSetLength(dwSize), dwSize, NULL, NULL);
    astr.ReleaseBuffer();
  }
  return astr;
}

std::wstring CUtils::UTF8ToWChar(const std::string& astr) {
  std::wstring wstr;
  int dwSize = ::MultiByteToWideChar(CP_UTF8, 0, astr.c_str(), (int)astr.length(), NULL, 0);
  if (dwSize) {
    wstr.resize(dwSize);
    ::MultiByteToWideChar(CP_UTF8, 0, astr.c_str(), (int)astr.length(), const_cast<wchar_t*>(wstr.data()), dwSize);
  }
  return wstr;
}

CStringW CUtils::UTF8ToWChar(const CStringA& astr) {
  CStringW wstr;
  int dwSize = ::MultiByteToWideChar(CP_UTF8, 0, astr, astr.GetLength(), NULL, 0);
  if (dwSize) {
    ::MultiByteToWideChar(CP_UTF8, 0, astr, astr.GetLength(), wstr.GetBufferSetLength(dwSize), dwSize);
    wstr.ReleaseBuffer();
  }
  return wstr;
}

std::string CUtils::GetCurrentTimeStr() {
  time_t t = time(NULL);
  char ch[64] = {0};
  tm curTime;
  localtime_s(&curTime, &t);
  strftime(ch, sizeof(ch) - 1, "%Y-%m-%d-%Hh%Mm%Ss", &curTime);
  return ch;
}

std::string CUtils::ToString(const float value, const std::string& format) {
  char strValue[128] = {0};
  sprintf_s(strValue, format.c_str(), value);
  return strValue;
}
