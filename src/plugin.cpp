#include <stdio.h>
#include "plugin.h"
#define STEAMNETWORKINGSOCKETS_STANDALONELIB
#include "steam/isteamnetworkingutils.h"

const char *netConfigTypes[] = {
	"Invalid",
	"Int32",
	"Int64",
	"Float",
	"String",
	"Ptr"
};
const char *netConfigScopes[] = {
	"Invalid",
	"Global",
	"SocketsInterface",
	"ListenSocket",
	"Connection"
};

void PrintSetConfigHelp()
{
	META_CONPRINTF("List of possible values:\n");

	for (ESteamNetworkingConfigValue eValue = SteamNetworkingUtils()->IterateGenericEditableConfigValues(k_ESteamNetworkingConfig_Invalid, true); 
		eValue != k_ESteamNetworkingConfig_Invalid;
		eValue = SteamNetworkingUtils()->IterateGenericEditableConfigValues(eValue, true))
	{
		ESteamNetworkingConfigDataType type;
		ESteamNetworkingConfigScope scope;
		const char* name = SteamNetworkingUtils()->GetConfigValueInfo(eValue, &type, &scope);
		META_CONPRINTF("\t%s (type %s, scope %s)\n", name, netConfigTypes[type], netConfigScopes[scope]);
	}
}

void IterateConfig( const CCommand &command, CUtlVector< CUtlString > &completions )
{
	for (ESteamNetworkingConfigValue eValue = SteamNetworkingUtils()->IterateGenericEditableConfigValues(k_ESteamNetworkingConfig_Invalid, true); 
		eValue != k_ESteamNetworkingConfig_Invalid;
		eValue = SteamNetworkingUtils()->IterateGenericEditableConfigValues(eValue, true))
	{
		if (command.ArgC() >= 2)
		{
			if (!V_stristr(SteamNetworkingUtils()->GetConfigValueInfo(eValue, nullptr, nullptr), command[1])) continue;
		}
		CUtlString str;
		str.Format("%s %s", command[0], SteamNetworkingUtils()->GetConfigValueInfo(eValue, nullptr, nullptr));
		completions.AddToTail(str);
	}
}

CON_COMMAND_F_COMPLETION(netconf, "Get or set SteamNetworkingSockets options.", FCVAR_RELEASE, IterateConfig)
{
	ESteamNetworkingConfigValue eValue = k_ESteamNetworkingConfig_Invalid;
	
	if (args.ArgC() < 2)
	{
		PrintSetConfigHelp();
		return;
	}
	for (ESteamNetworkingConfigValue val = SteamNetworkingUtils()->IterateGenericEditableConfigValues(k_ESteamNetworkingConfig_Invalid, true); 
		val != k_ESteamNetworkingConfig_Invalid;
		val = SteamNetworkingUtils()->IterateGenericEditableConfigValues(val, true))
	{
		const char* name = SteamNetworkingUtils()->GetConfigValueInfo(val, nullptr, nullptr);
		if (!V_stricmp(args.Arg(1), name))
		{
			eValue = val;
			break;
		}
	}
	if (!eValue)
	{
		PrintSetConfigHelp();
		return;
	}
	if (args.ArgC() == 3)
	{
		ESteamNetworkingConfigDataType type;
		SteamNetworkingUtils()->GetConfigValueInfo(eValue, &type, nullptr);
		bool result = false;
		switch (type)
		{
			case k_ESteamNetworkingConfig_Int32:
			{
				int32 newValue = V_atoi(args.Arg(2));
				result = SteamNetworkingUtils()->SetGlobalConfigValueInt32(eValue, newValue);
				break;
			}
			case k_ESteamNetworkingConfig_Int64:
			{
				int64 newValue = V_atoi64(args.Arg(2));
				result = SteamNetworkingUtils()->SetConfigValue(eValue, k_ESteamNetworkingConfig_Global, 0, type, &newValue);
				break;
			}
			case k_ESteamNetworkingConfig_Float:
			{
				float newValue = V_atof(args.Arg(2));
				result = SteamNetworkingUtils()->SetGlobalConfigValueFloat(eValue, newValue);
				break;
			}
			case k_ESteamNetworkingConfig_String:
			{
				result = SteamNetworkingUtils()->SetGlobalConfigValueString(eValue, args.Arg(2));
				break;
			}
			default:
			{
				META_CONPRINTF("Unable to set config value (unsupported type).\n");
				break;
			}
		}
		if (!result)
		{
			META_CONPRINTF("Unable to set config value (unsupported scope?).\n");
			return;
		}
	}
	char buffer[512];
	size_t size = sizeof(buffer);
	ESteamNetworkingConfigDataType type;
	ESteamNetworkingGetConfigValueResult getResult = SteamNetworkingUtils()->GetConfigValue(eValue, k_ESteamNetworkingConfig_Global, 0, &type, buffer, &size);
	switch (type)
	{
		case k_ESteamNetworkingConfig_Int32:
		{
			META_CONPRINTF("netconf %s = %d\n", args.Arg(1), *buffer);
			break;
		}
		case k_ESteamNetworkingConfig_Int64:
		{
			META_CONPRINTF("netconf %s: %lld\n", args.Arg(1), *buffer);
			break;
		}
		case k_ESteamNetworkingConfig_Float:
		{
			META_CONPRINTF("netconf %s: %f\n", args.Arg(1), *buffer);
			break;
		}
		case k_ESteamNetworkingConfig_String:
		{
			META_CONPRINTF("netconf %s: %s\n", args.Arg(1), buffer);
			break;
		}
	}
}
NetConfPlugin g_ThisPlugin;

PLUGIN_EXPOSE(NetConfPlugin, g_ThisPlugin);
bool NetConfPlugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	ConVar_Register();

	return true;
}