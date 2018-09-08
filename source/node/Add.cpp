#include "sw/node/Add.h"

#include <assert.h>

namespace sw
{
namespace node
{

void Add::Update()
{
	if (m_imports[0].conns.empty() &&
		m_imports[1].conns.empty()) {
		return;
	}

	VariableType type;
	for (auto& port : m_imports)
	{
		auto in_var = port.GetPair(0);
		if (!in_var) {
			continue;
		}
		type.TypePromoteFrom(in_var->Type());
	}

	m_imports[0].var.Type().TypePromoteFrom(type);
	m_imports[1].var.Type().TypePromoteFrom(type);
	m_exports[0].var.Type().TypePromoteFrom(type);
}

}
}