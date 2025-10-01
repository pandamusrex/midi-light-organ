#include "note-renderer.h"

#include "midi-mailbox.h"

NoteRenderer::NoteRenderer(rgb_matrix::Canvas *pCanvas) : m_pCanvas(pCanvas) {
    m_pSignallingBool = 0;
}

void NoteRenderer::setSignallingBool(std::atomic_bool *pSignallingBool) {
    m_pSignallingBool = pSignallingBool;
}

void NoteRenderer::doWork() {
    bool bDone = false;
    snd_seq_event_t ev;

    MidiMailbox& m = MidiMailbox::getInstance();

    do {
        if (m.dequeueMessage(ev)) {
            if (ev.type == SND_SEQ_EVENT_NOTEON) {
                this->renderNote(ev.data.note.note, true);
            }
            if (ev.type == SND_SEQ_EVENT_NOTEOFF) {
                this->renderNote(ev.data.note.note, false);
            }
        }
        if (m_pSignallingBool && *m_pSignallingBool) {
            bDone = true;
        }
    } while (!bDone);
}

void NoteRenderer::renderNote(int nNote, bool isOn) {
    if (nNote < 0) {
        return;
    }

    if (nNote > 127) {
        return;
    }

    int r = isOn ? 255 : 0;

    for (int y = 0; y < 32; y++) {
        m_pCanvas->SetPixel(nNote, y, r, 0, 0);
    }
}





