#include "led-matrix.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

int main(int argc, char **argv) {
    std::cout << "owobot" << std::endl;
    std::cout << "Press Ctrl-C to exit" << std::endl;

    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular";
    defaults.rows = 32;
    defaults.cols = 64;
    defaults.chain_length = 2;
    defaults.parallel = 1;
    defaults.show_refresh_rate = false;
    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
    if (canvas == NULL) {
        std::cout << "Unable to create canvas" << std::endl;
        return 1;
    }

    canvas->Clear();
    delete canvas;

    return 0;
}