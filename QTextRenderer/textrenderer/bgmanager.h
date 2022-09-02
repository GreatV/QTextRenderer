#ifndef BGMANAGER_H
#define BGMANAGER_H

#include <string>
#include <vector>

#include <boost/compute/detail/lru_cache.hpp>
#include <opencv2/opencv.hpp>

class BgManager
{
public:
    BgManager(const std::string &bg_dir, bool pre_load = true);

    void get_image(cv::Mat &img);
    void guard_bg_size(cv::Mat &out_image, const cv::Mat &in_image, const cv::Size &size);

private:
    std::vector<std::string> bg_paths{};
    std::vector<cv::Mat> bg_imgs{};
    bool pre_load;

    size_t max_size = 32;
    boost::compute::detail::lru_cache<std::string, cv::Mat> bg_image_cache{max_size};

private:
    void get_cached_image(cv::Mat &img, const std::string &bg_path);
};

#endif // BGMANAGER_H
