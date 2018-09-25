#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Exponential : public sw::Node
{
public:
	enum BaseType
	{
		BASE_E = 0,
		BASE_2,
	};

public:
	Exponential(BaseType type)
		: Node("Exponential")
		, m_type(type)
	{
		InitVariables({
			{ 0, "a" },
		}, {
			{ 0, "exp" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

	BaseType GetType() const { return m_type; }
	void SetType(BaseType type) { m_type = type; }

protected:
	virtual std::string GetBody() const override
	{
		switch (m_type)
		{
		case BASE_E:
			return "(exp) = exp((a));\n";
		case BASE_2:
			return "(exp) = exp2((a));\n";
		default:
			return "";
		}
	}

private:
	BaseType m_type;

}; // Exponential

}
}