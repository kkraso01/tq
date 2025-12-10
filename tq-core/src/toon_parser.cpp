#include "tq/toon_parser.hpp"
#include <sstream>
#include <cmath>
#include <limits>

namespace tq {

// Parse a complete TOON document
Value ToonParser::parse(const std::string& content) {
    std::vector<std::string> lines = split_lines(content);
    if (lines.empty()) {
        return Value(std::map<std::string, Value>{});  // Empty input is empty object
    }
    
    Context ctx;
    ctx.lines = lines;
    ctx.current_line = 0;
    ctx.indent_size = 2;  // Default indent
    
    // Check if root is an array
    if (!lines.empty()) {
        std::string first_content_line = get_line_content(lines[0]);
        if (is_array_header(first_content_line)) {
            return parse_root_array(ctx);
        }
    }
    
    // Check if root is single primitive
    if (lines.size() == 1) {
        std::string line_content = get_line_content(lines[0]);
        if (line_content.find(':') == std::string::npos) {
            return parse_primitive(line_content);
        }
    }
    
    // Root is an object
    return parse_object_fields(ctx, 0);
}

// Parse object fields at a given depth level
Value ToonParser::parse_object_fields(Context& ctx, int base_depth) {
    std::map<std::string, Value> obj;
    
    while (ctx.current_line < ctx.lines.size()) {
        int depth = get_line_depth(ctx.lines[ctx.current_line], ctx.indent_size);
        
        // Stop if we've moved to a shallower depth
        if (depth < base_depth) {
            break;
        }
        
        // Skip lines that are too deep (shouldn't happen in well-formed TOON)
        if (depth > base_depth) {
            break;
        }
        
        std::string content = get_line_content(ctx.lines[ctx.current_line]);
        if (content.empty() || content[0] == '-') {
            break;  // Not an object field
        }
        
        // Parse the key-value pair
        size_t colon_pos = find_unquoted_colon(content);
        if (colon_pos == std::string::npos) {
            break;  // Not a valid key-value line
        }
        
        std::string key_part = content.substr(0, colon_pos);
        std::string value_part = content.substr(colon_pos + 1);
        trim(key_part);
        trim(value_part);
        
        // Check for array header: key[n]: or key[n]{fields}:
        if (is_array_header(content)) {
            ArrayHeader header = parse_array_header(content);
            ctx.current_line++;
            
            Value array_value(std::vector<Value>{});
            if (!value_part.empty()) {
                // Inline primitive array
                array_value = parse_inline_array(value_part, header.length, header.delimiter);
            } else if (!header.fields.empty()) {
                // Tabular array
                array_value = parse_tabular_array(ctx, base_depth + 1, header);
            } else {
                // List array
                array_value = parse_list_array(ctx, base_depth + 1, header.length);
            }
            
            obj[header.key] = std::move(array_value);
        } else {
            // Regular key-value
            std::string key = parse_key(key_part);
            ctx.current_line++;
            
            if (value_part.empty()) {
                // Nested object
                Value nested = parse_object_fields(ctx, base_depth + 1);
                obj[key] = std::move(nested);
            } else {
                // Inline primitive value
                obj[key] = parse_primitive(value_part);
            }
        }
    }
    
    return Value(std::move(obj));
}

// Parse root-level array
Value ToonParser::parse_root_array(Context& ctx) {
    std::string content = get_line_content(ctx.lines[0]);
    ArrayHeader header = parse_array_header(content);
    ctx.current_line = 1;
    
    Value array_value;
    
    // Check for inline values
    size_t colon_pos = find_unquoted_colon(content);
    if (colon_pos != std::string::npos) {
        std::string after_colon = content.substr(colon_pos + 1);
        trim(after_colon);
        if (!after_colon.empty()) {
            array_value = parse_inline_array(after_colon, header.length, header.delimiter);
        }
    }
    
    if (array_value.is_null()) {
        if (!header.fields.empty()) {
            array_value = parse_tabular_array(ctx, 1, header);
        } else {
            array_value = parse_list_array(ctx, 1, header.length);
        }
    }
    
    // If the array header has a key, wrap it in an object
    if (!header.key.empty()) {
        std::map<std::string, Value> obj;
        obj[header.key] = std::move(array_value);
        return Value(std::move(obj));
    }
    
    return array_value;
}

// Parse inline primitive array (e.g., [3]: 1, 2, 3)
Value ToonParser::parse_inline_array(const std::string& values_str, int expected_length, char delimiter) {
    std::vector<Value> items;
    std::vector<std::string> parts = split_delimited(values_str, delimiter);
    
    for (const auto& part : parts) {
        std::string trimmed = part;
        trim(trimmed);
        if (!trimmed.empty()) {
            items.push_back(parse_primitive(trimmed));
        }
    }
    
    return Value(std::move(items));
}

// Parse tabular array (rows with delimited values)
Value ToonParser::parse_tabular_array(Context& ctx, int item_depth, const ArrayHeader& header) {
    std::vector<Value> items;
    
    while (ctx.current_line < ctx.lines.size() && items.size() < static_cast<size_t>(header.length)) {
        int depth = get_line_depth(ctx.lines[ctx.current_line], ctx.indent_size);
        
        if (depth < item_depth) {
            break;
        }
        
        if (depth == item_depth) {
            std::string content = get_line_content(ctx.lines[ctx.current_line]);
            std::vector<std::string> values = split_delimited(content, header.delimiter);
            
            std::map<std::string, Value> obj;
            for (size_t i = 0; i < header.fields.size() && i < values.size(); i++) {
                std::string val = values[i];
                trim(val);
                obj[header.fields[i]] = parse_primitive(val);
            }
            
            items.push_back(Value(std::move(obj)));
            ctx.current_line++;
        } else {
            break;
        }
    }
    
    return Value(std::move(items));
}

// Parse list array (items starting with -)
Value ToonParser::parse_list_array(Context& ctx, int item_depth, int expected_length) {
    std::vector<Value> items;
    
    while (ctx.current_line < ctx.lines.size() && items.size() < static_cast<size_t>(expected_length)) {
        int depth = get_line_depth(ctx.lines[ctx.current_line], ctx.indent_size);
        
        if (depth < item_depth) {
            break;
        }
        
        if (depth == item_depth) {
            std::string content = get_line_content(ctx.lines[ctx.current_line]);
            
            if (!content.empty() && content[0] == '-') {
                ctx.current_line++;
                
                // Get content after the dash
                std::string after_dash = content.substr(1);
                trim(after_dash);
                
                if (after_dash.empty()) {
                    // Empty object
                    items.push_back(Value(std::map<std::string, Value>{}));
                } else if (is_array_header(after_dash)) {
                    // Array item
                    ArrayHeader header = parse_array_header(after_dash);
                    Value arr(std::vector<Value>{});
                    
                    size_t colon_pos = find_unquoted_colon(after_dash);
                    if (colon_pos != std::string::npos) {
                        std::string after_colon = after_dash.substr(colon_pos + 1);
                        trim(after_colon);
                        if (!after_colon.empty()) {
                            arr = parse_inline_array(after_colon, header.length, header.delimiter);
                        }
                    }
                    
                    if (arr.is_null()) {
                        if (!header.fields.empty()) {
                            arr = parse_tabular_array(ctx, item_depth + 1, header);
                        } else {
                            arr = parse_list_array(ctx, item_depth + 1, header.length);
                        }
                    }
                    
                    items.push_back(std::move(arr));
                } else if (after_dash.find(':') != std::string::npos) {
                    // Object item starting with first field on same line
                    std::map<std::string, Value> obj;
                    
                    size_t colon_pos = find_unquoted_colon(after_dash);
                    std::string key = after_dash.substr(0, colon_pos);
                    std::string val = after_dash.substr(colon_pos + 1);
                    trim(key);
                    trim(val);
                    
                    key = parse_key(key);
                    obj[key] = parse_primitive(val);
                    
                    // Parse remaining fields
                    while (ctx.current_line < ctx.lines.size()) {
                        int field_depth = get_line_depth(ctx.lines[ctx.current_line], ctx.indent_size);
                        if (field_depth <= item_depth) {
                            break;
                        }
                        
                        std::string field_content = get_line_content(ctx.lines[ctx.current_line]);
                        if (field_content.empty() || field_content[0] == '-') {
                            break;
                        }
                        
                        size_t field_colon = find_unquoted_colon(field_content);
                        if (field_colon == std::string::npos) {
                            break;
                        }
                        
                        std::string field_key = field_content.substr(0, field_colon);
                        std::string field_val = field_content.substr(field_colon + 1);
                        trim(field_key);
                        trim(field_val);
                        
                        field_key = parse_key(field_key);
                        obj[field_key] = parse_primitive(field_val);
                        ctx.current_line++;
                    }
                    
                    items.push_back(Value(std::move(obj)));
                } else {
                    // Primitive item
                    items.push_back(parse_primitive(after_dash));
                }
            } else {
                break;
            }
        } else {
            break;
        }
    }
    
    return Value(std::move(items));
}

// Parse primitive value from string
Value ToonParser::parse_primitive(const std::string& str) {
    std::string s = str;
    trim(s);
    
    if (s.empty()) {
        return Value("");
    }
    
    // Boolean and null literals
    if (s == "true") return Value(true);
    if (s == "false") return Value(false);
    if (s == "null") return Value();  // null value
    
    // Quoted string
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        return Value(unescape_string(s.substr(1, s.size() - 2)));
    }
    
