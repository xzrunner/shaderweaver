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

	enum InputID
	{
		ID_BASE4 = 0,
		ID_BLEND4,
		ID_MODE,
	};

protected:
	virtual std::string GetHeader() const override;
	virtual std::string GetBody() const override;

}; // Blend

}
}