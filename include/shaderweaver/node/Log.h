#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Log : public sw::Node
{
public:
	enum BaseType
	{
		BASE_E = 0,
		BASE_2,
		BASE_10,
	};

public:
	Log(BaseType type)
		: Node("Log")
		, m_type(type)
	{
		InitVariables({
			{ 0, "a" },
		}, {
			{ 0, "log" },
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
			return "(log) = log((a));\n";
		case BASE_2:
			return "(log) = log2((a));\n";
		case BASE_10:
			return "(log) = log2((a)) / log2(10);\n";
		default:
			return "";
		}
	}

private:
	BaseType m_type;

}; // Log

}
}