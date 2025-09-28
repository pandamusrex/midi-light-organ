#ifndef MIDI_LISTENER_H
#define MIDI_LISTENER_H

#include <atomic>
#include <mutex>

#include <alsa/asoundlib.h>

class MidiListener {
    public:
        MidiListener();
        void setSignallingBool(std::atomic_bool *pSignallingBool);
        void doWork();

    private:
        void openMidi();

        std::atomic_bool *m_pSignallingBool;
        snd_seq_t *m_pSeqHandle;
        int m_nInPort;
};

#endif
