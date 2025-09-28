#include <unistd.h>
#include <iostream>

#include "midi-listener.h"

MidiListener::MidiListener() {
    m_pSignallingBool = 0;
}

void MidiListener::setSignallingBool(std::atomic_bool *pSignallingBool) {
    m_pSignallingBool = pSignallingBool;
}

void MidiListener::doWork() {
    bool bDone = false;

    std::cout << "entering MidiListener doWork" << std::endl << std::flush;
    do {
        std::cout << "listening for midi..." << std::endl << std::flush;
        sleep(1);
        if (m_pSignallingBool && *m_pSignallingBool) {
            bDone = true;
        }
    } while (!bDone);
    std::cout << "exiting MidiListener doWork" << std::endl << std::flush;
}