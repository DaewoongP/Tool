#pragma once

enum OBJID		{ PLAYER, BRICK, WIRE, SPIKE, TEEMO, OBJ_END };
enum TEXTYPE	{ TEX_SINGLE, TEX_MULTI, TEX_END };
enum SCENEID    { SC_STAGE1, SC_END };
enum CHANNELID {
	SOUND_BGM,
	SOUND_EFFECT,
	SOUND_HIT,
	SOUND_DEAD,
	MAXCHANNEL
};
enum DIRECTION { LEFT, RIGHT, DIR_END };

enum PICKMOD { PICK_OBJ, PICK_TILE, PICK_END};