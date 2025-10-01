#ifndef NOTE_RENDERER_H
#define NOTE_RENDERER_H

#include <vector>
#include <pthread.h>

#include "led-matrix.h"

#include <alsa/asoundlib.h>

class NoteRenderer {
    public:
        NoteRenderer(rgb_matrix::Canvas *pCanvas);
        void setSignallingBool(std::atomic_bool *pSignallingBool);
        void doWork();

    private:
        void renderNote(int nNote, bool isOn);

        std::atomic_bool *m_pSignallingBool;
        rgb_matrix::Canvas *m_pCanvas;
};

#endif
