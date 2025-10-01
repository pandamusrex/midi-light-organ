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
    if (nNote < 37) {
        return;
    }

    if (nNote > 108) {
        return;
    }

    // We have 12 notes (an octave) across - so each row is an octave
    // In the movie they had six rows, so let's do that too

    // The well known movie notes are A#5 C6 G#5 G#4 D#5

    // If you play that on the keyboard MIDI will report out those notes
    // as 82 84 80 68 75
    // Note: MIDI thinks of those notes A#4 C5, etc (an octave different)
    // but it doesn't matter - we're interested in the integer values

    // So, next, figure out the column and row of the rectangle that corresponds to this note

    // In the movie C6 (MIDI C5) (84) appears far right, 3rd of 6 rows (counting from top)
    // So that gives us
    // Row 0: 097 - 108
    // Row 1: 085 - 096
    // Row 2: 073 - 084 <-- C6/MIDI C5
    // Row 3: 061 - 072
    // Row 4: 049 - 060
    // Row 5: 037 - 048

    // Ignore notes < 037 or > 108

    int col = (nNote - 37) % 12;
    int row = 5 - (nNote - 37) / 12;

    // Compute the pixel bounds of that rectangle
    // Top left corner of the matrix is x=0, y=0?
    // Rectangles are always 128/12 = 10 pixels wide
    // and 32/6 = 5 pixels high

    // And add a centering inset for the entire display of (128-12*10)/2 = 8/2 = 4 (x)
    // and (32-6*5)/2 = 2/2 = 1 (y)

    int x0 = 4 + col * 10;
    int y0 = 1 + row * 5;

    // Compute the color

    // TODO: Rainbow it like in the movie. Just red for now.
    int r = isOn ? 255 : 0;

    // Draw it
    for (int x = x0; x < x0 + 10; x++) {
        for (int y = y0; y < y0 + 5; y++) {
            m_pCanvas->SetPixel(x, y, r, 0, 0);
        }
    }
}





