#ifndef MIDI_MAILBOX_H
#define MIDI_MAILBOX_H

#include <vector>
#include <pthread.h>

#include <alsa/asoundlib.h>

class MidiMailbox {
    public:
        static MidiMailbox& getInstance() {
            static MidiMailbox instance; // Guaranteed to be thread-safe for initialization
            return instance;
        }

        // Prevent copying and assignment
        MidiMailbox(const MidiMailbox&) = delete;
        MidiMailbox& operator=(const MidiMailbox&) = delete;

        // Threadsafe Mailbox access
        void enqueueMessage(snd_seq_event_t *ev);
        bool dequeueMessage(snd_seq_event_t &ev);

    private:
        MidiMailbox();
        ~MidiMailbox();

        std::vector<snd_seq_event_t> *m_pMailbox;
        pthread_mutex_t m_mtx; // Mutex to protect internal state
};

#endif
