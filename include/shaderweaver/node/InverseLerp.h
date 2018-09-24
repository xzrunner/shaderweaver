#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class InverseLerp : public sw::Node
{
public:
	InverseLerp()
		: Node("InverseLerp")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
			{ 0,  "t" },
		}, {
			{ 0, "inv_lerp" },
		}, {
		});
	}

		m_dim_group = { 0, 1, MAX_IMPORTS_COUNT };
	virtual void Update() override {
		NodeHelper::TypePropote(*this);
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
		ID_T,
	};

protected:
	virtual std::string GetBody() const override
	{
		return "(inv_lerp) = ((t) - (a)) / ((b) - (a));\n";
	}

}; // InverseLerp

}
}