#pragma once

#include <unordered_map>
#include <stack>
#include <array>
#include <ostream>
#include <sstream>
#include <vector>

#ifdef __unix__
    #include <unistd.h>
#elif defined(_WIN32)

#endif

namespace iro {
    enum effect_type {
        foreground_color,
        background_color,
        font_weight,
        underlinedness, // I know this isn't a real word
        blink,

        number_of_effect_types
    };

    namespace color {
        enum color {
            black,
            red,
            green,
            yellow,
            blue,
            magenta,
            cyan,
            white,

            bright_black,
            bright_red,
            bright_green,
            bright_yellow,
            bright_blue,
            bright_magenta,
            bright_cyan,
            bright_white
        };
    }


    class effect;

    namespace detail {
        effect create(const char* code, effect_type type) noexcept;
    }

    class effect {
        const char* code_;
        effect_type type_;

        effect(const char* code, effect_type type) noexcept;

        friend effect detail::create(const char* code, effect_type type) noexcept; // Make this private so that the user can't construct invalid effects
        friend class effect_set;
        friend class persist;
    };

    ///  effects  ///
        ///  foreground colors  ///
            const effect black          = detail::create("\x1b[30m", foreground_color);
            const effect red            = detail::create("\x1b[31m", foreground_color);
            const effect green          = detail::create("\x1b[32m", foreground_color);
            const effect yellow         = detail::create("\x1b[33m", foreground_color);
            const effect blue           = detail::create("\x1b[34m", foreground_color);
            const effect magenta        = detail::create("\x1b[35m", foreground_color);
            const effect cyan           = detail::create("\x1b[36m", foreground_color);
            const effect white          = detail::create("\x1b[37m", foreground_color);

            const effect bright_black   = detail::create("\x1b[90m", foreground_color);
                const effect& gray = bright_black;
                const effect& grey = bright_black; // for the british people
            const effect bright_red     = detail::create("\x1b[91m", foreground_color);
            const effect bright_green   = detail::create("\x1b[92m", foreground_color);
            const effect bright_yellow  = detail::create("\x1b[93m", foreground_color);
            const effect bright_blue    = detail::create("\x1b[94m", foreground_color);
            const effect bright_magenta = detail::create("\x1b[95m", foreground_color);
            const effect bright_cyan    = detail::create("\x1b[96m", foreground_color);
            const effect bright_white   = detail::create("\x1b[97m", foreground_color);
        /// /foreground colors  ///

        ///  background colors  ///
            const effect background_black          = detail::create("\x1b[40m", background_color);
            const effect background_red            = detail::create("\x1b[41m", background_color);
            const effect background_green          = detail::create("\x1b[42m", background_color);
            const effect background_yellow         = detail::create("\x1b[43m", background_color);
            const effect background_blue           = detail::create("\x1b[44m", background_color);
            const effect background_magenta        = detail::create("\x1b[45m", background_color);
            const effect background_cyan           = detail::create("\x1b[46m", background_color);
            const effect background_white          = detail::create("\x1b[47m", background_color);

            const effect background_bright_black   = detail::create("\x1b[100m", background_color);
                const effect& background_gray = bright_black;
                const effect& background_grey = bright_black; // for the british people
            const effect background_bright_red     = detail::create("\x1b[101m", background_color);
            const effect background_bright_green   = detail::create("\x1b[102m", background_color);
            const effect background_bright_yellow  = detail::create("\x1b[103m", background_color);
            const effect background_bright_blue    = detail::create("\x1b[104m", background_color);
            const effect background_bright_magenta = detail::create("\x1b[105m", background_color);
            const effect background_bright_cyan    = detail::create("\x1b[106m", background_color);
            const effect background_bright_white   = detail::create("\x1b[107m", background_color);
        /// /background colors  ///

        ///  font weight  ///
            const effect bold          = detail::create("\x1b[1m",  font_weight);
            const effect faint         = detail::create("\x1b[2m",  font_weight);
            const effect normal_weight = detail::create("\x1b[22m", font_weight);
        /// /font weight  ///

