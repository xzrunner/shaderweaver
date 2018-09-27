#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Comparison : public sw::Node
{
public:
	enum CmpType
	{
		CMP_EQUAL = 0,
		CMP_NOT_EQUAL,
		CMP_LESS,
		CMP_LESS_OR_EQUAL,
		CMP_GREATER,
		CMP_GREATER_OR_EQUAL,
	};

public:
	Comparison(CmpType cmp_type)
		: Node("Comparison")
		, m_cmp_type(cmp_type)
	{
		InitVariables({
			{ 0, "a" },
			{ 0, "b" },
		}, {
			{ t_bool, "_out" },
		}, {
		});

		m_dim_group = { ID_A, ID_B };
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
	};

protected:
	virtual std::string GetBody() const override
	{
		switch (m_cmp_type)
		{
		case CMP_EQUAL:
			return "#_out# = #a# == #b# ? true : false;\n";
		case CMP_NOT_EQUAL:
			return "#_out# = #a# != #b# ? true : false;\n";
		case CMP_LESS:
			return "#_out# = #a# <  #b# ? true : false;\n";
		case CMP_LESS_OR_EQUAL:
			return "#_out# = #a# <= #b# ? true : false;\n";
		case CMP_GREATER:
			return "#_out# = #a# >  #b# ? true : false;\n";
		case CMP_GREATER_OR_EQUAL:
			return "#_out# = #a# >= #b# ? true : false;\n";
		default:
			return "";
		}
	}

private:
	CmpType m_cmp_type;

}; // Comparison

}
}