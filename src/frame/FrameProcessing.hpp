
#ifndef FRAME_PROCESSSING_HPP
#define FRAME_PROCESSING_HPP

/* Currently #included in OnUpdate.hpp */

/**
 * Functionality for further frame processing before
 * compiling the individual frames into a video
 */

// Deleted unused content for GitHub

inline
void add_frame_counter(RawPixelArray& pixels, int n, unsigned width, unsigned height)
{
    /*
    static const std::vector<RawPixelArray> numbers  = ;//load_number_images();
    // 321 / 10 = 32 ... 32 % 10 = 2
    int hundreds_place = n / 100;
    int tens_place     = (n / 10) % 10;
    int ones_place     = n % 10;

    //#warning "TODO: Add Frame Counter"
    // use Fallout font?

    const RawPixelArray& hundreds_image = numbers[hundreds_place];
    const RawPixelArray& tens_image     = numbers[tens_place];
    const RawPixelArray& ones_image     = numbers[ones_place];
    */
    // TODO overlay image onto pixels:
}
#endif // FRAME_PROCESSSING_HPP
