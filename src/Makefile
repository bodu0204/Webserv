NAME    = webserv

SRCS    = accept_handler.cpp cgir_handler.cpp cgiw_handler.cpp \
		  echo_handler.cpp handler.cpp http_handler.cpp main.cpp \
		  mime.cpp config/config.cpp config/location_conf.cpp config/port_conf.cpp \
		  config/server_conf.cpp utils/_bracket.cpp utils/is_numstring.cpp \
		  utils/new_token.cpp utils/readfile.cpp utils/trim_meta.cpp utils/trim_sp.cpp

OBJS    = $(SRCS:%.cpp=%.o)

CXX     := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98


all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all