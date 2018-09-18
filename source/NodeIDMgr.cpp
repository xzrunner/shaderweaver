#include "shaderweaver/NodeIDMgr.h"

#include <assert.h>

namespace sw
{

CU_SINGLETON_DEFINITION(NodeIDMgr);

NodeIDMgr::NodeIDMgr()
{
	Reset();
}

uint32_t NodeIDMgr::Gen()
{
	for (uint32_t i = 0; i < MAX_COUNT; ++i) {
		if (!m_slots[i]) {
			m_slots[i] = true;
			return i;
		}
	}

	throw std::exception("id overflow.");
}

void NodeIDMgr::Return(uint32_t id)
{
	assert(id < MAX_COUNT && m_slots[id]);
	m_slots[id] = false;
}

void NodeIDMgr::Reset()
{
	m_slots.fill(false);
}

}