#include "fontmanager.h"
#include <cassert>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

#include <fmt/core.h>

namespace fs = boost::filesystem;

FontManager::FontManager(const std::string &font_dir,
                         const std::string &font_list_file,
                         const int min_font_size,
                         const int max_font_size)
{
    std::vector<std::string> font_extensions{".ttc", ".TTC", ".ttf", ".TTF", ".otf", ".OTF"};

    assert(min_font_size <= max_font_size);
    this->min_font_size = min_font_size;
    this->max_font_size = max_font_size;

    std::string font_name;
    std::vector<std::string> trimmed_font_list;
    if (!font_list_file.empty()) {
        std::ifstream file(font_list_file);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                boost::trim(line);
                if (!line.empty()) {
                    fs::path dir(font_dir);
                    fs::path filename(line);
                    fs::path full_path = dir / filename;
                    if (fs::exists(full_path)) {
                        trimmed_font_list.push_back(full_path.string());
                    } else {
                        BOOST_LOG_TRIVIAL(warning)
                            << fmt::format("Font file not exist: {}", full_path.string());
                    }
                }
            }

            if (trimmed_font_list.empty()) {
                BOOST_LOG_TRIVIAL(error)
                    << fmt::format("Font list file is empty: {}", font_list_file);
            }
        } else {
            BOOST_LOG_TRIVIAL(info)
                << fmt::format("Can not open font list file: {}", font_list_file);
        }
    } else {
        fs::path current_path(font_dir);
        if (!(fs::exists(current_path) && fs::is_directory(current_path))) {
            BOOST_LOG_TRIVIAL(warning)
                << fmt::format("Current path {} is not exists or is not a directory,", font_dir);
        }

        fs::recursive_directory_iterator dir_iter(current_path);
        for (auto &dir_entry : dir_iter) {
            if (fs::is_regular_file(dir_entry)
                && (std::find(font_extensions.begin(),
                              font_extensions.end(),
                              dir_entry.path().extension())
                    != font_extensions.end())) {
                std::string real_font_path = dir_entry.path().string();
                font_paths.push_back(real_font_path);
            }
        }
    }

    load_font_support_chars();
}

void FontManager::load_font_support_chars()
{
    /**/
}

void FontManager::load_ttfont(std::string &font_path, stbtt_fontinfo &font_info)
{
    FILE *font_file = fopen(font_path.c_str(), "rb");
    if (font_file == nullptr) {
        BOOST_LOG_TRIVIAL(error) << fmt::format("Can not open font file: {}", font_path);
    }
    fseek(font_file, 0, SEEK_END);
    auto size = ftell(font_file);
    fseek(font_file, 0, SEEK_SET);

    auto *font_buffer = (unsigned char *) calloc(size, sizeof(unsigned char));
    fread(font_buffer, size, 1, font_file);
    fclose(font_file);

    if (!stbtt_InitFont(&font_info, font_buffer, 0)) {
        BOOST_LOG_TRIVIAL(error) << fmt::format("stb init font failed");
    }

    free(font_buffer);
}
