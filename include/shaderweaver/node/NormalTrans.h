#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class NormalTrans : public Node
{
public:
	NormalTrans()
		: Node("NormalTrans")
	{
		InitVariables({
			{ t_mat3 | t_unif, NormalMatName() },
			{ t_nor3, "normal" },
		}, {
			{ t_nor3, "_out" },
		}, {
		});
	}

    static const char* NormalMatName() { return "u_normal_matrix"; }

	enum InputID
	{
		ID_NORMAL_MAT = 0,
		ID_NORMAL,
	};

protected:
	virtual std::string GetBody() const override
	{
		// todo: no transpose and inverse, calc normal_matrix by cpu
//		return "#_out# = mat3(transpose(inverse((model_matrix)))) * (normal);\n";
		return "#_out# = #u_normal_matrix# * #normal#;\n";
	}

	RTTR_ENABLE(Node)

}; // NormalTrans

}
}