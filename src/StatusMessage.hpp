#ifndef LASER_STATUSMESSAGE_HPP
#define LASER_STATUSMESSAGE_HPP

#include <string>

enum MessageType {
	MSG_BLANK,
	MSG_NOT_ENOUGH_AP,
	MSG_NOTHING_TO_PICK,
	MSG_WEAPON_EQUIPPED,
	MSG_WEAPON_UNEQUIPPED,
	MSG_ITEM_HEALED,
	MSG_ITEM_MAX_HEALTH,
	MSG_ITEM_UNUSABLE,
	MSG_ITEM_NONE_SELECTED,
	MSG_ITEM_INCREASE_AP,
	MSG_ITEM_INCREASE_HP,
	MSG_ITEM_INCREASE_LOS,
	MSG_RELOADING,
	MSG_NOT_ENOUGH_AMMO,
	MSG_INVENTORY_FULL,
};
enum MessageSeverity {
	SEVERITY_INFORMATION,
	SEVERITY_CRITICAL,
	SEVERITY_BOOST,
};

class StatusMessage {
public:
	std::string						getMessage();
	MessageSeverity					getSeverity() { return messageSeverity; }
	void							setStatusMessage(MessageType msg, MessageSeverity severity);
	void							updateStatusMessage(int delta);
	void							clearStatusMessage() { statusMessage = MSG_BLANK;};
private:
	MessageType						statusMessage = MSG_BLANK;
	MessageSeverity					messageSeverity = SEVERITY_INFORMATION;
	int								statusMessageTimeout = 3000000; // time in microseconds that the message will be displayed
	int								currentStatusMessageTimeout = statusMessageTimeout;
};

#endif