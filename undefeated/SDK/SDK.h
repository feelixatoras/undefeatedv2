#pragma once
#include "../stdafx.h"

class bf_read;
class bf_write;
typedef unsigned char uint8;
typedef unsigned char BYTE;
typedef unsigned char byte;

#if !defined( INETCHANNELINFO_H )
#define INETCHANNELINFO_H
#ifdef _WIN32
#pragma once
#endif

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

class INetChannelInfo
{
public:

	enum
	{
		GENERIC = 0, // must be first and is default group
		LOCALPLAYER, // bytes for local player entity update
		OTHERPLAYERS, // bytes for other players update
		ENTITIES, // all other entity bytes
		SOUNDS, // game sounds
		EVENTS, // event messages
		USERMESSAGES, // user messages
		ENTMESSAGES, // entity messages
		VOICE, // voice data
		STRINGTABLE, // a stringtable update
		MOVE, // client move cmds
		STRINGCMD, // string command
		SIGNON, // various signondata
		TOTAL, // must be last and is not a real group
	};

	virtual const char* GetName( void ) const = 0; // get channel name
	virtual const char* GetAddress( void ) const = 0; // get channel IP address as string
	virtual float GetTime( void ) const = 0; // current net time
	virtual float GetTimeConnected( void ) const = 0; // get connection time in seconds
	virtual int GetBufferSize( void ) const = 0; // netchannel packet history size
	virtual int GetDataRate( void ) const = 0; // send data rate in byte/sec

	virtual bool IsLoopback( void ) const = 0; // true if loopback channel
	virtual bool IsTimingOut( void ) const = 0; // true if timing out
	virtual bool IsPlayback( void ) const = 0; // true if demo playback

	virtual float GetLatency( int flow ) const = 0; // current latency (RTT), more accurate but jittering
	virtual float GetAvgLatency( int flow ) const = 0; // average packet latency in seconds
	virtual float GetAvgLoss( int flow ) const = 0; // avg packet loss[0..1]
	virtual float GetAvgChoke( int flow ) const = 0; // avg packet choke[0..1]
	virtual float GetAvgData( int flow ) const = 0; // data flow in bytes/sec
	virtual float GetAvgPackets( int flow ) const = 0; // avg packets/sec
	virtual int GetTotalData( int flow ) const = 0; // total flow in/out in bytes
	virtual int GetSequenceNr( int flow ) const = 0; // last send seq number
	virtual bool IsValidPacket( int flow, int frame_number ) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float GetPacketTime( int flow, int frame_number ) const = 0; // time when packet was send
	virtual int GetPacketBytes( int flow, int frame_number, int group ) const = 0; // group size of this packet
	virtual bool GetStreamProgress( int flow, int* received, int* total ) const = 0; // TCP progress if transmitting
	virtual float GetTimeSinceLastReceived( void ) const = 0; // get time since last recieved packet in seconds
	virtual float GetCommandInterpolationAmount( int flow, int frame_number ) const = 0;

	virtual void GetPacketResponseLatency( int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke ) const = 0;

	virtual void GetRemoteFramerate( float* pflFrameTime, float* pflFrameTimeStdDeviation ) const = 0;

	virtual float GetTimeoutSeconds() const = 0;
};

#endif // INETCHANNELINFO_H

class KeyValues
{
public:
	char _pad[0x20];//csgo, for css its a diff size
};

inline void**& getvtable( void* inst, size_t offset = 0 )
{
	return *reinterpret_cast< void*** >( ( size_t )inst + offset );
}

inline const void** getvtable( const void* inst, size_t offset = 0 )
{
	if( !inst && !offset )
		return nullptr;
	return *reinterpret_cast< const void*** >( ( size_t )inst + offset );
}

template< typename Fn >
inline Fn getvfunc( const void* inst, size_t index, size_t offset = 0 )
{
	if( !inst && offset == 0 )
		return NULL;

	return reinterpret_cast< Fn >( getvtable( inst, offset )[ index ] );
}

typedef float matrix3x4[3][4];
typedef float matrix4x4[4][4];

typedef void* (__cdecl* CreateInterface_t)( const char*, int* );

typedef void* (*CreateInterfaceFn)( const char* pName, int* pReturnCode );

typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef float vec_t;

class VMatrix;

