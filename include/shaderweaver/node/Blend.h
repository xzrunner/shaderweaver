#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Blend : public sw::Node
{
public:
	Blend();

	enum ImportIdx
	{
		IN_BASE4 = 0,
		IN_BLEND4,
		IN_MODE,
	};

protected:
	virtual std::string GetHeader() const override;
	virtual std::string GetBody() const override;

}; // Blend

}
}