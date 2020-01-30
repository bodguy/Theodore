// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "StringUtil.h"

#include <regex>
#include <algorithm>

namespace Theodore {
  std::string StringUtil::monthTable[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  StringUtil::StringUtil(void) {}

  StringUtil::~StringUtil(void) {}

  bool StringUtil::Contains(const std::string& txt, const std::string& contain) { return (txt.find(contain) != std::string::npos); }

  bool StringUtil::Match(const std::string& txt, const std::string& regex) {
    std::regex pattern(regex);
    if (std::regex_match(txt, pattern)) {
      return true;
    }

    return false;
  }

  bool StringUtil::IsComment(const std::string& c) { return (c.empty()) || (c.at(0) == '#'); }

  bool StringUtil::IsSpace(char ch) { return (ch == ' ' || ch == '\t' || ch == '\r' || ch == 'n'); }

  size_t StringUtil::Split(const std::string& txt, std::vector<std::string>& strs, char ch) {
    size_t pos = txt.find(ch);
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while (pos != std::string::npos) {
      strs.push_back(txt.substr(initialPos, pos - initialPos));
      initialPos = pos + 1;

      pos = txt.find(ch, initialPos);
    }

    // Add the last one
    strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

    return strs.size();
  }

  bool StringUtil::EqualsIgnoreCase(const std::string& strA, const std::string& strB) {
    size_t length = strA.size();
    if (strB.size() != length) {
      return false;
    }

    for (size_t i = 0; i < length; i++) {
      if (tolower(strA[i]) != tolower(strB[i])) {
        return false;
      }
    }

    return true;
  }

	std::string StringUtil::Replace(const std::string& str, const std::string& old_str, const std::string& new_str, int n) {
  	// TODO
  	return std::string();
  }

  std::string StringUtil::ReplaceAll(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    std::string::size_type pos = 0;
    std::string::size_type offset = 0;

    while ((pos = result.find(from, offset)) != std::string::npos) {
      result.replace(result.begin() + pos, result.begin() + pos + from.length(), to);
      offset = pos + to.length();
    }

    return result;
  }

  bool StringUtil::StartsWith(const std::string& str, const std::string& prefix) { return (str.length() >= prefix.length() && str.compare(0, prefix.length(), prefix) == 0); }

  bool StringUtil::EndsWith(const std::string& str, const std::string& suffix) {
    return (str.length() >= suffix.length() && str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0);
  }

  std::string StringUtil::DateToUTCString(std::tm* date) {
    return std::string(StringUtil::monthTable[date->tm_mon] + " " + std::to_string(date->tm_mday) + ", " + std::to_string(date->tm_year + 1900) + " " + std::to_string(date->tm_hour) + ":" +
                       std::to_string(date->tm_min) + ":" + std::to_string(date->tm_sec));
  }

  float StringUtil::ParseFloat(const std::string& str) { return std::stof(str); }

  double StringUtil::ParseDouble(const std::string& str) { return std::stod(str); }

  int StringUtil::ParseInteger(const std::string& str) { return std::stoi(str); }

	long StringUtil::ParseLong(const std::string& str) {
  	return std::stol(str);
  }

  int StringUtil::UTF8Decode(const char* ch) {
    static const unsigned int limits[] = {0xFF, 0x7F, 0x7FF, 0xFFFF};
    const unsigned char* s = (const unsigned char*)ch;
    unsigned int c = s[0];
    unsigned int res = 0; /* final result */
    if (c < 0x80)         /* ascii? */
      res = c;
    else {
      int count = 0;                    /* to count number of continuation bytes */
      while (c & 0x40) {                /* still have continuation bytes? */
        int cc = s[++count];            /* read next byte */
        if ((cc & 0xC0) != 0x80)        /* not a continuation byte? */
          return -1;                    /* invalid byte sequence */
        res = (res << 6) | (cc & 0x3F); /* add lower 6 bits from cont. byte */
        c <<= 1;                        /* to test next bit */
      }
      res |= ((c & 0x7F) << (count * 5));                                 /* add first byte */
      if (count > 3 || res > 0x10FFFF || res <= limits[count]) return -1; /* invalid byte sequence */
      s += count;                                                         /* skip continuation bytes read */
    }
    return res;
  }

	std::string StringUtil::Trim(const std::string& str) {
		std::string copy = str;
		return TrimLeft(TrimRight(copy));;
  }

	std::string StringUtil::TrimLeft(const std::string& str) {
		std::string copy = str;
		copy.erase(copy.begin(), std::find_if(copy.begin(), copy.end(), [](int ch) {
			return !std::isspace(ch);
		}));
		return copy;
  }

	std::string StringUtil::TrimRight(const std::string& str) {
		std::string copy = str;
		copy.erase(std::find_if(copy.rbegin(), copy.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), copy.end());
		return copy;
	}

	std::string StringUtil::ToLower(const std::string &str) {
		std::string copy = str;
		std::transform(copy.begin(), copy.end(), copy.begin(), tolower);
		return copy;
	}

	std::string StringUtil::ToUpper(const std::string &str) {
		std::string copy = str;
		std::transform(copy.begin(), copy.end(), copy.begin(), toupper);
		return copy;
	}

	int StringUtil::Index(const std::string& str, const std::string& substr) {
		std::string::size_type pos = str.find(substr);
		return (pos != std::string::npos) ? pos : -1;
  }

	int StringUtil::LastIndex(const std::string& str, const std::string& substr) {
		std::string::size_type pos = str.rfind(substr);
		return (pos != std::string::npos) ? pos : -1;
	}

	std::string StringUtil::Repeat(const std::string& str, int count) {
  	if (count < 0) throw std::out_of_range("count is negative");
  	std::string copy;
  	while(count) {
  		copy.append(str);
  		count--;
  	}
		return copy;
  }

	int StringUtil::Count(const std::string& str, const std::string& substr) {
  	if (!substr.empty()) {
  		int n = 0;
  		std::string::size_type pos = 0;
			while ((pos = str.find(substr, pos)) != std::string::npos) {
				n++;
				pos += substr.length();
			}
			return n;
  	}

  	return str.length() + 1;
  }
}  // namespace Theodore
