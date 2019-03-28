#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class MatrixInverse : public sw::Node
{
public:
	MatrixInverse()
		: Node("MatrixInverse")
	{
		InitVariables({
			{ t_d_mat, "_in" },
		}, {
			{ t_d_mat,  "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = inverse(#_in#);\n";
	}

	RTTR_ENABLE(Node)

}; // MatrixInverse

}
}