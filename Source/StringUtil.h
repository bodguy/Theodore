#ifndef StringUtil_h
#define StringUtil_h

#include <vector>

namespace Quark {
	class StringUtil {
	public:
		StringUtil();
		~StringUtil();

		static bool PatternMatch(const std::string& txt, const std::string& regex);
		static bool Comment(const std::string& c);
		static size_t Split(const std::string& txt, std::vector<std::string>& strs, char ch);
		static bool EqualsIgnoreCase(const std::string& strA, const std::string& strB);
		static std::string ReplaceAll(const std::string& str, const std::string& from, const std::string& to);

	private:

	};
}

#endif // StringUtil_h

