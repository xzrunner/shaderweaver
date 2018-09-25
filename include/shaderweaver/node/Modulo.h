#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Modulo : public sw::Node
{
public:
	Modulo()
		: Node("Modulo")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
		}, {
			{ 0, "modulo" },
		}, {
		});

		m_dim_group = { ID_A, ID_B, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(modulo) = mod((a), (b));\n";
	}

}; // Modulo

}
}