        /// underline  ///
            const effect underlined     = detail::create("\x1b[4m",  underlinedness);
                const effect& underline = underlined; // I can't decide whether to call this one underlined or underlineD, so I'll just let both be valid
            const effect not_underlined = detail::create("\x1b[24m", underlinedness);
        /// /underline  ///

        ///  blink  ///
            const effect blinking     = detail::create("\x1b[5m",  blink);
            const effect not_blinking = detail::create("\x1b[25m", blink);
        /// /blink  ///

    /// /effects  ///

    class effect_set {
        std::array<const char*, number_of_effect_types> type_to_code_;

        friend class persist;
        friend class effect_string;

    public:
        effect_set();

        effect_set(const effect& e);
        effect_set(const effect& e, const effect& e2);

        effect_set   operator| (const effect& rhs) const&;
        effect_set&& operator| (const effect& rhs)&&;
        effect_set&  operator|=(const effect& rhs)&;
        effect_set&& operator|=(const effect& rhs)&&;

        effect_set   operator| (const effect_set& rhs) const&;
        effect_set&& operator| (const effect_set& rhs)&&;
        effect_set&  operator|=(const effect_set& rhs)&;
        effect_set&& operator|=(const effect_set& rhs)&&;
    };

    effect_set operator|(const effect& e1, const effect& e2);


    namespace detail {
        unsigned push_effect(std::ostream& stream, effect_type type, const char* code);
        unsigned push_empty_effect(std::ostream& stream, effect_type type);
        void pop_effect(std::ostream& stream, effect_type type);
        void set(std::ostream& stream, effect_type type, unsigned index, const char* code);
        void set_top(std::ostream& stream, effect_type type, const char* code);
        void reapply_top(std::ostream& stream, effect_type type);
    }


    class effect_string;

    class persist {
        std::array<unsigned, number_of_effect_types> effect_locations_in_stack_; // 0 is a special value that indicates an inactive effect
        std::ostream* stream_ = nullptr;                                         // 0 is always available because index 0 (the first element in the stack) is always the default effect for a given effect type

        friend persist operator<<(std::ostream&, const effect_string&);
        friend persist&& operator<<(persist&, const effect_string&);

    public:
        persist();

        persist(std::ostream& os, const effect& e);
        persist(std::ostream& os, const effect_set& e);

        persist(persist&& other);
        persist& operator=(persist&& rhs);

        persist(const persist&) = delete;
        persist& operator=(const persist&) = delete;

        persist&& operator<<(const effect& e);
        persist&& operator<<(const effect_set& es);

        template<typename T, std::enable_if_t<(!std::is_same<T, effect>::value) && (!std::is_same<T, effect_set>::value), bool> = true>
        persist&& operator<<(const T& arg) {
            *stream_ << arg;
            return std::move(*this);
        }

        ~persist();
    };

    persist operator<<(std::ostream& stream, const effect& e);
    persist operator<<(std::ostream& stream, const effect_set& e);


    class effect_string {
        struct string_and_effects {
            std::string string;
            std::array<bool, number_of_effect_types> active_effects_;

            inline string_and_effects() {
                active_effects_.fill(false);
            }
        };
        std::vector<string_and_effects> strings_;

        template<typename T, typename...Ts>
        void init_(std::stringstream& stream, const T& arg) {
            stream << arg;
        }

        template<typename T, typename...Ts, std::enable_if_t<(sizeof...(Ts) > 0), bool> = true>
        void init_(std::stringstream& stream, const T& arg, const Ts&...args) {
            stream << arg;
        }

        friend persist   operator<<(std::ostream&, const effect_string&);
        friend persist&& operator<<(persist&,      const effect_string&);

        string_and_effects& back_();
        const string_and_effects& back_() const;

    public:
        template<typename T, typename...Ts>
        effect_string(const effect_set& effects, const T& arg, const Ts&...args) {
            std::stringstream stream; // probably really slow
            for(unsigned i = 0; i < effects.type_to_code_.size(); ++i) {
                const auto& e = effects.type_to_code_[i];
                if(e) {
                    strings_.resize(1);
                    stream << e;
                    strings_.back().active_effects_[i] = true;
                }
            }
            init_(stream, arg, args...);
            strings_.back().string = stream.str();
        }

