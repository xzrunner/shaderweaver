#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Relay : public sw::Node
{
public:
	Relay()
		: Node("Relay")
	{
		InitVariables({
			{ 0,  "_in" },
		}, {
			{ 0, "_out" },
		}, {
		});

        m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = #_in#;\n";
	}

	RTTR_ENABLE(Node)

}; // Relay

}
}