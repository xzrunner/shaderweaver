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
			{ 0, "log_in" },
		}, {
			{ 0, "log_out" },
		}, {
		});
	}

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	virtual void Update() override {
		NodeHelper::TypePropote(*this);
	}

	BaseType GetType() const { return m_type; }
	void SetType(BaseType type) { m_type = type; }

protected:
	virtual std::string GetBody() const override
	{
		switch (m_type)
		{
		case BASE_E:
			return "(log_out) = log((log_in));\n";
		case BASE_2:
			return "(log_out) = log2((log_in));\n";
		case BASE_10:
			return "(log_out) = log2((log_in)) / log2(10);\n";
		default:
			return "";
		}
	}

private:
	BaseType m_type;

}; // Log

}
}