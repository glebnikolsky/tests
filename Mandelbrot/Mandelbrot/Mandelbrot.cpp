//
// Copyright 2005-2007 Adobe Systems Incorporated
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
//#define BOOST_GIL_EXTENSION_IO_JPEG_C_LIB_COMPILED_AS_CPLUSPLUS
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/extension/io/jpeg.hpp>

// Creates a synthetic image defining the Mandelbrot set.
// The example relies on a virtual_2d_locator to iterate over the pixels in the destination view.
// The pixels (of type rgb8_pixel_t) are generated programmatically, and the code shows how to access
// the colour channels and set them to arbitrary values.


using namespace boost::gil;

// Models a Unary Function
template <typename P>   // Models PixelValueConcept
struct mandelbrot_fn
{
    using point_t = boost::gil::point_t;
    using const_t = mandelbrot_fn;
    using value_type = P;
    using reference = value_type;
    using const_reference = value_type;
    using argument_type = point_t;
    using result_type = reference;
    static constexpr bool is_mutable =false;

    value_type                    _in_color,_out_color;
    point_t                       _img_size;
    static const int MAX_ITER=100;        // max number of iterations

    mandelbrot_fn() {}
    mandelbrot_fn(const point_t& sz, const value_type& in_color, const value_type& out_color) : _in_color(in_color), _out_color(out_color), _img_size(sz) {}

    result_type operator()(const point_t& p) const {
        // normalize the coords to (-2..1, -1.5..1.5)
        // (actually make y -1.0..2 so it is asymmetric, so we can verify some view factory methods)
        double t=get_num_iter(point<double>(p.x/(double)_img_size.x*3-2, p.y/(double)_img_size.y*3-1.0f));//1.5f));
        t=pow(t,0.2);

        value_type ret;
        for (std::size_t k=0; k<num_channels<P>::value; ++k)
            ret[k]=(typename channel_type<P>::type)(_in_color[k]*t + _out_color[k]*(1-t));
        return ret;
    }

private:
    double get_num_iter(const point<double>& p) const {
        point<double> Z(0,0);
        for (int i=0; i<MAX_ITER; ++i) {
            Z = point<double>(Z.x*Z.x - Z.y*Z.y + p.x, 2*Z.x*Z.y + p.y);
            if (Z.x*Z.x + Z.y*Z.y > 4)
                return i/(double)MAX_ITER;
        }
        return 0;
    }
};

int main()
{
    using deref_t = mandelbrot_fn<rgb8_pixel_t>;
    using point_t = deref_t::point_t;
    using locator_t = virtual_2d_locator<deref_t,false>;
    using my_virt_view_t = image_view<locator_t>;

    boost::function_requires<PixelLocatorConcept<locator_t>>();
    gil_function_requires<StepIteratorConcept<locator_t::x_iterator>>();

    point_t dims(200,200);
    my_virt_view_t mandel(dims, locator_t(point_t(0,0), point_t(1,1), deref_t(dims, rgb8_pixel_t(255,0,255), rgb8_pixel_t(0,255,0))));
    write_view("out-mandelbrot.jpg",mandel, jpeg_tag{});

    return 0;
}