//USERCMD OFFSETS
#define USERCMDOFFSET 0xEC
#define VERIFIEDCMDOFFSET 0xF0
#define MULTIPLAYER_BACKUP 150
#define CURRENTCOMMANDOFFSET 0x16E8
#define CURRENTPLAYERCOMMANDOFFSET 0x1640
#define PREIDCTIONSEEDOFFSET 0x30
#define PREDICTIONPLAYEROFFSET 0x54
#define GLOBALSOFFSET 0x53
#define WEAPONDATA_MAXRANGEOFFSET 0x77C
#define WEAPONDATA_DAMAGEOFFSET 0x778
#define WEAPONDATA_RANGEMODIFIEROFFSET 0x780
#define WEAPONDATA_PENETRATIONPOWEROFFSET 0x774
#define INPUTOFFSET 0x5F
#define GETSPREADOFFSET 0x740
#define GETCONEOFFSET 0x744
#define UPDATEACCURACYPENALTYOFFSET 0x748
#define WEAPONIDOFFSET 0x6D8
#define WEAPONDATAOFFSET 0x708
#define GETNAMEOFFSET 0x5CC
#define APPSYSTEMFACTORYOFFSET 0x3D
#define CLIENTFACTORYOFFSET 0x75
#define GLOWINDEXOFFSET 0x1DB8

//LIFESTATE
#define	LIFE_ALIVE				0
#define	LIFE_DYING				1
#define	LIFE_DEAD				2
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

//Player flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

/* MOVE TYPES */
enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

//USERCMD BUTTONS
#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

#include "SDK Headers/SDK_Others.h"

class ISurface;
class IPanel;
class HLCLient;
class CEntityList;
class CEngineClient;
class CInput;
class CGlowObjectManager;
class IVModelInfo;
class IVModelRender;
class CPrediction;
class CGameMovement;
class IMoveHelper;
class CGlobalVars;
class IEngineTrace;
class IPhysicsSurfaceProps;
class CDebugOverlay;
class IMaterialSystem;;
class IVRenderView;
class ICVar;
class ICvar2;
class IEffects;
class IGameEventManager2;

class CInterfaces
{
private:
	void* GetPointer( const char* Module, const char* InterfaceName )
	{
		void* Interface = nullptr;
		char PossibleInterfaceName[1024];

		CreateInterfaceFn CreateInterface = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleA( Module ), "CreateInterface" );
		for( int i = 1; i < 100; i++ )
		{
			Interface = ( void* )CreateInterface( PossibleInterfaceName, nullptr );
			if( Interface != nullptr )
			{
				//cout << PossibleInterfaceName << " Found: 0x " << igreen << Interface << endl;
				break;
			}

			Interface = ( void* )CreateInterface( PossibleInterfaceName, nullptr );
			if( Interface != nullptr )
			{
				//cout << PossibleInterfaceName << " Found: 0x " << igreen << Interface << endl;
				break;
			}
		}

		return Interface;
	}

	void* GetInterfacePtr( const char* interfaceName, const char* ptrName, CreateInterface_t pInterface )
	{
		char szDebugString[1024];

		std::string sinterface = "";
		std::string interfaceVersion = "0";

		for( int i = 0; i <= 99; i++ )
		{
			sinterface = interfaceName;
			sinterface += interfaceVersion;
			sinterface += std::to_string( i );

			void* funcPtr = pInterface( sinterface.c_str(), nullptr );

			if( ( DWORD )funcPtr != 0x0 )
			{
				sprintf_s( szDebugString, "%s: 0x%x (%s%s%i)", ptrName, ( DWORD )funcPtr, interfaceName, interfaceVersion.c_str(), i );
				//cout << iblue << ptrName << igreen << ": 0x" << funcPtr << "(" << iyellow << interfaceName << interfaceVersion << i << igreen << ")" << white << endl;
				return funcPtr;
			}
			if( i >= 99 && interfaceVersion == "0" )
			{
				interfaceVersion = "00";
				i = 0;
			}
			else if( i >= 99 && interfaceVersion == "00" )
			{
				sprintf_s( szDebugString, "%s: 0x%x (error)", ptrName, ( DWORD )funcPtr );
				//cout << ired << ptrName << ": 0x" << funcPtr << " (ERROR)" << white << endl;
			}
		}
		return nullptr;
	}
