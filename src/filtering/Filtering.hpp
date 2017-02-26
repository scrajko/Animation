
#ifndef FILTERING_HPP
#define FILTERING_HPP

#include <vector>
#include <cmath>

using DoubleArray = std::vector<double>;

/**
 * Contains the RGBA pixel array for an image
 * while storing its dimensions
 * in order to pass it as a single convenient object
 */
struct DoubleArrayImage {
    DoubleArrayImage(const DoubleArray& image, int image_width, int image_height)
    : pixels(image), width(image_width), height(image_height)
    {}

    DoubleArray pixels;
    int width;
    int height;
};

/**
 * TODO: templates -> ex: Allow Complex arrays to work (for Fourier Transforms)
 *
 * Image rotation to emulate MatLabs! -> perhaps use some sort of library?
 *   I'm sure there must be some MatLab emulation code thing for C++ online.
 */

/**
 * Represents a kernel matrix used in convolution
 * or other image processing.
 * If the filter is 3x3,
 *  values with have 9 elements,
 *  and size will be 3
 */
struct Kernel {
    DoubleArray values;
    std::size_t size;
};

/**
 * Gaussian distribution with mean = 0
 * https://en.wikipedia.org/wiki/Gaussian_blur
 *  pixels at a distance of more than 3 * sigma are
 *  small enough to be considered effectively zero.
 */
double gaussian_distribution(double x, double y, double sigma)
{
    return (1.0 / (2.0 * Pi * sigma * sigma)) *
            std::exp(-(x *x + y * y) / (2 * sigma * sigma));
}

DoubleArray generate_gaussian_blur_kernel(double sigma)
{
    int size = std::ceil(6.0 * sigma);
    if (size % 2 == 0)
        size++;

    DoubleArray kernel(size * size);

    /*
     * Mapping: if size = 7, 3 = (7 - 1) / 2
       x values should be -3 to 3, so that
       [  0   1   2  3  4  5  6  ]
       [ -3  -2  -1  0  1  2  3  ]
    */

    int offset = (size - 1) / 2;

    double sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        double y = i - offset;
        for (int j = 0; j < size; j++)
        {
            double x = j - offset;
            std::cout << x << std::endl;
            double value = gaussian_distribution(x, y, sigma);
            kernel[i * size + j] = value;
            sum += value;
        }
    }
    for (int i = 0; i < size * size; i++)
    {
        kernel[i] /= sum;
    }

    return kernel;
}


DoubleArray unapply_padding(const DoubleArray& image, unsigned padding, int in_width, int in_height)
{
    int out_width  = in_width  - 2 * padding; // * 2 to account for padding on each side
    int out_height = in_height - 2 * padding;
    DoubleArray image_out( out_width * out_height );

    std::cout << "out_size = [" << out_width << " " << out_height << ", padding = " << padding << std::endl;
    for (int i = padding; i < in_height; i++)
    for (int j = padding; j < in_width;  j++)
    {
        std::cout << "image_out[" << ", " << "] = image_in[" << ", " << "]" << std::endl;
        image_out[(i - padding) * out_width + (j - padding)] = image[(i)*in_width + j];
    }

    return image_out;
}

// TODO: Fix unsigned vs int inputs and for loops
DoubleArray apply_padding(const DoubleArray& image, unsigned padding, int in_width, int in_height)
{
    DoubleArray image_out( (padding + in_width + padding) * (padding + in_height + padding) );
    int out_width  = padding + in_width  + padding;
    int out_height = padding + in_height + padding;

    for (int i = 0; i < in_height; i++)
    {
        for (int j = 0; j < in_width; j++)
        {
            image_out[(i + padding)* (out_width) + j + padding] = image[i * in_width + j];
        }
    }

    // extend the border values instead of zeroing them:

    // Top
    for (unsigned i = 0; i < padding; i++)
    {
        for (int j = 0; j < out_width; j++)
        {
            image_out[i * out_width + j] = image_out[padding * out_width + j];
        }
    }

    // Bottom
    for (int i = out_height - padding - 1; i < out_height; i++)
    {
        for (int j = 0; j < out_width; j++)
        {
            image_out[i * out_width + j] = image_out[(out_height - padding - 1)*out_width + j];
        }
    }

    // Left
    for (unsigned i = 0; i < padding; i++)
    {
        for (int j = 0; j < out_height; j++)
        {
            image_out[j * out_width + i] = image_out[j * out_width + padding];
        }
    }

    // Right
    for (int i = out_width - padding - 1; i < out_width; i++)
    {
        for (int j = 0; j < out_height; j++)
        {
            image_out[j * out_width + i] = image_out[j*out_width + padding];
        }
    }

    return image_out;
}

