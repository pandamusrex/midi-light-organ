#include "midi-mailbox.h"

MidiMailbox::MidiMailbox() {
    m_pMailbox = new std::vector<snd_seq_event_t>;
}

MidiMailbox::~MidiMailbox() {
    delete m_pMailbox;
}

void MidiMailbox::enqueueMessage(snd_seq_event_t *ev) {
    std::lock_guard<std::mutex> lock(m_mtx); // Acquire lock for thread-safe access
    m_pMailbox->insert(ev);
}

bool MidiMailbox::dequeueMessage(snd_seq_event_t &ev) {
    std::lock_guard<std::mutex> lock(m_mtx); // Acquire lock for thread-safe access
    if ( m_pMailbox->empty() ) {
        return false;
    }

    ev = m_pMailbox->back();
    m_pMailbox->pop_back();
}