public:
	void InitialiseSDK()
	{
		auto p = Utils.PatternSearch("client.dll", (BYTE*)"\x8B\x0D\x00\x00\x00\x00\xFF\x75\x08\x8B\x01\xFF\x50\x64", "xx????xxxxxxxx", NULL, NULL);
		if (p)
		{
			pClientMode = **(IClientModeShared***)(p + 2);
			pClientMode = pClientMode;
		}
		//cout << iblue << "pClientMode" << igreen << ": 0x" << pClientMode << white << endl;
		
		CreateInterface_t ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("client.dll"), "CreateInterface");
		CreateInterface_t EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("engine.dll"), "CreateInterface");
		CreateInterface_t VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("vgui2.dll"), "CreateInterface");
		CreateInterface_t VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("vguimatsurface.dll"), "CreateInterface");
		CreateInterface_t MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("materialsystem.dll"), "CreateInterface");
		CreateInterface_t PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("vphysics.dll"), "CreateInterface");
		CreateInterface_t StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("vstdlib.dll"), "CreateInterface");

		pSurface = ( ISurface* )GetInterfacePtr( "VGUI_Surface", "g_pSurface", VGUISurfaceFactory);
		pPanel = ( IPanel* )GetInterfacePtr( "VGUI_Panel", "g_pPanel", VGUI2Factory);
		pClient = ( HLCLient* )GetInterfacePtr( "VClient", "g_pClient", ClientFactory );
		pEngine = ( CEngineClient* )GetInterfacePtr( "VEngineClient", "g_pEngine", EngineFactory );
		pEntList = ( CEntityList* )GetInterfacePtr( "VClientEntityList", "g_pEntList", ClientFactory );
		g_pDebugOverlay = ( CDebugOverlay* )GetInterfacePtr( "VDebugOverlay", "g_pDebugOverlay", EngineFactory );
		pTrace = ( IEngineTrace* )GetInterfacePtr( "EngineTraceClient", "g_pEngineTraceClient", EngineFactory );
		g_pModelInfo = ( IVModelInfo* )GetInterfacePtr( "VModelInfoClient", "g_pModelInfo", EngineFactory );
		g_pModelRender = ( IVModelRender* )GetInterfacePtr( "VEngineModel", "g_ModelRender", EngineFactory );
		g_pPred = ( CPrediction* )GetInterfacePtr( "VClientPrediction", "g_pPred", ClientFactory );
		g_pGameMovement = ( CGameMovement* )GetInterfacePtr( "GameMovement", "g_pGameMovement", ClientFactory );
		pPhysProps = ( IPhysicsSurfaceProps* )GetInterfacePtr( "VPhysicsSurfaceProps", "g_pPhysprops", PhysFactory );
		pMaterialSystem = ( IMaterialSystem* )GetInterfacePtr( "VMaterialSystem", "pMaterialSystem", MatFactory );
		g_pRenderView = ( IVRenderView* )GetInterfacePtr( "VEngineRenderView", "g_pRenderView", EngineFactory );
		pGlobalVars = *( CGlobalVars** )( ( ( *( PDWORD* )pClient )[ 0 ] ) + 0x1B );
		pGlobalVars = ( CGlobalVars* )*( PDWORD )pGlobalVars;
		//cout << iblue << "pGlobalVars " << igreen << ": 0x" << pGlobalVars << white << endl;
		pInput = *( CInput** )( ( *( DWORD** )pClient )[ 15 ] + 0x1 );
		g_ICVars = ( ICVar* )GetInterfacePtr( "VEngineCvar", "g_pCVars", StdFactory );
		//GameEventManager = ( IGameEventManager2* )GetInterfacePtr ( "GAMEEVENTSMANAGER", "g_pGameEventManager", MaterialSystemFactory );
		GameResources = ( DWORD )Utils.PatternSearch( "client.dll", ( PBYTE )"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx", NULL, NULL ) - ( DWORD )pClient;
		//cout << iblue << "GameResources " << igreen << ": 0x" << GameResources << white << endl;
		//g_pEffects = (IEffects*)GetInterfacePtr("VEngineEffects", "g_pEffects", AppSystemFactory);	
		g_GameEventMgr = ( IGameEventManager2* )( EngineFactory ( "GAMEEVENTSMANAGER002", nullptr ) );
		//cout << iblue << "g_GameEventMgr " << igreen << ": 0x" << g_GameEventMgr << white << endl;
		OffsetLineGoes = (DWORD)Utils.PatternSearch("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx", NULL, NULL);
	}

private:
	bool InitialisedSuccessfuly = false;
