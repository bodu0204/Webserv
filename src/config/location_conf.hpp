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

#define AUTO_INDEX_FMT_0 "<html>\n\
<head>\n\
    <title>Index of "
#define AUTO_INDEX_FMT_1 "</title>\n\
    <style>\n\
        body {\n\
            font-family: \"Arial\", sans-serif;\n\
            background-color: #f9f9f9;\n\
            margin: 20px;\n\
        }\n\
        \n\
        h1 {\n\
            text-align: center;\n\
            color: #a1cff4;\n\
            font-size: 24px;\n\
            margin-top: 0;\n\
        }\n\
        \n\
        pre {\n\
            background-color: #fff;\n\
            padding: 10px;\n\
            border-radius: 10px;\n\
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);\n\
        }\n\
        \n\
        pre a {\n\
            text-decoration: none;\n\
            color: #67a8dd;\n\
            display: block;\n\
            margin-bottom: 10px;\n\
            font-weight: bold;\n\
            transition: color 0.3s ease;\n\
        }\n\
        \n\
        pre a:hover {\n\
            color: #e6007e;\n\
        }\n\
        \n\
        hr {\n\
            border: none;\n\
            border-top: 1px solid #eee;\n\
            margin-top: 20px;\n\
        }\n\
    </style>\n\
</head>\n\
<body>\n\
    <h1>Index of "
#define AUTO_INDEX_FMT_2 "</h1>\n\
    <hr>\n\
    <pre>\n\
    "
#define AUTO_INDEX_FMT_3 "<a href=\""
#define AUTO_INDEX_FMT_4 "\">"
#define AUTO_INDEX_FMT_5 "</a>\n\
    "
#define AUTO_INDEX_FMT_6 "</pre>\n\
    <hr>\n\
</body>\n\
</html>"