        template<typename T, std::enable_if_t<(!std::is_same<T, effect_string>::value), bool> = true>
        effect_string& operator<<(T& arg) {
            back_() += arg;
            return *this;
        }
        effect_string& operator<<(const effect_string& arg);
        effect_string& operator<<(effect_string&& arg);

        template<typename T, std::enable_if_t<(!std::is_same<T, effect_string>::value), bool> = true>
        effect_string& operator+=(T& arg) {
            back_() += arg;
            return *this;
        }
        effect_string& operator+=(const effect_string& arg);
        effect_string& operator+=(effect_string&& arg);

        template<typename T>
        effect_string operator+(T& arg) {
            auto ret = *this;
            ret += arg;
            return ret;
        }
    };

    persist   operator<<(std::ostream& os, const effect_string& es);
    persist&& operator<<(persist& p,       const effect_string& es);

    #ifdef IRO_IMPL
        effect::effect(const char* code, effect_type type) noexcept : code_(code), type_(type) {}


        effect_set::effect_set() {
            type_to_code_.fill(nullptr);
        }

        effect_set::effect_set(const effect& e) {
            type_to_code_.fill(nullptr);
            type_to_code_[e.type_] = e.code_;
        }

        effect_set::effect_set(const effect& e1, const effect& e2) {
            type_to_code_.fill(nullptr);
            type_to_code_[e1.type_] = e1.code_;
            type_to_code_[e2.type_] = e2.code_;
        }

        effect_set effect_set::operator|(const effect& rhs) const& {
            effect_set ret = *this;
            ret.type_to_code_[rhs.type_] = rhs.code_;

            return ret;
        }

        effect_set&& effect_set::operator|(const effect& rhs)&& {
            return std::move(*this |= rhs);
        }

        effect_set& effect_set::operator|=(const effect& rhs)& {
            type_to_code_[rhs.type_] = rhs.code_;

            return *this;
        }

        effect_set&& effect_set::operator|=(const effect& rhs)&& {
            return std::move(*this|=rhs);
        }

        effect_set effect_set::operator|(const effect_set& rhs) const& {
            effect_set ret;
            ret |= rhs;
            return ret;
        }

        effect_set&& effect_set::operator|(const effect_set& rhs)&& {
            return std::move(*this |= rhs);
        }

        effect_set& effect_set::operator|=(const effect_set& rhs)& {
            for(unsigned i = 0; i < rhs.type_to_code_.size(); ++i) {
                const auto& e = rhs.type_to_code_[i];
                if(e) {
                    type_to_code_[i] = e;
                }
            }

            return *this;
        }

        effect_set&& effect_set::operator|=(const effect_set& rhs)&& {
            return std::move(*this |= rhs);
        }

        effect_set operator|(const effect& e1, const effect& e2) {
            return {e1, e2};
        }


        persist::persist() {
            for(unsigned i = 0; i < effect_locations_in_stack_.size(); ++i) {
                effect_locations_in_stack_[i] = detail::push_empty_effect(*stream_, static_cast<effect_type>(i));
            }
        }

        persist::persist(std::ostream& os, const effect& e) : stream_(&os) {
            for(unsigned i = 0; i < effect_locations_in_stack_.size(); ++i) {
                effect_locations_in_stack_[i] = detail::push_empty_effect(*stream_, static_cast<effect_type>(i));
            }
            detail::set(os, e.type_, effect_locations_in_stack_[e.type_], e.code_);
        }

        persist::persist(std::ostream& os, const effect_set& e) : stream_(&os) {
            for(unsigned i = 0; i < effect_locations_in_stack_.size(); ++i) {
                effect_locations_in_stack_[i] = detail::push_empty_effect(*stream_, static_cast<effect_type>(i));
            }

            for(unsigned i = 0; i < e.type_to_code_.size(); ++i) {
                auto& el =  e.type_to_code_[i];
                if(el) {
                    auto type = static_cast<effect_type>(i);
                    detail::set(os, type, effect_locations_in_stack_[type], el);
                }
            }
        }

        persist::persist(persist&& other) {
            stream_ = other.stream_;

            effect_locations_in_stack_ = other.effect_locations_in_stack_;
            other.effect_locations_in_stack_.fill(0);
        }

