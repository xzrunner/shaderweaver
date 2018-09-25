#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Posterize : public sw::Node
{
public:
	Posterize()
		: Node("Posterize")
	{
		InitVariables({
			{ 0, "a" },
			{ 0, "steps" },
		}, {
			{ 0, "posterize" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_INPUT = 0,
		ID_STEPS,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(posterize) = floor((a) / (1 / (steps))) * (1 / (steps));\n";
	}

}; // Posterize

}
}