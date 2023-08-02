#include <map>
#include <string>
#include <cstdint>

namespace game::util::ini
{

class IniReader
{
public:
    // Construct INIReader and parse given filename. See ini.h for more info
    // about the parsing.
    explicit IniReader(const std::string& filename);

    // Construct INIReader and parse given buffer. See ini.h for more info
    // about the parsing.
    explicit IniReader(const char *buffer, size_t buffer_size);

    // Return the result of ini_parse(), i.e., 0 on success, line number of
    // first error on parse error, or -1 on file open error.
    int ParseError() const;

    // Get a string value from INI file, returning default_value if not found.
    std::string Get(const std::string& section, const std::string& name,
                    const std::string& default_value) const;

    // Get a string value from INI file, returning default_value if not found,
    // empty, or contains only whitespace.
    std::string GetString(const std::string& section, const std::string& name,
                    const std::string& default_value) const;

    // Get an integer (long) value from INI file, returning default_value if
    // not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
    long GetInteger(const std::string& section, const std::string& name, long default_value) const;

    // Get a 64-bit integer (int64_t) value from INI file, returning default_value if
    // not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
    int64_t GetInteger64(const std::string& section, const std::string& name, int64_t default_value) const;

    // Get an unsigned integer (unsigned long) value from INI file, returning default_value if
    // not found or not a valid unsigned integer (decimal "1234", or hex "0x4d2").
    unsigned long GetUnsigned(const std::string& section, const std::string& name, unsigned long default_value) const;

    // Get an unsigned 64-bit integer (uint64_t) value from INI file, returning default_value if
    // not found or not a valid unsigned integer (decimal "1234", or hex "0x4d2").
    uint64_t GetUnsigned64(const std::string& section, const std::string& name, uint64_t default_value) const;

    // Get a real (floating point double) value from INI file, returning
    // default_value if not found or not a valid floating point value
    // according to strtod().
    double GetReal(const std::string& section, const std::string& name, double default_value) const;

    // Get a boolean value from INI file, returning default_value if not found or if
    // not a valid true/false value. Valid true values are "true", "yes", "on", "1",
    // and valid false values are "false", "no", "off", "0" (not case sensitive).
    bool GetBoolean(const std::string& section, const std::string& name, bool default_value) const;

    // Return true if the given section exists (section must contain at least
    // one name=value pair).
    bool HasSection(const std::string& section) const;

    // Return true if a value exists with the given section and field names.
    bool HasValue(const std::string& section, const std::string& name) const;

private:
    int _error;
    std::map<std::string, std::string> _values;
    static std::string MakeKey(const std::string& section, const std::string& name);
    static int ValueHandler(void* user, const char* section, const char* name,
                            const char* value);
};

}