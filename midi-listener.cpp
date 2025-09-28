#include <unistd.h>
#include <iostream>

#include "midi-listener.h"

MidiListener::MidiListener() {
    m_pSignallingBool = 0;
    m_pSeqHandle = 0;
    m_nInPort = 0;
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

void MidiListener::openMidi() {
    snd_seq_open(&m_pSeqHandle, "default", SND_SEQ_OPEN_INPUT, 0);

    snd_seq_set_client_name(m_pSeqHandle, "Midi Listener");
    m_nInPort = snd_seq_create_simple_port(m_pSeqHandle, "listen:in",
        SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION
    );
}