        persist& persist::operator=(persist&& rhs) {
            stream_ = rhs.stream_;

            std::swap(effect_locations_in_stack_, rhs.effect_locations_in_stack_);

            return *this;
        }

        persist&& persist::operator<<(const effect& e) {
            detail::set(*stream_, e.type_, effect_locations_in_stack_[e.type_], e.code_);
            /*if(effect_locations_in_stack_[e.type_]) {
                detail::set_top(*stream_, e.type_, e.code_);
            }
            else {
                effect_locations_in_stack_[e.type_] = detail::push_effect(*stream_, e.type_, e.code_);
            }*/
            return std::move(*this);
        }

        persist&& persist::operator<<(const effect_set& es) {
            for(unsigned i = 0; i < es.type_to_code_.size(); ++i) {
                if(es.type_to_code_[i]) {
                    auto type = static_cast<effect_type>(i);
                    detail::set(*stream_, type, effect_locations_in_stack_[type], es.type_to_code_[i]);
                }
            }

            return std::move(*this);
        }

        persist operator<<(std::ostream& stream, const effect& e) {
            return {stream, e};
        }

        persist operator<<(std::ostream& stream, const effect_set& e) {
            return {stream, e};
        }

        persist::~persist() {
            for(unsigned i = 0; i < effect_locations_in_stack_.size(); ++i) {
                if(effect_locations_in_stack_[i]) {
                    detail::pop_effect(*stream_, static_cast<effect_type>(i));
                }
            }
        }


        effect_string::string_and_effects& effect_string::back_() {
            if(!strings_.size()) {
                strings_.push_back({});
            }
            return strings_.back();
        }

        const effect_string::string_and_effects& effect_string::back_() const {
            return strings_.back();
        }

        effect_string& effect_string::operator<<(const effect_string& arg) {
            for(const auto& string : arg.strings_) {
                strings_.push_back(string);
            }

            return *this;
        }

        effect_string& effect_string::operator<<(effect_string&& arg) {
            for(auto& string : arg.strings_) {
                strings_.push_back(std::move(string));
            }

            return *this;
        }

        effect_string& effect_string::operator+=(const effect_string& arg) {
            return (*this << arg);
        }

        effect_string& effect_string::operator+=(effect_string&& arg) {
            return (*this << std::move(arg));
        }

        persist&& operator<<(persist& p, const effect_string& es) {

            for(const auto& string : es.strings_) {
                p << string.string;

                for(unsigned i = 0; i < string.active_effects_.size(); ++i) {
                    if(string.active_effects_[i]) {
                        detail::reapply_top(*p.stream_, static_cast<effect_type>(i));
                    }
                }
            }

            return std::move(p);
        }

        persist operator<<(std::ostream& os, const effect_string& es) {
            persist ret;
            ret.stream_ = &os;

            ret << es;

            return ret;
        }


        namespace detail {
            #ifdef __unix__
                bool stdout_isatty() {
                    return isatty(STDOUT_FILENO);
                }
                bool stderr_isatty() {
                    return isatty(STDERR_FILENO);
                }
            #elif defined(_WIN32)

            #endif

            bool isatty(const std::ostream& os) {
                if(&os == &std::cout) {
                    return stdout_isatty();
                }
                else if(&os == &std::cerr) {
                    return stderr_isatty();
                }
                else {
                    return false;
                }
            }

            effect create(const char* code, effect_type type) noexcept {
                return {code, type};
            }

            struct effect_type_to_stream_hash_t {
                std::size_t operator()(const std::ostream* os) const {
                    if(((os == &std::cout) || (os == &std::cerr)) && stdout_isatty() && stderr_isatty()) {
                        return std::hash<const std::ostream*>()(&std::cout);
                    }
                    else {
                        return std::hash<const std::ostream*>()(os);
                    }
                }
            };

            struct effect_type_to_stream_equals_t {
                std::size_t operator()(const std::ostream* lhs, const std::ostream* rhs) const {
                    if(((lhs == &std::cout) || (lhs == &std::cerr)) && ((rhs == &std::cout) || (rhs == &std::cerr)) && stdout_isatty() && stderr_isatty()) {
                        return true;
                    }
                    else {
                        return (lhs == rhs);
                    }
                }
            };
                                                                            // the code
            static std::array<std::unordered_map<std::ostream*, std::vector<const char*>, effect_type_to_stream_hash_t, effect_type_to_stream_equals_t>,
                              number_of_effect_types> effect_type_to_stream_to_effect_stack_;