DoubleArrayImage apply_padding(const DoubleArrayImage& image, unsigned padding)
{
    DoubleArray image_out = apply_padding(image.pixels, padding, image.width, image.height);
    return DoubleArrayImage(image_out, image.width + 2 * padding, image.height + 2 * padding);
}
DoubleArrayImage unapply_padding(const DoubleArrayImage& image, unsigned padding)
{
    DoubleArray image_out = unapply_padding(image.pixels, padding, image.width, image.height);
    return DoubleArrayImage(image_out, image.width - 2 * padding, image.height - 2 * padding);
}


int required_padding(int filter_size)
{
    return filter_size / 2;
}

/*
DoubleArray apply_filter_func(const DoubleArray& image, int in_width, int in_height,
                              FilterFunction f, int kernel_size)
{
    DoubleArray image_out(in_width * in_height);

    int filter_size = kernel_size;
    int padding = kernel_size / 2;

    DoubleArray sub_image(kernel_size * kernel_size);

    for (int i = padding; i < in_height - padding; i++) // rows
    {
        for (int j = padding; j < in_width - padding; j++) // cols
        {

            // TODO: Put in own function

            for (int r = 0; r < filter_size; r++)
            {
                for (int c = 0; c < filter_size; c++)
                {
                    int offset = filter_size / 2;
                    sub_image[r * filter_size + c] = image[(i + r - offset) * in_width + (j + c - offset)];
                }
            }

            std::sort(sub_image);

            // A WEIGHTED median filter would require re-allocation every pixel
            // Could pre-allocate a larger-than-needed array based on
            // the sum of the filter weight used.

            // If max filter:
            image_out[i * in_width + j] = sub_image.back();

            // If median filter:
            image_out[i * in_width + j] = sub_image[sub_image.size() / 2];

            // If min filter:
            image_out[i * in_width + j] = sub_image.front();
        }
    }
}
*/

/**
 * Applies a filter to an image through convolution given the filter kernel
 * O(n^2) --> inefficient for heavy processing
 * padding should be big enough to handle kernel
 * ex: filter size 3x3 requires 1 padding,
       filter size 5x5 requires 2 padding,
       filter size 7x7 requires 3 padding
 */
DoubleArray apply_filter(const DoubleArray& image,         int in_width, int in_height,
                         const DoubleArray& filter_kernel, int kernel_size, int padding)
{
    DoubleArray image_out(in_width * in_height);

    /*
    DoubleArray gaussian_filter = {
        1.0,  4.0,  7.0,  4.0, 1.0,
        4.0, 16.0, 26.0, 16.0, 4.0,
        7.0, 26.0, 41.0, 26.0, 7.0,
        4.0, 16.0, 26.0, 16.0, 4.0,
        1.0,  4.0,  7.0,  4.0, 1.0,
    };
    for (int i = 0; i < gaussian_filter.size(); i++)
    {
        gaussian_filter[i] = gaussian_filter[i] / 273.0;
    }*/

    int filter_size = kernel_size;

    for (int i = padding; i < in_height - padding; i++) // rows
    {
        for (int j = padding; j < in_width - padding; j++) // cols
        {
            double sum = 0.0;
            for (int r = 0; r < filter_size; r++)
            {
                for (int c = 0; c < filter_size; c++)
                {
                    int offset = filter_size / 2;
                    sum += image[(i + r - offset) * in_width + (j + c - offset)] * filter_kernel[r * filter_size + c];
                }
            }
            image_out[i * in_width + j] = sum;
        }
    }

    return image_out;
}

DoubleArray ones(std::size_t width, std::size_t height)
{
    DoubleArray ones_arr(width*height);
    for (auto& element : ones_arr)
        element = 1.0;
    return ones_arr;
}

#endif // FILTERING_HPP
