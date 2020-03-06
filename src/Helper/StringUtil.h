// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef StringUtil_h
#define StringUtil_h

#include <ctime>
#include <sstream>
#include <string>
#include <vector>

#include "Platform/CondiitonalVars.h"

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
    static bool Contains(const std::string& str, const std::string& substr);
    static bool Matches(const std::string& str, const std::string& pat);
    static bool IsSpace(char ch);
    static bool IsNewLine(char ch);
    static size_t Split(const std::string& str, std::vector<std::string>& slices, const std::string& delim);
		static std::pair<std::string, std::string> SplitPair(const std::string& str, const char* delims);
    static bool EqualsIgnoreCase(const std::string& strA, const std::string& strB);
    static std::string Replace(const std::string& str, const std::string& old_str, const std::string& new_str, int n);
    static std::string ReplaceAll(const std::string& str, const std::string& old_str, const std::string& new_str);
    static bool StartsWith(const std::string& str, const std::string& prefix);
    static bool EndsWith(const std::string& str, const std::string& suffix);
    static std::string ToDateFormat(DateTime* date);
    static float ParseFloat(const std::string& str);
    static double ParseDouble(const std::string& str);
    static int ParseInteger(const std::string& str);
    static long ParseLong(const std::string& str);
    static int UTF8Decode(const char* ch);
    static std::string Trim(const std::string& str);
    static std::string TrimLeft(const std::string& str);
    static std::string TrimRight(const std::string& str);
    template <typename Iter>
    static std::string Join(const std::string& delimiter, Iter start, Iter end);
    template <typename Iter>
    static std::string Join(const std::string& delimiter, const std::string& prefix, const std::string& suffix, Iter start, Iter end);
    static std::string ToLower(const std::string& str);
    static std::string ToUpper(const std::string& str);
    static int Index(const std::string& str, const std::string& substr);
    static int LastIndex(const std::string& str, const std::string& substr);
    static std::string Repeat(const std::string& str, int count);
    static int Count(const std::string& str, const std::string& substr);
		static std::istream& GetLine(std::istream& input, std::string& line);

  private:
    static std::string monthlyMap[];
  };

  template <typename Iter>
  std::string StringUtil::Join(const std::string& delimiter, Iter start, Iter end) {
    std::stringstream ss;
    for (Iter it = start; it != end; it++) {
      if (&*it != &*start) {
        ss << delimiter;
      }
      ss << *it;
    }

    return ss.str();
  }

  template <typename Iter>
  std::string StringUtil::Join(const std::string& delimiter, const std::string& prefix, const std::string& suffix, Iter start, Iter end) {
    std::string output = prefix;
    output.append(Join(delimiter, start, end));
    output.append(suffix);
    return output;
  }
}  // namespace Theodore

#endif  // StringUtil_h
