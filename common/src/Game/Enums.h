#pragma once

enum MessageType
{
	MessageType_Unknown = 0,
	MessageType_Connect,
	MessageType_Disconnect,
	MessageType_Join,
	MessageType_Leave,
	MessageType_Start,
	MessageType_End,
	MessageType_Update,

	// Confirmations
	MessageType_Connected
};