public:
	//IGameEventManager2* GameEventManager;
	ISurface* pSurface;
	IClientModeShared* pClientMode;
	IPanel* pPanel;
	HLCLient* pClient;
	CEntityList* pEntList;
	CEngineClient* pEngine;
	CInput* pInput;
	CGlowObjectManager* g_pGlowObjectManager;
	IVModelInfo* g_pModelInfo;
	IVModelRender* g_pModelRender;
	CPrediction* g_pPred;
	CGameMovement* g_pGameMovement;
	IMoveHelper* g_pMoveHelper;
	CGlobalVars* pGlobalVars;
	IEngineTrace* pTrace;
	//DWORD* pGameEventManager;
	IPhysicsSurfaceProps* pPhysProps;
	CDebugOverlay* g_pDebugOverlay;
	IMaterialSystem* pMaterialSystem;
	IVRenderView* g_pRenderView;
	IEffects* g_pEffects;
	ICVar* g_ICVars;
	IGameEventManager2* pGameEventManager;
	IGameEventManager2* g_GameEventMgr;
	DWORD OffsetLineGoes;
	//IGameEventListener2* GameEventListener = nullptr;
	DWORD GameResources = 0;

	HANDLE __FNTHANDLE;
};

extern CInterfaces Interfaces;

#include "Math.h"

#include "SDK Headers/Valve/checksum_crc.h"
#include "SDK Headers/Valve/dt_recv2.h"
#include "SDK Headers/NetVars.h"

#include "SDK Headers/ISurface.h"
#include "SDK Headers/CClient.h"
#include "SDK Headers/EngineClient.h"
#include "SDK Headers/Entitys.h"
#include "SDK Headers/EntList.h"
#include "SDK Headers/DebugOverlay.h"
#include "SDK Headers/CTrace.h"
#include "SDK Headers/IVRenderView.h"
#include "SDK Headers/CModelInfo.h"
#include "SDK Headers/CInput.h"
#include "SDK Headers/ICVars.h"
#include "SDK Headers/CGlobleVars.h"
#include "SDK Headers/CGameMovement.h"
#include "SDK Headers/CPred.h"

void UTIL_TraceLine( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, const IHandleEntity* ignore, int collisionGroup, trace_t* ptr );

void UTIL_ClipTraceToPlayers( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr );

bool TraceToExit( Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace );

/* FIRE BULLET DATA */
struct FireBulletData
{
	FireBulletData( const Vector& eye_pos ) : src( eye_pos )
	{
	}

