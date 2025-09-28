#include <unistd.h>

#include "midi-listener.h"

MidiListener::MidiListener() {
    m_pSignallingBool = 0;
}

void MidiListener::setSignallingBool(std::atomic_bool *pSignallingBool) {
	m_pSignallingBool = pSignallingBool;
}

void MidiListener::doWork() {
	bool bDone = false;
	do {
		sleep(1);
		if (m_pSignallingBool && *m_pSignallingBool) {
			bDone = true;
		}
	} while (!bDone);
}