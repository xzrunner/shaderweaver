#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class MatrixOnlyRot : public sw::Node
{
public:
	MatrixOnlyRot()
		: Node("MatrixOnlyRot")
	{
		InitVariables({
			{ t_mat4, "_in" },
		}, {
			{ t_mat4,  "_out" },
		}, {
		});
	}

protected:
	virtual std::string GetBody() const override
	{
        return "#_out# = mat4(mat3(#_in#));\n";
	}

	RTTR_ENABLE(Node)

}; // MatrixOnlyRot

}
}