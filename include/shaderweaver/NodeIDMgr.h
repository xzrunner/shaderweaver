#pragma once

#include <cu/cu_macro.h>

#include <array>

namespace sw
{

class NodeIDMgr
{
public:
	uint32_t Gen();
	void Return(uint32_t id);

	void Reset();

private:
	static const uint32_t MAX_COUNT = 1024;

	std::array<bool, MAX_COUNT> m_slots;

	CU_SINGLETON_DECLARATION(NodeIDMgr)

}; // NodeIDMgr

}