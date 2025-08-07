#ifndef CUSTOMMAPMEMENTO_H
#define CUSTOMMAPMEMENTO_H

#include <string>
#include <vector>

class CustomMapMemento {
    private:
        std::string mapFileName;
        int width;
        int height;
        std::vector<int> mapGrid;
    public:
        CustomMapMemento(const std::string& mapFileName, int width, int height, const std::vector<int>& mapGrid);
        std::string getMapFileName() const;
        int getWidth() const;
        int getHeight() const;
        const std::vector<int>& getMapGrid() const;
};

#endif