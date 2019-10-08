// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef StringUtil_h
#define StringUtil_h

#include "Platform/os_types.h"
#include <vector>
#include <string>
#include <ctime>

#ifdef Theodore_MacOS
#define DateTime tm
#else
#define DateTime std::tm
#endif

namespace Theodore {
  class StringUtil {
  private:
    StringUtil();
    ~StringUtil();

  public:
    static bool Contains(const std::string& txt, const std::string& contain);
    static bool Match(const std::string& txt, const std::string& regex);
    static bool IsComment(const std::string& c);
    static bool IsSpace(char ch);
    static size_t Split(const std::string& txt, std::vector<std::string>& strs, char ch);
    static bool EqualsIgnoreCase(const std::string& strA, const std::string& strB);
    static std::string ReplaceAll(const std::string& str, const std::string& from, const std::string& to);
    static bool StartsWith(const std::string& str, const std::string& prefix);
    static bool EndsWith(const std::string& str, const std::string& suffix);
    static std::string DateToUTCString(DateTime* date);
    static float ParseFloat(const std::string& str);
    static double ParseDouble(const std::string& str);
    static int ParseInteger(const std::string& str);
    // template<typename ...Args>
    // static std::string Join(const std::string& delimiter, Args... args);

  private:
    static std::string monthTable[];
  };

  // template<typename ... std::enable_if_t<std::is_same<Args, std::string>::value>::type = nullptr>
  // std::string StringUtil::Join(const std::string& delimiter, const Args& ... args) {
  //	return "";
  //}
}  // namespace Theodore

#endif  // StringUtil_h
