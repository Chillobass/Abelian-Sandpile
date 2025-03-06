#include "grid.h"

void addToCstr(char*& cstr, char charik) {
    char* new_cstr = new char[strlen(cstr) + 2];
    strcpy(new_cstr, cstr);
    new_cstr[strlen(cstr)] = charik;
    new_cstr[strlen(cstr) + 1] = '\0';
    if (cstr != "") delete[] cstr;

    cstr = new_cstr;
}

void find_max_coordinates(const char* path, int16_t& max_x, int16_t& max_y) {
    std::ifstream file;
    file.open(path);
    if (file.is_open() == 0) {
        std::cout << "file is not opening\n";
        return;
    }

    char line[(64+32+3)]; // uint64_t + 2*int16_t + 2*'\t' + '\n' == 99
    while (file.getline(line, 99)) {

        char* tempStr = new char[1];
        tempStr[0] = '\0';

        int i = 0;
        while (line[i] != '\t') {
            addToCstr(tempStr, line[i]);
            i++;
        }
        if (std::atoi(tempStr) > max_x) max_x = std::atoi(tempStr);
        delete[] tempStr;
        tempStr = new char[1];
        tempStr[0] = '\0';


        i++;
        while (line[i] != '\t') {
            addToCstr(tempStr, line[i]);
            i++;
        }
        if (std::atoi(tempStr) > max_y) max_y = std::atoi(tempStr);
        delete[] tempStr;
    }
}

Color::Color() {
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Image::Image(int w, int h) {
    m_width = w;
    m_height = h;
    m_colors = new Color[w*h];
}

Image::~Image() {}

Color Image::getColor(int x, int y) const {
    return m_colors[y*m_width + x];
}

void Image::setColor(Color &color, int x, int y) {
    m_colors[y*m_width + x].r = color.r;
    m_colors[y*m_width + x].g = color.g;
    m_colors[y*m_width + x].b = color.b;
}


void Image::Export(const char *path) {
    /*
     1) file header
     2) info header
     3) pixel array
     */
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    if (file.is_open() == 0) {
        std::cout << "file is not opening\n";
        return;
    }
    unsigned char padding[3] = {0, 0, 0};
    const int padding_amount = (4 - (m_width * 3) % 4) % 4; //==alignment last %4 for the case when (m_width * 3) % 4) == 0

    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int filesize = fileHeaderSize + infoHeaderSize + m_width * m_height * 3 + padding_amount * m_height;

    unsigned char fileHeader[fileHeaderSize];

    fileHeader[0] = 'B';
    fileHeader[1] = 'M';

    fileHeader[2] = filesize;
    fileHeader[3] = filesize >> 8;
    fileHeader[4] = filesize >> 16;
    fileHeader[5] = filesize >> 24;

    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;

    fileHeader[10] = fileHeaderSize + infoHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;


    unsigned char infoHeader[infoHeaderSize];

    infoHeader[0] = infoHeaderSize;
    infoHeader[1] = 0;
    infoHeader[2] = 0;
    infoHeader[3] = 0;

    infoHeader[4] = m_width;
    infoHeader[5] = m_width >> 8;
    infoHeader[6] = m_width >> 16;
    infoHeader[7] = m_width >> 24;

    infoHeader[8] = m_height;
    infoHeader[9] = m_height >> 8;
    infoHeader[10] = m_height >> 16;
    infoHeader[11] = m_height >> 24;

    infoHeader[12] = 1;
    infoHeader[13] = 0;
    infoHeader[14] = 24;
    infoHeader[15] = 0;

    for (int i = 16; i < 40; i++) {
        infoHeader[i] = 0;
    }

    file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    file.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            unsigned char r = static_cast<unsigned char>(getColor(x, y).r);
            unsigned char g = static_cast<unsigned char>(getColor(x, y).g);
            unsigned char b = static_cast<unsigned char>(getColor(x, y).b);

            unsigned char color[] = {b, g, r};
            file.write(reinterpret_cast<char*>(color), 3);
        }
        file.write(reinterpret_cast<char*>(padding), padding_amount);
    }

    file.close();
    std::cout << "file has been created" << '\n';

}