    // Try to parse as number
    if (is_numeric(s)) {
        if (s.find('.') != std::string::npos || s.find('e') != std::string::npos || s.find('E') != std::string::npos) {
            // Floating point
            try {
                double d = std::stod(s);
                // Normalize -0 to 0
                if (d == 0.0 && std::signbit(d)) {
                    d = 0.0;
                }
                return Value(d);
            } catch (...) {
                return Value(s);
            }
        } else {
            // Integer
            try {
                int i = std::stoi(s);
                return Value(i);
            } catch (...) {
                return Value(s);
            }
        }
    }
    
    // Unquoted string
    return Value(s);
}

// Utility functions

std::vector<std::string> ToonParser::split_lines(const std::string& content) {
    std::vector<std::string> lines;
    std::string line;
    std::istringstream stream(content);
    
    while (std::getline(stream, line)) {
        // Remove \r if present (Windows line endings)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    
    return lines;
}

int ToonParser::get_line_depth(const std::string& line, int indent_size) {
    int spaces = 0;
    for (char c : line) {
        if (c == ' ') {
            spaces++;
        } else {
            break;
        }
    }
    return spaces / indent_size;
}

std::string ToonParser::get_line_content(const std::string& line) {
    size_t start = 0;
    while (start < line.size() && line[start] == ' ') {
        start++;
    }
    return line.substr(start);
}

void ToonParser::trim(std::string& s) {
    // Trim left
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    // Trim right
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

bool ToonParser::is_array_header(const std::string& content) {
    // Look for pattern: [number] or key[number]
    size_t bracket_pos = content.find('[');
    if (bracket_pos == std::string::npos) {
        return false;
    }
    
    size_t close_bracket = content.find(']', bracket_pos);
    if (close_bracket == std::string::npos) {
        return false;
    }
    
    // Must have colon after bracket section
    return content.find(':', close_bracket) != std::string::npos;
}

ToonParser::ArrayHeader ToonParser::parse_array_header(const std::string& content) {
    ArrayHeader header;
    header.delimiter = ',';  // Default
    
    size_t bracket_start = content.find('[');
    size_t bracket_end = content.find(']', bracket_start);
    
    // Extract key if present
    if (bracket_start > 0) {
        std::string key_part = content.substr(0, bracket_start);
        trim(key_part);
        header.key = parse_key(key_part);
    }
    
    // Extract length from [N]
    std::string bracket_content = content.substr(bracket_start + 1, bracket_end - bracket_start - 1);
    
    // Check for delimiter suffix
    if (!bracket_content.empty()) {
        char last = bracket_content.back();
        if (last == '\t') {
            header.delimiter = '\t';
            bracket_content.pop_back();
        } else if (last == '|') {
            header.delimiter = '|';
            bracket_content.pop_back();
        }
    }
    
    trim(bracket_content);
    header.length = std::stoi(bracket_content);
    
    // Check for field names {field1,field2}
    size_t brace_start = content.find('{', bracket_end);
    size_t brace_end = content.find('}', brace_start);
    
    if (brace_start != std::string::npos && brace_end != std::string::npos) {
        std::string fields_content = content.substr(brace_start + 1, brace_end - brace_start - 1);
        header.fields = split_delimited(fields_content, header.delimiter);
        
        // Trim and unquote field names
        for (auto& field : header.fields) {
            trim(field);
            if (field.size() >= 2 && field.front() == '"' && field.back() == '"') {
                field = field.substr(1, field.size() - 2);
            }
        }
    }
    
    return header;
}

std::string ToonParser::parse_key(const std::string& key_str) {
    std::string k = key_str;
    trim(k);
    
    // Remove quotes if present
    if (k.size() >= 2 && k.front() == '"' && k.back() == '"') {
        return unescape_string(k.substr(1, k.size() - 2));
    }
    
    return k;
}

size_t ToonParser::find_unquoted_colon(const std::string& str) {
    bool in_quotes = false;
    bool escaped = false;
    
    for (size_t i = 0; i < str.size(); i++) {
        if (escaped) {
            escaped = false;
            continue;
        }
        
        if (str[i] == '\\' && in_quotes) {
            escaped = true;
            continue;
        }
        
        if (str[i] == '"') {
            in_quotes = !in_quotes;
            continue;
        }
        
        if (str[i] == ':' && !in_quotes) {
            return i;
        }
    }
    
    return std::string::npos;
}

std::vector<std::string> ToonParser::split_delimited(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::string current;
    bool in_quotes = false;
    bool escaped = false;
    
    for (size_t i = 0; i < str.size(); i++) {
        char c = str[i];
        
        if (escaped) {
            current += c;
            escaped = false;
            continue;
        }
        
        if (c == '\\' && in_quotes) {
            current += c;
            escaped = true;
            continue;
        }
        
        if (c == '"') {
            in_quotes = !in_quotes;
            current += c;
            continue;
        }
        
        if (c == delimiter && !in_quotes) {
            result.push_back(current);
            current.clear();
            continue;
        }
        
        current += c;
    }
    
    if (!current.empty() || !result.empty()) {
        result.push_back(current);
    }
    
    return result;
}

bool ToonParser::is_numeric(const std::string& str) {
    if (str.empty()) return false;
    
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') {
        start = 1;
    }
    
    if (start >= str.size()) return false;
    
    bool has_digit = false;
    bool has_dot = false;
    bool has_e = false;
    
    for (size_t i = start; i < str.size(); i++) {
        char c = str[i];
        
        if (c >= '0' && c <= '9') {
            has_digit = true;
        } else if (c == '.') {
            if (has_dot || has_e) return false;
            has_dot = true;
        } else if (c == 'e' || c == 'E') {
            if (has_e || !has_digit) return false;
            has_e = true;
            has_digit = false;  // Need digit after e
            if (i + 1 < str.size() && (str[i + 1] == '+' || str[i + 1] == '-')) {
                i++;  // Skip sign after e
            }
        } else {
            return false;
        }
    }
    
    return has_digit;
}

std::string ToonParser::unescape_string(const std::string& str) {
    std::string result;
    bool escaped = false;
    
    for (char c : str) {
        if (escaped) {
            switch (c) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                case '\\': result += '\\'; break;
                case '"': result += '"'; break;
                default: 
                    result += '\\';
                    result += c;
                    break;
            }
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else {
            result += c;
        }
    }
    
    return result;
}

} // namespace tq
