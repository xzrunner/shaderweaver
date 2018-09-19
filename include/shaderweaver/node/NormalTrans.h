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
			{ t_nor3, "nor_trans" },
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
//		return "(nor_trans) = mat3(transpose(inverse((model)))) * (nor);\n";
		return "(nor_trans) = mat3((model)) * (nor);\n";
	}

}; // NormalTrans

}
}