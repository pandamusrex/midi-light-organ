#ifndef MIDI_LISTENER_H
#define MIDI_LISTENER_H

#include <atomic>
#include <mutex>

class MidiListener {
    public:
        MidiListener();
        void setSignallingBool(std::atomic_bool *pSignallingBool);
        void doWork();

    private:
        std::atomic_bool *m_pSignallingBool;
};

#endif
