#include "midi-mailbox.h"

MidiMailbox::MidiMailbox() {
    m_pMailbox = new std::vector<snd_seq_event_t>;
    pthread_mutex_init(&m_mtx, NULL);
}

MidiMailbox::~MidiMailbox() {
    delete m_pMailbox;
    pthread_mutex_destroy(&m_mtx);
}

void MidiMailbox::enqueueMessage(snd_seq_event_t *ev) {
    pthread_mutex_lock(&m_mtx); // Acquire lock for thread-safe access
    m_pMailbox->push_back(*ev); // Add to end of vector
    pthread_mutex_unlock(&m_mtx);
}

bool MidiMailbox::dequeueMessage(snd_seq_event_t &ev) {
    pthread_mutex_lock(&m_mtx); // Acquire lock for thread-safe access
    if ( m_pMailbox->empty() ) {
        pthread_mutex_unlock(&m_mtx);
        return false;
    }

    ev = m_pMailbox->front(); // Get from beginning of vector
    m_pMailbox->erase(m_pMailbox->begin());
    pthread_mutex_unlock(&m_mtx);

    return true;
}