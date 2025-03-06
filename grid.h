#ifndef GRID_H
#define GRID_H

#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>

struct ColorValue;

class Grid {
    uint64_t** grid;
    int width;
    int height;

    void AccelerationNeeds(int&, int&, int&, int&);

public:
    uint64_t** getGrid() const;
    uint64_t getValue(int x, int y) const;

    int getWidth() const;
    int getHeight() const;

    Grid(const char *path);
    ~Grid();

    void printGrid() const;

    void Scatter1();
    bool checkScattered();

};

void fillGridWithZeros(uint64_t** grid, int h, int w);

void find_max_coordinates(const char* path, int16_t&, int16_t&);

struct Color {
    uint8_t r, g, b;

    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
};
class Image {
    int m_width, m_height;
    Color* m_colors;
public:
    Image(int w, int h);
    ~Image();

    Color getColor(int x, int y) const;
    void setColor(Color &color, int x, int y);

    void Export(const char* path);
};
#endif //GRID_H
