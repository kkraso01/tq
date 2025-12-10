#pragma once

#include "value.hpp"
#include <string>
#include <vector>

namespace tq {

class ToonParser {
public:
    static Value parse(const std::string& content);
    
private:
    // Context for parsing state
    struct Context {
        std::vector<std::string> lines;
        size_t current_line;
        int indent_size;
    };
    
    // Array header information
    struct ArrayHeader {
        std::string key;
        int length;
        char delimiter;
        std::vector<std::string> fields;
    };
    
    // Main parsing functions
    static Value parse_object_fields(Context& ctx, int base_depth);
    static Value parse_root_array(Context& ctx);
    static Value parse_inline_array(const std::string& values_str, int expected_length, char delimiter);
    static Value parse_tabular_array(Context& ctx, int item_depth, const ArrayHeader& header);
    static Value parse_list_array(Context& ctx, int item_depth, int expected_length);
    static Value parse_primitive(const std::string& str);
    
    // Helper functions
    static std::vector<std::string> split_lines(const std::string& content);
    static int get_line_depth(const std::string& line, int indent_size);
    static std::string get_line_content(const std::string& line);
    static void trim(std::string& s);
    
    // Array header parsing
    static bool is_array_header(const std::string& content);
    static ArrayHeader parse_array_header(const std::string& content);
    
    // String utilities
    static std::string parse_key(const std::string& key_str);
    static size_t find_unquoted_colon(const std::string& str);
    static std::vector<std::string> split_delimited(const std::string& str, char delimiter);
    static bool is_numeric(const std::string& str);
    static std::string unescape_string(const std::string& str);
};

} // namespace tq