uint64_t** Grid::getGrid() const {
    return grid;
}

uint64_t Grid::getValue(int x, int y) const {
    return grid[y][x];
}

int Grid::getWidth() const{
    return this->width;
}

int Grid::getHeight() const{
    return this->height;
}

Grid::Grid(const char *path) {
    std::ifstream file;
    file.open(path); // 1    1    100
    if (!file.is_open()) {
        std::cout << "file is not opening " << "\n";
    }

    char charik;

    int16_t w = 0; // длина строки
    int16_t h = 0; // высота

    find_max_coordinates(path, w, h);
    this->width = ++w;
    this->height = ++h;
    // std::cout << "grid width: " << this->width << "\n";
    // std::cout << "grid height: " << this->height << "\n";

    this->grid = new uint64_t* [h];
    for (int i = 0; i < h; i++) {
        grid[i] = new uint64_t[w];
    } // навыделял память под весь массив

    file.clear();
    file.seekg(0);

    fillGridWithZeros(this->grid, this->height, this->width);

    char line[(64+32+3)]; // uint64_t + 2*int16_t + 2*'\t' + '\n' == 99
    while (file.getline(line, 99)) {

        char* tempStr = new char[1];
        tempStr[0] = '\0';

        int i = 0;
        while (line[i] != '\t') {
            addToCstr(tempStr, line[i]);
            ++i;
        }
        int16_t x_coord = std::atoi(tempStr);
        delete[] tempStr;
        tempStr = new char[1];
        tempStr[0] = '\0';

        ++i;
        while (line[i] != '\t') {
            addToCstr(tempStr, line[i]);
            ++i;
        }
        int16_t y_coord = std::atoi(tempStr);
        delete[] tempStr;
        tempStr = new char[1];
        tempStr[0] = '\0';

        ++i;
        while (i < strlen(line)) {
            addToCstr(tempStr, line[i]);
            ++i;
        }
        uint64_t value = std::atoi(tempStr);

        grid[y_coord][x_coord] = value; // всё ради этого
    }

    file.close();
}

Grid::~Grid() {
    for (int i = 0; i < this->height; i++) {
        delete[] this->grid[i];
    }
    delete[] this->grid;
}

void Grid::printGrid() const {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            std::cout << this->grid[i][j] << ' ';
        }
        std::cout << "\n";
    }
}

void fillGridWithZeros(uint64_t** grid, int h, int w) { // застестил, пашет
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            grid[i][j] = 0;
        }
    }
}

void Grid::AccelerationNeeds(int &up, int &down, int &left, int &right) {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->grid[i][j] >= 4) {
                if (j == 0) left=1;
                if (i == 0) up=1;
                if (j == this->width - 1) right=1;
                if (i == this->height - 1) down=1;
            } // да колхоз, но либо перезаписи, либо 4 флага, а я не хочу 4 новые переменные плодить
        }
    }
} // ну должно

void Grid::Scatter1() {
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;
    AccelerationNeeds(up, down, left, right);

    uint64_t** scattered_grid = new uint64_t*[height+up+down];
    for (int i = 0; i < (height+up+down); i++) {
        scattered_grid[i] = new uint64_t[width+left+right];
    }
    fillGridWithZeros(scattered_grid, height+up+down, width+left+right);

    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            scattered_grid[i+up][j+left] += this->grid[i][j];
            if (this->grid[i][j] >= 4) {
                scattered_grid[i+up][j+left]-=4;
                scattered_grid[i+up-1][j+left]+=1; // единичка наверх
                scattered_grid[i+up+1][j+left]+=1; // единичка вниз
                scattered_grid[i+up][j+left-1]+=1; // единичка налево
                scattered_grid[i+up][j+left+1]+=1; // единичка направо
            }
        }
    }
    this->~Grid();
    this->grid = scattered_grid;
    this->width+=(left+right);
    this->height+=(up+down);
}

bool Grid::checkScattered() {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->grid[i][j] >= 4) {
                return false;
            }
        }
    }
    return true;
}
