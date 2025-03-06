#include "parser.cpp"
#include "grid.cpp"

#include <iostream>
#include <cmath>

void drawing_picture(Grid &a, char* output_file, int i) {

    Image img(a.getWidth(), a.getHeight());
    Color white = Color(255, 255, 255);
    Color green = Color(0, 255, 0);
    Color violet = Color(128, 0, 128); // или 255 0 255
    Color yellow = Color(255, 255, 0);
    Color black = Color(0, 0, 0);

    for (int y = 0; y < a.getHeight(); y++) {
        for (int x = 0; x < a.getWidth(); x++) {
            uint64_t pixelColor = a.getValue(x, y);

            if (pixelColor == 0) {img.setColor(white, x, y);}
            else if (pixelColor == 1) {img.setColor(green, x, y);}
            else if (pixelColor == 2) {img.setColor(violet, x, y);}
            else if (pixelColor == 3) {img.setColor(yellow, x, y);}
            else {img.setColor(black, x, y);}

        }
    }

    char data[100];
    strcpy(data, output_file);

    char buffer[20];
    sprintf(buffer, "%d", i);
    
    strcat(data, buffer);
    strcat(data, ".bmp");
    img.Export(data);
}

int main(int argc, char** argv) {

    char* input_file;
    char* output_file;
    int maxiter = 0;
    int freq = 0;
    parse_arguments(&argc, argv, input_file, output_file, &maxiter, &freq);

    std::cout << "input_file = " << input_file << "\n";
    std::cout << "output_file = " << output_file << "\n";
    std::cout << "maxiter = " << maxiter << "\n";
    std::cout << "freq = " << freq << "\n";
    std::cout << '\n';

    Grid a(input_file);

    std::cout << '\n';

    if (freq != 0){
        for (int i = 0; i < maxiter; i++) {
            if (a.checkScattered() == true) {
                std::cout << "scattered !" << std::endl;
                break;
            } else a.Scatter1();
            if (i%freq == 0) {
                drawing_picture(a, output_file, i);
            }
        }
        drawing_picture(a, output_file, -1);
    } else {
        for (int i = 0; i < maxiter; i++) {
            if (a.checkScattered() == true) {
                std::cout << "scattered !" << std::endl;
                break;
            } else a.Scatter1();
        }
        drawing_picture(a, output_file, -1);
    }

    delete[] input_file;
    delete[] output_file;
    return 0;
}
