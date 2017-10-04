#include "Unell_Script.h"

namespace Unell_Script
{
	std::string Unell_accuser(uint32 accid) // accuser(accid);
	{
		
		QueryResult resultuser = LoginDatabase.PQuery("SELECT username FROM account WHERE id = %u", accid);
		if(resultuser)					
		{
			Field * field = resultuser->Fetch();								
			std::string username = field[0].GetString();	
			return username;
		}
		if(!resultuser)
			return "Неизвестно";
	}
	
	bool Unell_isgm(uint32 guid) // Unell_isgm(guid);
	{
		
		QueryResult resultacc = CharacterDatabase.PQuery("SELECT account FROM characters WHERE guid = %u", guid);
		if(resultacc)
		{
			Field * field = resultacc->Fetch();								
			uint32 accid = field[0].GetUInt32();
			
			QueryResult res = LoginDatabase.PQuery("SELECT gmlevel FROM account_access WHERE id = %u", accid);
			if(res)
			{
				Field * field = res->Fetch();								
				uint32 gmlevel = field[0].GetUInt32();
				if (gmlevel > 0)
					return true;
				else
					return false;
			}
			if(!res)
				return false;
		}
		if(!resultacc)
			return false;
	}

	uint32 Unell_refid(uint32 accid) // refid_token(accid);
	{
		
		QueryResult res = LoginDatabase.PQuery("SELECT refereraccid FROM account_token WHERE id = %u", accid);
		if (res)
		{
			uint32 refid = res->Fetch()->GetUInt32();
			return refid;
		}
		if (!res)
		{
			return 0;
		}
		
	}

	bool Unell_acctoken(uint32 accid) // accuser(accid);
	{
		
		QueryResult resultuser = LoginDatabase.PQuery("SELECT id FROM account_token WHERE id = %u", accid);
		
		if(resultuser)					
		{
			return true;
		}
		
		if(!resultuser)
			return false;
	}

	bool Unell_accref(uint32 accid) // accuser(accid);
	{
		
		QueryResult resultuser = LoginDatabase.PQuery("SELECT id FROM account_ref WHERE id = %u", accid);
		
		if(resultuser)					
		{
			return true;
		}
		
		if(!resultuser)
			return false;
	}
}