#ifndef UNELL_SCRIPT_H
#define UNELL_SCRIPT_H

#include "Define.h"
#include <string>
#include "Player.h"


namespace Unell_Script
{
	std::string Unell_accuser(uint32 accid);
	uint32 Unell_refid(uint32 accid);
	bool Unell_acctoken(uint32 accid);
	bool Unell_accref(uint32 accid);
	bool Unell_isgm(uint32 guid);
};

#endif