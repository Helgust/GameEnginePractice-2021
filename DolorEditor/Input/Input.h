#pragma once

enum EInputCommand : size_t
{
	eIC_TurnLeft = 0,
	eIC_TurnRight,
	eIC_MoveForward,
	eIC_MoveBack,
	eIC_MoveLeft,
	eIC_MoveRight,
	eIC_MoveUp,
	eIC_MoveDown,
	eIC_HotReloadScript,

	eIC_Max
};