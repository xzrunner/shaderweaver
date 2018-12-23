#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class MatrixTranspose : public sw::Node
{
public:
	MatrixTranspose()
		: Node("MatrixTranspose", 120)
	{
		InitVariables({
			{ t_d_mat, "_in" },
		}, {
			{ t_d_mat, "_out" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = transpose(#_in#);\n";
	}

	RTTR_ENABLE(Node)

}; // MatrixTranspose

}
}