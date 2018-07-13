#ifndef StringUtil_h
#define StringUtil_h

#include <vector>

namespace Theodore {
	class StringUtil {
	private:
		StringUtil();
		~StringUtil();
	public:
		static bool Contains(const std::string& txt, const std::string& contain);
		static bool Match(const std::string& txt, const std::string& regex);
		static bool Comment(const std::string& c);
		static size_t Split(const std::string& txt, std::vector<std::string>& strs, char ch);
		static bool EqualsIgnoreCase(const std::string& strA, const std::string& strB);
		static std::string ReplaceAll(const std::string& str, const std::string& from, const std::string& to);
		static bool StartsWith(const std::string& str, const std::string& prefix);
		static bool EndsWith(const std::string& str, const std::string& suffix);
		static std::string DateToUTCString(std::tm* date);
		static float ParseFloat(const std::string& str);
		static double ParseDouble(const std::string& str);
		static int ParseInteger(const std::string& str);
		//template<typename ...Args>
		//static std::string Join(const std::string& delimiter, Args... args);

	private:
		static std::string monthTable[];
	};

	//template<typename ... std::enable_if_t<std::is_same<Args, std::string>::value>::type = nullptr>
	//std::string StringUtil::Join(const std::string& delimiter, const Args& ... args) {
	//	return "";
	//}
}

#endif // StringUtil_h

