#pragma once
#include <string>
class CUtils {
public:
	CUtils();
	~CUtils();

	static std::string WCharToUTF8(const std::wstring& wstr);
	static CStringA CUtils::WCharToUTF8(const CStringW& wstr);

	static std::wstring UTF8ToWChar(const std::string& astr);
	static CStringW UTF8ToWChar(const CStringA& astr);
	static std::string GetCurrentTimeStr();

	static std::string ToString(const float value, const std::string& format = "%.2f");
};

