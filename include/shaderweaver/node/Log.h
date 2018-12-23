#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

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
			{ 0, "_in" },
		}, {
			{ 0, "_out" },
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
			return "#_out# = log(#_in#);\n";
		case BASE_2:
			return "#_out# = log2(#_in#);\n";
		case BASE_10:
			return "#_out# = log2(#_in#) / log2(10);\n";
		default:
			return "";
		}
	}

private:
	BaseType m_type;

	RTTR_ENABLE(Node)

}; // Log

}
}