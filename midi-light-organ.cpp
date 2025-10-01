#include <iostream>
#include <unistd.h>

#include <atomic>
#include <thread>

#include "signal.h"

#include "midi-listener.h"
#include "note-renderer.h"

// Zeller
#include "led-matrix.h"

// Image Magick
#include <Magick++.h>
#include <magick/image.h>

MidiListener *pMidiListener = NULL;

std::atomic_bool signalled = false;
static void InterruptHandler(int signo) {
    signalled = true;
}

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

using ImageVector = std::vector<Magick::Image>;

// Given the filename, load the image and scale to the size of the
// matrix.
// If this is an animated image, the resutlting vector will contain multiple.
// https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/examples-api-use/image-example.cc
static ImageVector LoadImageAndScaleImage(const char *filename,
                                          int target_width,
                                          int target_height) {
    ImageVector result;
    ImageVector frames;

    try {
        readImages(&frames, filename);
    } catch (std::exception &e) {
        if (e.what())
            fprintf(stderr, "%s\n", e.what());
        return result;
    }

    if (frames.empty()) {
        fprintf(stderr, "No image found.");
        return result;
    }

    // Animated images have partial frames that need to be put together
    if (frames.size() > 1) {
        Magick::coalesceImages(&result, frames.begin(), frames.end());
    } else {
        result.push_back(frames[0]); // just a single still image.
    }

    for (Magick::Image &image : result) {
        image.scale(Magick::Geometry(target_width, target_height));
    }

    return result;
}


// Copy an image to a Canvas. Note, the RGBMatrix is implementing the Canvas
// interface as well as the FrameCanvas we use in the double-buffering of the
// animted image.
// https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/examples-api-use/image-example.cc
void CopyImageToCanvas(const Magick::Image &image, Canvas *canvas) {
    const int offset_x = 0, offset_y = 0;  // If you want to move the image.
    // Copy all the pixels to the canvas.
    for (size_t y = 0; y < image.rows(); ++y) {
        for (size_t x = 0; x < image.columns(); ++x) {
            const Magick::Color &c = image.pixelColor(x, y);
            if (c.alphaQuantum() < 256) {
                canvas->SetPixel(x + offset_x, y + offset_y,
                    ScaleQuantumToChar(c.redQuantum()),
                    ScaleQuantumToChar(c.greenQuantum()),
                    ScaleQuantumToChar(c.blueQuantum())
                );
            }
        }
    }
}

int main(int argc, char **argv) {
    std::cout << "midi-light-organ" << std::endl;
    std::cout << "Press Ctrl-C to exit" << std::endl;

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);
    signal(SIGHUP, InterruptHandler);

    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular";
    defaults.rows = 32;
    defaults.cols = 64;
    defaults.chain_length = 2;
    defaults.parallel = 1;
    defaults.show_refresh_rate = false;
    defaults.led_rgb_sequence = "rbg";

    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
    if (canvas == NULL) {
        std::cout << "Unable to create canvas" << std::endl;
        return 1;
    }

    Magick::InitializeMagick(0);

    // Show "splash" screen for 3 seconds
    ImageVector images = LoadImageAndScaleImage("media/pandamusrex-128x32.png", 128, 32);
    CopyImageToCanvas(images[0], canvas);
    sleep(3);
    canvas->Clear();

    // Start listening to MIDI
    pMidiListener = new MidiListener();
    pMidiListener->setSignallingBool(&signalled);
    std::thread midiThread(&MidiListener::doWork, pMidiListener);

    // Start Rendering Thread
    pNoteRenderer = new NoteRenderer(canvas);
    pNoteRenderer->setSignallingBool(&signalled);
    std::thread renderThread(&NoteRenderer::doWork, pNoteRenderer);

    // Join all the threads and wait for signalling to occur
    midiThread.join();
    renderThread.join();

    // Signalled, let's clean things up
    canvas->Clear();
    delete pMidiListener;
    delete pNoteRenderer;
    delete canvas;

    return 0;
}