	Vector src;
	trace_t enter_trace;
	Vector direction;
	CTraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

/* HITGROUP DEFINITIONS */
#define		HITGROUP_GENERIC    0
#define		HITGROUP_HEAD       1
#define		HITGROUP_CHEST      2
#define		HITGROUP_STOMACH    3
#define		HITGROUP_LEFTARM    4
#define		HITGROUP_RIGHTARM   5
#define		HITGROUP_LEFTLEG    6
#define		HITGROUP_RIGHTLEG   7
#define		HITGROUP_GEAR       10

typedef bool (*ShouldHitFunc_t)( IHandleEntity* pHandleEntity, int contentsMask );

enum class CSGOClassID
{
	CTestTraceline = 196,
	CTEWorldDecal = 197,
	CTESpriteSpray = 194,
	CTESprite = 193,
	CTESparks = 192,
	CTESmoke = 191,
	CTEShowLine = 189,
	CTEProjectedDecal = 186,
	CFEPlayerDecal = 61,
	CTEPlayerDecal = 185,
	CTEPhysicsProp = 182,
	CTEParticleSystem = 181,
	CTEMuzzleFlash = 180,
	CTELargeFunnel = 178,
	CTEKillPlayerAttachments = 177,
	CTEImpact = 176,
	CTEGlowSprite = 175,
	CTEShatterSurface = 188,
	CTEFootprintDecal = 172,
	CTEFizz = 171,
	CTEExplosion = 169,
	CTEEnergySplash = 168,
	CTEEffectDispatch = 167,
	CTEDynamicLight = 166,
	CTEDecal = 164,
	CTEClientProjectile = 163,
	CTEBubbleTrail = 162,
	CTEBubbles = 161,
	CTEBSPDecal = 160,
	CTEBreakModel = 159,
	CTEBloodStream = 158,
	CTEBloodSprite = 157,
	CTEBeamSpline = 156,
	CTEBeamRingPoint = 155,
	CTEBeamRing = 154,
	CTEBeamPoints = 153,
	CTEBeamLaser = 152,
	CTEBeamFollow = 151,
	CTEBeamEnts = 150,
	CTEBeamEntPoint = 149,
	CTEBaseBeam = 148,
	CTEArmorRicochet = 147,
	CTEMetalSparks = 179,
	CSteamJet = 142,
	CSmokeStack = 135,
	DustTrail = 246,
	CFireTrail = 64,
	SporeTrail = 252,
	SporeExplosion = 251,
	RocketTrail = 249,
	SmokeTrail = 250,
	CPropVehicleDriveable = 122,
	ParticleSmokeGrenade = 248,
	CParticleFire = 100,
	MovieExplosion = 247,
	CTEGaussExplosion = 174,
	CEnvQuadraticBeam = 56,
	CEmbers = 46,
	CEnvWind = 60,
	CPrecipitation = 116,
	CPrecipitationBlocker = 117,
	CBaseTempEntity = 18,
	NextBotCombatCharacter = 0,
	CEconWearable = 45,
	CBaseAttributableItem = 4,
	CEconEntity = 44,
	CWeaponXM1014 = 244,
	CWeaponTaser = 239,
	CSmokeGrenade = 133,
	CWeaponSG552 = 236,
	CSensorGrenade = 129,
	CWeaponSawedoff = 232,
	CWeaponNOVA = 228,
	CIncendiaryGrenade = 87,
	CMolotovGrenade = 97,
	CWeaponM3 = 220,
	CKnifeGG = 94,
	CKnife = 93,
	CHEGrenade = 84,
	CFlashbang = 66,
	CWeaponElite = 211,
	CDecoyGrenade = 40,
	CDEagle = 39,
	CWeaponUSP = 243,
	CWeaponM249 = 219,
	CWeaponUMP45 = 242,
	CWeaponTMP = 241,
	CWeaponTec9 = 240,
	CWeaponSSG08 = 238,
	CWeaponSG556 = 237,
	CWeaponSG550 = 235,
	CWeaponScout = 234,
	CWeaponSCAR20 = 233,
	CSCAR17 = 127,
	CWeaponP90 = 231,
	CWeaponP250 = 230,
	CWeaponP228 = 229,
	CWeaponNegev = 227,
	CWeaponMP9 = 226,
	CWeaponMP7 = 225,
	CWeaponMP5Navy = 224,
	CWeaponMag7 = 223,
	CWeaponMAC10 = 222,
	CWeaponM4A1 = 221,
	CWeaponHKP2000 = 218,
	CWeaponGlock = 217,
	CWeaponGalilAR = 216,
	CWeaponGalil = 215,
	CWeaponG3SG1 = 214,
	CWeaponFiveSeven = 213,
	CWeaponFamas = 212,
	CWeaponBizon = 207,
	CWeaponAWP = 205,
	CWeaponAug = 204,
	CAK47 = 1,
	CWeaponCSBaseGun = 209,
	CWeaponCSBase = 208,
	CC4 = 29,
	CWeaponBaseItem = 206,
	CBaseCSGrenade = 8,
	CSmokeGrenadeProjectile = 134,
	CSensorGrenadeProjectile = 130,
	CMolotovProjectile = 98,
	CItem_Healthshot = 91,
	CItemDogtags = 92,
	CDecoyProjectile = 41,
	CFireCrackerBlast = 62,
	CInferno = 88,
	CChicken = 31,
	CFootstepControl = 68,
	CCSGameRulesProxy = 34,
	CWeaponCubemap = 0,
	CWeaponCycler = 210,
	CTEPlantBomb = 183,
	CTEFireBullets = 170,
	CTERadioIcon = 187,
	CPlantedC4 = 108,
	CCSTeam = 38,
	CCSPlayerResource = 36,
	CCSPlayer = 35,
	CCSRagdoll = 37,
	CTEPlayerAnimEvent = 184,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CBaseCSGrenadeProjectile = 9,
	CHandleTest = 83,
	CTeamplayRoundBasedRulesProxy = 146,
	CSpriteTrail = 140,
	CSpriteOriented = 139,
	CSprite = 138,
	CRagdollPropAttached = 125,
	CRagdollProp = 124,
	CPredictedViewModel = 118,
	CPoseController = 114,
	CGameRulesProxy = 82,
	CInfoLadderDismount = 89,
	CFuncLadder = 74,
	CTEFoundryHelpers = 173,
	CEnvDetailController = 52,
	CWorld = 245,
	CWaterLODControl = 203,
	CWaterBullet = 202,
	CVoteController = 201,
	CVGuiScreen = 200,
	CPropJeep = 121,
	CPropVehicleChoreoGeneric = 0,
	CTriggerSoundOperator = 199,
	CBaseVPhysicsTrigger = 22,
	CTriggerPlayerMovement = 198,
	CBaseTrigger = 20,
	CTest_ProxyToggle_Networkable = 195,
	CTesla = 190,
	CBaseTeamObjectiveResource = 17,
	CTeam = 145,
	CSunlightShadowControl = 144,
	CSun = 143,
	CParticlePerformanceMonitor = 101,
	CSpotlightEnd = 137,
	CSpatialEntity = 136,
	CSlideshowDisplay = 132,
	CShadowControl = 131,
	CSceneEntity = 128,
	CRopeKeyframe = 126,
	CRagdollManager = 123,
	CPhysicsPropMultiplayer = 106,
	CPhysBoxMultiplayer = 104,
	CPropDoorRotating = 120,
	CBasePropDoor = 16,
	CDynamicProp = 43,
	CProp_Hallucination = 119,
	CPostProcessController = 115,
	CPointWorldText = 113,
	CPointCommentaryNode = 112,
	CPointCamera = 111,
	CPlayerResource = 110,
	CPlasma = 109,
	CPhysMagnet = 107,
	CPhysicsProp = 105,
	CStatueProp = 141,
	CPhysBox = 103,
	CParticleSystem = 102,
	CMovieDisplay = 99,
	CMaterialModifyControl = 96,
	CLightGlow = 95,
	CItemAssaultSuitUseable = 0,
	CItem = 0,
	CInfoOverlayAccessor = 90,
	CFuncTrackTrain = 81,
	CFuncSmokeVolume = 80,
	CFuncRotating = 79,
	CFuncReflectiveGlass = 78,
	CFuncOccluder = 77,
	CFuncMoveLinear = 76,
	CFuncMonitor = 75,
	CFunc_LOD = 70,
	CTEDust = 165,
	CFunc_Dust = 69,
	CFuncConveyor = 73,
	CFuncBrush = 72,
	CBreakableSurface = 28,
	CFuncAreaPortalWindow = 71,
	CFish = 65,
	CFireSmoke = 63,
	CEnvTonemapController = 59,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvProjectedTexture = 55,
	CEnvParticleScript = 54,
	CFogController = 67,
	CEnvDOFController = 53,
	CCascadeLight = 30,
	CEnvAmbientLight = 51,
	CEntityParticleTrail = 50,
	CEntityFreezing = 49,
	CEntityFlame = 48,
	CEntityDissolve = 47,
	CDynamicLight = 42,
	CColorCorrectionVolume = 33,
	CColorCorrection = 32,
	CBreakableProp = 27,
	CBeamSpotlight = 25,
	CBaseButton = 5,
	CBaseToggle = 19,
	CBasePlayer = 15,
	CBaseFlex = 12,
	CBaseEntity = 11,
	CBaseDoor = 10,
	CBaseCombatCharacter = 6,
	CBaseAnimatingOverlay = 3,
	CBoneFollower = 26,
	CBaseAnimating = 2,
	CAI_BaseNPC = 0,
	CBeam = 24,
	CBaseViewModel = 21,
	CBaseParticleEntity = 14,
	CBaseGrenade = 13,
	CBaseCombatWeapon = 7,
	CBaseWeaponWorldModel = 23,
};

struct dlight_t
{
	int flags;
	Vector origin;
	float radius;
	CColor color;
	float die;
	float decay;
	float minlight;
	int key;
	int style;
	Vector m_Direction;
	float m_InnerAngle;
	float m_OuterAngle;

	float GetRadius() const
	{
		return radius;
	}

	float GetRadiusSquared() const
	{
		return radius * radius;
	}

	float IsRadiusGreaterThanZero() const
	{
		return radius > 0.0f;
	}
};

class IEffects
{
public:
	dlight_t* CL_AllocDlight( int key )
	{
		typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc< OriginalFn >( this, 4 )( this, key );
	}

	dlight_t* CL_AllocElight( int key )
	{
		typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc< OriginalFn >( this, 5 )( this, key );
	}

	dlight_t* GetElightByKey( int key )
	{
		typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc< OriginalFn >( this, 8 )( this, key );
	}
};

class ICollideable
{
public:
	virtual void pad0();

	virtual const Vector& OBBMins() const;

	virtual const Vector& OBBMaxs() const;
};
