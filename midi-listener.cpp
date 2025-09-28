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
    this->openMidi();

    do {

        if (this->isMidiEventAvailable()) {
            std::cout << "a MIDI event is available" << std::endl << std::flush;
            this->readMidiEvent();
        } else {
            std::cout << "a MIDI event is NOT available" << std::endl << std::flush;
        }

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

bool MidiListener::isMidiEventAvailable() {
    return (snd_seq_event_input_pending(m_pSeqHandle, 0) > 0);
}

void MidiListener::readMidiEvent() {
    snd_seq_event_t *ev;

    snd_seq_event_input(m_pSeqHandle, &ev);
    switch (ev->type) {
        case SND_SEQ_EVENT_CONTROLLER:
            std::cout << "CONTROL event on Channel:" <<
                ev->data.control.channel <<
                " Value:" <<
                ev->data.control.value <<
                std::endl <<
                std::flush;
            break;
        case SND_SEQ_EVENT_PITCHBEND:
            std::cout << "PITCHBEND event on Channel:" <<
                ev->data.control.channel <<
                " Value:" <<
                ev->data.control.value <<
                std::endl <<
                std::flush;
            break;
        case SND_SEQ_EVENT_NOTEON:
            std::cout << "NOTE ON event on Channel:" <<
                ev->data.control.channel <<
                " Note:" <<
                ev->data.note.note <<
                std::endl <<
                std::flush;
            break;
        case SND_SEQ_EVENT_NOTEOFF:
            std::cout << "NOTE OFF event on Channel:" <<
                ev->data.control.channel <<
                " Note:" <<
                ev->data.note.note <<
                std::endl <<
                std::flush;
            break;
    }

    snd_seq_free_event(ev);
}