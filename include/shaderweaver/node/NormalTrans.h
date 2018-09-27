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
			{ t_mat4, "model" },
			{ t_nor3, "nor" },
		}, {
			{ t_nor3, "_out" },
		}, {
		});
	}

	enum InputID
	{
		ID_MODEL = 0,
		ID_NORM,
	};

protected:
	virtual std::string GetBody() const override
	{
		// todo: no transpose and inverse
//		return "#_out# = mat3(transpose(inverse((model)))) * (nor);\n";
		return "#_out# = mat3(#model#) * #nor#;\n";
	}

}; // NormalTrans

}
}