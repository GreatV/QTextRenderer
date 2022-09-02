#include "bgmanager.h"
#include "utils/utils.h"

#include <assert.h>

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

#include <fmt/core.h>

namespace fs = boost::filesystem;

// Determine if alpha channel is used in an image
bool is_transparent_image(const std::string &image_path)
{
    cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED);

    auto channels = image.channels();
    if (channels == 4) {
        auto *pixel_ptr = (uint8_t *) image.data;
        for (auto i = 0; i < image.rows; i++) {
            for (auto j = 0; j < image.cols; j++) {
                auto alpha = pixel_ptr[i * image.cols * channels + j * channels + 3];
                if (alpha != 255) {
                    return true;
                }
            }
        }
    }

    return false;
}

BgManager::BgManager(const std::string &bg_dir, const bool pre_load)
    : pre_load(pre_load)
{
    std::vector<std::string>
        image_extensions{".jpeg", ".JPEG", ".jpg", ".JPG", ".png", ".PNG", ".bmp", ".BMP"};

    fs::path current_path(bg_dir);
    if (!(fs::exists(current_path) && fs::is_directory(current_path))) {
        BOOST_LOG_TRIVIAL(warning)
            << fmt::format("Current path {} is not exists or is not a directory,", bg_dir);
    }
    fs::recursive_directory_iterator dir_iter(current_path);
    for (auto &dir_entry : dir_iter) {
        if (fs::is_regular_file(dir_entry)
            && (std::find(image_extensions.begin(),
                          image_extensions.end(),
                          dir_entry.path().extension())
                != image_extensions.end())) {
            std::string real_bg_path = dir_entry.path().string();
            if (is_transparent_image(real_bg_path)) {
                BOOST_LOG_TRIVIAL(warning) << fmt::format(
                    "Ignore transparent background image, please convert is to JPEG: {}",
                    real_bg_path);
                continue;
            }
            bg_paths.push_back(real_bg_path);
            if (pre_load) {
                cv::Mat cached_image;
                get_cached_image(cached_image, real_bg_path);
                bg_imgs.push_back(cached_image);
            }
        }
    }
    assert(bg_imgs.empty() != false && "background image is empty");
}

// LRU Cache
void BgManager::get_cached_image(cv::Mat &img, const std::string &bg_path)
{
    if (bg_image_cache.contains(bg_path)) {
        auto optional = bg_image_cache.get(bg_path);
        img = optional.get();
    } else {
        cv::Mat cached_image = cv::imread(bg_path);
        bg_image_cache.insert(bg_path, cached_image);
    }
}

void BgManager::get_image(cv::Mat &img)
{
    if (pre_load) {
        auto out_samples = random_choice(bg_imgs, 1);
        img = out_samples[0];
    } else {
        auto out_paths = random_choice(bg_paths, 1);
        auto bg_path = out_paths[0];
        get_cached_image(img, bg_path);
    }
}

// Make sure background size is large than input size
void BgManager::guard_bg_size(cv::Mat &out_image, const cv::Mat &in_image, const cv::Size &size)
{
    auto width = size.width;
    auto height = size.height;

    out_image = in_image.clone();
    auto scale = std::max((float) width / (float) in_image.cols,
                          (float) height / (float) in_image.rows);
    if (scale > 1.0f) {
        auto image_width = in_image.cols;
        auto image_height = in_image.rows;
        auto scaled_width = int(image_width * scale);
        auto scaled_height = int(image_height * scale);
        cv::resize(in_image, out_image, cv::Size(scaled_width, scaled_height));
    }
}
