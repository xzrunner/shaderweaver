#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class ReciprocalSquareRoot : public sw::Node
{
public:
	ReciprocalSquareRoot()
		: Node("ReciprocalSquareRoot")
	{
		InitVariables({
			{ 0, "in_" },
		}, {
			{ 0, "out_" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "(out_) = inversesqrt((in_));\n";
	}

}; // ReciprocalSquareRoot

}
}