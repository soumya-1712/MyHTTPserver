#ifndef MIMETYPES_H
#define MIMETYPES_H

#include <map>
#include <string>

namespace web {
    class MimeTypes {
    private:
        std::map<std::string, std::string> _mimeTypes;
        std::string _defaultMimeType;

    public:
        MimeTypes();

        std::string getMimeType(const std::string& extension) const; 
        bool loadFromFile(const std::string& filename);
        bool saveToFile(const std::string& filename);
        void loadDefaultMimeTypes();
        void addMimeType(const std::string& extension, const std::string& mimeType);
        void removeMimeType(const std::string& extension);

        const std::map<std::string, std::string>& data() const; 
    };
}

#endif