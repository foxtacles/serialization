#ifndef PACKETTYPES_H
#define PACKETTYPES_H

#include "Data.hpp"

typedef unsigned char pTypesSize;

enum class pTypes : pTypesSize
{
	ID_GAME_AUTH = ID_GAME_FIRST,
	ID_GAME_LOAD,
	ID_GAME_MOD,
	ID_GAME_START,
	ID_GAME_END,
	ID_GAME_MESSAGE,
	ID_GAME_CHAT,
	ID_GAME_GLOBAL,
	ID_GAME_WEATHER,
	ID_GAME_BASE,
	ID_GAME_DELETED,

	ID_BASE_NEW,
	ID_REFERENCE_NEW,
	ID_OBJECT_NEW,
	ID_VOLATILE_NEW,
	ID_ITEMLIST_NEW,
	ID_ITEM_NEW,
	ID_CONTAINER_NEW,
	ID_ACTOR_NEW,
	ID_PLAYER_NEW,
	ID_OBJECT_REMOVE,

	ID_WINDOW_NEW,
	ID_BUTTON_NEW,
	ID_TEXT_NEW,
	ID_EDIT_NEW,
	ID_CHECKBOX_NEW,
	ID_RADIOBUTTON_NEW,
	ID_WINDOW_REMOVE,

	ID_UPDATE_NAME,
	ID_UPDATE_POS,
	ID_UPDATE_ANGLE,
	ID_UPDATE_CELL,
	ID_UPDATE_LOCK,
	ID_UPDATE_OWNER,
	ID_UPDATE_ACTIVATE,
	ID_UPDATE_COUNT,
	ID_UPDATE_CONDITION,
	ID_UPDATE_EQUIPPED,
	ID_UPDATE_VALUE,
	ID_UPDATE_STATE,
	ID_UPDATE_RACE,
	ID_UPDATE_SEX,
	ID_UPDATE_DEAD,
	ID_UPDATE_FIREWEAPON,
	ID_UPDATE_IDLE,
	ID_UPDATE_CONTROL,
	ID_UPDATE_INTERIOR,
	ID_UPDATE_EXTERIOR,
	ID_UPDATE_CONTEXT,
	ID_UPDATE_CONSOLE,
	ID_UPDATE_WPOS,
	ID_UPDATE_WSIZE,
	ID_UPDATE_WVISIBLE,
	ID_UPDATE_WLOCKED,
	ID_UPDATE_WTEXT,
	ID_UPDATE_WMAXLEN,
	ID_UPDATE_WVALID,
	ID_UPDATE_WCLICK,
	ID_UPDATE_WSELECTED,
	ID_UPDATE_WRSELECTED,
	ID_UPDATE_WGROUP,
	ID_UPDATE_WMODE
};

#endif
