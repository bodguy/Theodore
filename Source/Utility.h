#ifndef Utility_h
#define Utility_h

namespace Quark {
	#define SafeDealloc(p) { if(p) { delete (p); (p)=NULL; } }
	#define SafeArrayDealloc(p) { if(p) { delete[] (p); (p)=NULL; } }
	#define SafeContDealloc(p) { for(auto i : p) SafeDealloc(i); p.clear(); }
	#define UnUsed(p) ((void)p)

	enum States {
		NONE = 0,
		ONLY_A,
		ONLY_B,
		BOTH
	};

	class Utility {
	public:
		struct KeyAndValueCompare {
			template <typename T>
			bool operator() (T const &lhs, T const &rhs) const {
				return lhs.first == rhs.first && *(lhs.second) == *(rhs.second);
			}
		};

		struct ValueCompare {
			template <typename T>
			bool operator() (T const &lhs, T const &rhs) const {
				return *lhs == *rhs;
			}
		};

		template <typename T>
		static bool CompareUnorderedmap(T const &lhs, T const &rhs) {
			// No predicate needed because there is operator== for pairs already.
			return lhs.size() == rhs.size()
				&& std::equal(lhs.begin(), lhs.end(),
					rhs.begin(), KeyAndValueCompare());
		}

		template <typename T>
		static bool CompareVector(T const &lhs, T const &rhs) {
			// No predicate needed because there is operator== for pairs already.
			return lhs.size() == rhs.size()
				&& std::equal(lhs.begin(), lhs.end(),
					rhs.begin(), ValueCompare());
		}

		inline static bool SortLayer(unsigned int a, unsigned int b) {
			return a < b;
		}

		inline static int States(int X, int Y) { return (X << 1) | Y; } // switch (STATES(a,b))

	private:
		Utility() {}
		~Utility() {}

	};
}
#endif /* Utility_h */
