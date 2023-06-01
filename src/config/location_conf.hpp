#pragma once
#include <string>
#include <vector>

class location_conf{
private:
	bool is_faile;
	std::string _root;
	std::string _index;
	std::string _cgi;
	bool _autoindex;
	location_conf();
public:
	location_conf(std::string);
	location_conf(const location_conf &);
	const location_conf &operator=(const location_conf &);
	bool faile() const;
	std::string root() const;
	std::string index() const;
	std::string cgi() const;
	bool autoindex() const;
	~location_conf();

	static const location_conf error;
};

#define AUTO_INDEX_FMT_0 "<html>\
<head>\
    <title>Index of "
#define AUTO_INDEX_FMT_1 "</title>\
    <style>\
        body {\
            font-family: \"Arial\", sans-serif;\
            background-color: #f9f9f9;\
            margin: 20px;\
        }\
        \
        h1 {\
            text-align: center;\
            color: #a1cff4;\
            font-size: 24px;\
            margin-top: 0;\
        }\
        \
        pre {\
            background-color: #fff;\
            padding: 10px;\
            border-radius: 10px;\
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);\
        }\
        \
        pre a {\
            text-decoration: none;\
            color: #67a8dd;\
            display: block;\
            margin-bottom: 10px;\
            font-weight: bold;\
            transition: color 0.3s ease;\
        }\
        \
        pre a:hover {\
            color: #e6007e;\
        }\
        \
        hr {\
            border: none;\
            border-top: 1px solid #eee;\
            margin-top: 20px;\
        }\
    </style>\
</head>\
<body>\
    <h1>Index of "
#define AUTO_INDEX_FMT_2 "</h1>\
    <hr>\
    <pre>\
    <a href=\"../\">../</a>\
    "
#define AUTO_INDEX_FMT_3 "<a href=\""
#define AUTO_INDEX_FMT_4 "\">"
#define AUTO_INDEX_FMT_5 "</a>\
    "
#define AUTO_INDEX_FMT_6 "</pre>\
    <hr>\
</body>\
</html>"


