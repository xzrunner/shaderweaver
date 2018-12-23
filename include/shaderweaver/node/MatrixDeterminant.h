#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class MatrixDeterminant : public sw::Node
{
public:
	MatrixDeterminant()
		: Node("MatrixDeterminant", 150)
	{
		InitVariables({
			{ t_d_mat, "_in" },
		}, {
			{ t_flt1,  "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
		return "#_out# = determinant(#_in#);\n";
	}

	RTTR_ENABLE(Node)

}; // MatrixDeterminant

}
}