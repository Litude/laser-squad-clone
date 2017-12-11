#include "StatusMessage.hpp"

std::string StatusMessage::getMessage() {
	switch (statusMessage) {
	case MSG_BLANK:
		return "";
	case MSG_NOT_ENOUGH_AP:
		return "Not enough action points!";
	case MSG_NOTHING_TO_PICK:
		return "Nothing to pick up!";
	case MSG_WEAPON_EQUIPPED:
		return "Equipped weapon";
	case MSG_WEAPON_UNEQUIPPED:
		return "Unequipped weapon";
	case MSG_ITEM_MAX_HEALTH:
		return "Already at maximum health!";
	case MSG_ITEM_HEALED:
		return "Healed the character";
	case MSG_ITEM_UNUSABLE:
		return "Cannot use that item!";
	case MSG_ITEM_NONE_SELECTED:
		return "Select an item first";
	case MSG_RELOADING:
		return "Reloading...";
	case MSG_NOT_ENOUGH_AMMO:
		return "Out of ammo!";
	}
}

void StatusMessage::setStatusMessage(MessageType msg, MessageSeverity severity) {
	currentStatusMessageTimeout = statusMessageTimeout;
	statusMessage = msg;
	messageSeverity = severity;
}

void StatusMessage::updateStatusMessage(int delta) {
	currentStatusMessageTimeout -= delta;
	if (currentStatusMessageTimeout < 0) {
		currentStatusMessageTimeout = 0;
		statusMessage = MSG_BLANK;
	}
}