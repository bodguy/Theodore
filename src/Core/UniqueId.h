// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef UniqueId_h
#define UniqueId_h

#include <atomic>

namespace Theodore {
	class UniqueId {
	public:
		UniqueId() : instanceId(++type_id) {}

	private:
		static std::atomic<uint32_t> type_id;

	protected:
		uint32_t instanceId;
	};
} // namespace Theodore

#endif /* UniqueId_h */
