#include "StringUtil.h"
#include <regex>

namespace Quark {
	StringUtil::StringUtil(void) {
	}

	StringUtil::~StringUtil(void) {
	}

	bool StringUtil::Contains(const std::string& txt, const std::string& contain) {
		return (txt.find(contain) != std::string::npos);
	}

	bool StringUtil::Match(const std::string& txt, const std::string& regex) {
		std::regex pattern(regex);
		if (std::regex_match(txt, pattern)) {
			return true;
		}

		return false;
	}

	bool StringUtil::Comment(const std::string& c) {
		return (c.empty()) || (c.at(0) == '#');
	}

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

	bool StringUtil::StartsWith(const std::string& str, const std::string& prefix) {
		return (str.length() >= prefix.length() && str.compare(0, prefix.length(), prefix) == 0);
	}

	bool StringUtil::EndsWith(const std::string& str, const std::string& suffix) {
		return (str.length() >= suffix.length() && str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0);
	}
}

