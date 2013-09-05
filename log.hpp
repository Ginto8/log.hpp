#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>
#include <memory>
#include <mutex>

template <typename charT = char>
class basic_logger {
public:
    enum log_level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    
private:
    log_level _minLevel;
    bool _color;
    std::basic_ostream<charT>& _os;

    template<class T>
    void _log(const T& x) {
        _os << x;
    }

    template <class T,class... Ts>
    void _log(const T& x,Ts... args) {
        _log(x);
        _log(args...);
    }

public:
    basic_logger(log_level minLevel = INFO,
        bool color = false,
        std::basic_ostream<charT>& os = std::cout) :
      _minLevel(minLevel),_color(color),_os(os) {}

    log_level minLevel() const {
        return _minLevel;
    }

    template <class... Ts>
    basic_logger& operator()(log_level level,Ts... args) {
        if(level >= (int)_minLevel) {
            std::string prefix =
                level == DEBUG   ?   ">>> [DEBUG] " :
                level == INFO    ?   ">> [INFO] "   :
                level == WARNING ? std::string(_color ? "\e[33m" : "")
                                   + "> [WARNING] " :
              /*level == ERROR  */ std::string(_color ? "\e[31m" : "")
                                   + " [ERROR] ";
            _log(prefix,args...);
            if(_color) {
                _log("\e[37m");
            }
            _os.flush();
        }
        return *this;
    }
    template <class... Ts>
    basic_logger& debug(Ts... args) {
        return operator()(DEBUG,args...);
    }
    template <class... Ts>
    basic_logger& debugln(Ts... args) {
        return debug(args...,"\n");
    }
    template <class... Ts>
    basic_logger& info(Ts... args) {
        return operator()(INFO,args...);
    }
    template <class... Ts>
    basic_logger& infoln(Ts... args) {
        return info(args...,"\n");
    }
    template <class... Ts>
    basic_logger& warning(Ts... args) {
        return operator()(WARNING,args...);
    }
    template <class... Ts>
    basic_logger& warningln(Ts... args) {
        return warning(args...,"\n");
    }
    template <class... Ts>
    basic_logger& error(Ts... args) {
        return operator()(ERROR,args...);
    }
    template <class... Ts>
    basic_logger& errorln(Ts... args) {
        return error(args...,"\n");
    }
};

using logger = basic_logger<>;

#endif
