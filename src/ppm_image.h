#ifndef image_H_
#define image_H_

#include <iostream>

// This is a placeholder class
// Feel free to replace this clas with your own ppm_image class
namespace agl {
    struct ppm_pixel {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    class ppm_image {
    public:

        ppm_image();

        ppm_image(int width, int height);

        ppm_image(const ppm_image &orig);

        ppm_image &operator=(const ppm_image &orig);

        virtual ~ppm_image();

        // save the given filename
        bool save(const std::string &filename) const;

        // return the current width
        inline int width() const { return myWidth; }

        // return the current height
        inline int height() const { return myHeight; }

        // Get the pixel at index (row, col)
        ppm_pixel get(int row, int col) const;

        // Get the pixel at index (row, col)
        void set(int row, int col, const ppm_pixel &color);

    private:
        ppm_pixel *myData;
        unsigned int myWidth;
        unsigned int myHeight;
    };
}

#endif
