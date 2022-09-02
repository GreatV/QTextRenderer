#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <map>
#include <set>
#include <string>
#include <vector>

class FontManager
{
public:
    FontManager(const std::string &font_dir,
                const std::string &font_list_file,
                int min_font_size,
                int max_font_size);

private:
    int min_font_size = 0;
    int max_font_size = 0;
    std::vector<std::string> font_paths{};
    std::map<std::string, std::set<unsigned char>> font_support_chars_cache{};
    std::map<std::string, std::set<unsigned char>> font_support_chars_intersection_with_chars{};

private:
    void load_font_support_chars();
};

#endif // FONTMANAGER_H
