#pragma once

class Command
{
public:
	Command() {}
	virtual ~Command() {}
	virtual void Execute() = 0;
};