            static std::array<const char*, number_of_effect_types> effect_type_to_default_code_ = {"\x1b[39m",
                                                                                                   "\x1b[49m",
                                                                                                   "\x1b[22m",
                                                                                                   "\x1b[24m",
                                                                                                   "\x1b[25m"};

            static std::ostream* streams_[] = {&std::cout, &std::cerr};

            // returns location in map
            unsigned push_effect(std::ostream& stream, effect_type type, const char* code) {
                auto& map = effect_type_to_stream_to_effect_stack_[type];
                if(!map.count(&stream)) { // I'm using c++14, so .contains() isn't available :(
                    map[&stream].push_back(effect_type_to_default_code_[type]);
                }
                unsigned ret = map.at(&stream).size();
                map.at(&stream).push_back(code);
                stream << code;

                if(stdout_isatty() && stderr_isatty()) {
                    for(unsigned i = 0; i < 2; ++i) {
                        if(&stream == streams_[i]) {
                            *streams_[!i] << code;
                            break;
                        }               // ^ !i turns 0 into 1 and 1 into 0,
                                        // so this basically says "if the stream is cout, also print this effect to cerr,
                                        // and if the stream is cerr, also print this iffect to cout
                    }
                }

                return ret;
            }

            unsigned push_empty_effect(std::ostream& stream, effect_type type) {
                auto& map = effect_type_to_stream_to_effect_stack_[type];
                if(!map.count(&stream)) { // I'm using c++14, so .contains() isn't available :(
                    map[&stream].push_back(effect_type_to_default_code_[type]);
                }
                unsigned ret = map.at(&stream).size();
                map.at(&stream).push_back(nullptr);

                return ret;
            }

            void pop_effect(std::ostream& stream, effect_type type) {
                auto& map = effect_type_to_stream_to_effect_stack_[type];
                auto& stack = map.at(&stream);
                stack.pop_back();

                if(stack.back()) { // don't apply a null code
                    stream << stack.back();

                    if(stdout_isatty() && stderr_isatty()) {
                        for(unsigned i = 0; i < 2; ++i) {
                            if(&stream == streams_[i]) {
                                *streams_[!i] << stack.back();
                                break;
                            }               // ^ !i turns 0 into 1 and 1 into 0,
                            // so this basically says "if the stream is cout, also print this effect to cerr,
                            // and if the stream is cerr, also print this iffect to cout
                        }
                    }
                }
                /*else { no, what were you thinking???
                    pop_effect(stream, type); // pop again because we didn't actually apply anything.
                }                             // this cycle continues as many times as necessary*/

                /*if(stack.size() == 1) {
                    map.erase(&stream);
                }*/
            }

            void set(std::ostream& stream, effect_type type, unsigned index, const char* code) {
                auto& stack = effect_type_to_stream_to_effect_stack_[type].at(&stream);
                stack[index] = code;

                bool is_top_non_emtpy = true;
                for(unsigned i = index+1; i < stack.size(); ++i) {
                    is_top_non_emtpy &= !stack[i];
                }
                if(is_top_non_emtpy) {
                    stream << code;
                }
            }

            void set_top(std::ostream& stream, effect_type type, const char* code) {
                effect_type_to_stream_to_effect_stack_[type].at(&stream).back() = code;
                stream << code;
            }

            void reapply_top(std::ostream& stream, effect_type type) {
                auto& map = effect_type_to_stream_to_effect_stack_[type];
                if(!map.count(&stream)) { // I'm using c++14, so .contains() isn't available :(
                    map[&stream].push_back(effect_type_to_default_code_[type]);
                }
                auto& stack = map.at(&stream);

                for(unsigned i = stack.size(); i > 0; --i) {
                    if(stack[i-1]) { // find first code that isn't null
                        stream << stack[i-1];
                        break;
                    }
                }
            }
        }
    #endif
}
