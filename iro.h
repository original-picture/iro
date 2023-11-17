#pragma once

#include <unordered_map>
#include <stack>
#include <array>
#include <ostream>
#include <sstream>
#include <vector>
#include <cassert>

#if defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__)
    #define IRO_UNIX
    #include <unistd.h>
#elif defined(_WIN32)
    #define IRO_WINDOWS
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
    class effect_set;

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

        friend effect_set   operator|(const effect& e, const effect_set& es);
        friend effect_set&& operator|(const effect& e, effect_set&& es);
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

    class effect_set;
    namespace detail{
        unsigned push_effects(std::ostream* stream, const effect_set& effects);
        void set(std::ostream* stream, unsigned index, const effect_set& effects);
    }
    class effect_set {
        std::array<const char*, number_of_effect_types> type_to_code_;

        friend class persist;
        friend class effect_string;

        friend effect_set   operator|(const effect& e, const effect_set& es);
        friend effect_set&& operator|(const effect& e, effect_set&& es);

        friend unsigned detail::push_effects(std::ostream* stream, const effect_set& effects);
        friend void detail::set(std::ostream* stream, unsigned index, const effect_set& effects);

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

    effect_set   operator|(const effect& e1, const effect& e2);
    effect_set   operator|(const effect& e, const effect_set& es);
    effect_set&& operator|(const effect& e, effect_set&& es);


    namespace detail {
        struct effect_entry_t {
            bool is_empty; // I'm not a huge fan of there being two distinct empty states, but I can't think of another way to implement the behavior I want
            bool is_destructed;
            std::array<const char*, number_of_effect_types> type_to_code;
        };
        unsigned push_effect(std::ostream* stream, effect_type type, const char* code);
        unsigned push_empty_effect(std::ostream* stream); // TODO: change all instances of effect in function names to persist, because now all effects are bundled into one entry in the stack
        void pop_effect(std::ostream* stream);
        void delete_persist(std::ostream* stream, unsigned index_in_stack);
        void set(std::ostream* stream, unsigned index, effect_type type, const char* code);
        void set_top(std::ostream* stream, effect_type type, const char* code);
        const char* get_top_code(const std::ostream* stream, effect_type type);
        void reapply_top(std::ostream* stream, effect_type type);
        bool persist_has_effect_of_type(std::ostream* stream, unsigned index, effect_type type);
    }


    class effect_string;

    class persist {
        unsigned location_in_stack_;            // 0 is a special value that indicates a persist with no active effects
        std::ostream* stream_ = nullptr;        // 0 is always available because index 0 (the first element in the stack) is always the default effect set

        static constexpr unsigned empty_persist_location = 0;

        friend persist   operator<<(std::ostream&, const effect_string&);
        friend persist&& operator<<(persist&,      const effect_string&);

    public:
        persist() = delete;

        persist(std::ostream& os);

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

        std::ostream& ostream();
        const std::ostream& ostream() const;

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

        template<typename T>
        void init_(std::stringstream& stream, const T& arg) {
            stream << arg;
        }

        template<typename T, typename...Ts, std::enable_if_t<(sizeof...(Ts) > 0), bool> = true>
        void init_(std::stringstream& stream, const T& arg, const Ts&...args) {
            stream << arg;
            init_(stream, args...);
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

        /**
         * Returns an std::string with the correct escape codes embedded
         *
         * This function is unsafe because the conversion to an std::string embeds escape codes directly as character data.
         * This means that the escape codes to undo the effects are
         *
         * You need to make absolutely sure that you don't modify any iro state between the call to unsafe_string and the resultant string being printed to tge given stream
         * And (obviously), don't print the given
         *
         * You should really only use this function if you need to interface with a different printing library
         *
         * @param stream the stream you will eventually print this string to
         */
        std::string unsafe_string(const std::ostream& stream) const;
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
            return std::move(*this |= rhs); // avoids creating unnecessary temporaries
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

        effect_set operator|(const effect& e, const effect_set& es) {
            if(es.type_to_code_[e.type_]) {
                return std::move(es);
            }
            else {
                return es|e;
            }
        }

        effect_set&& operator|(const effect& e, effect_set&& es) { // FIXME:
            if(es.type_to_code_[e.type_]) {
                return std::move(es);
            }
            else {
                return std::move(es |= e);
            }
        }


        persist::persist(std::ostream& os) : stream_(&os) {
            location_in_stack_ = detail::push_empty_effect(stream_);
        }

        persist::persist(std::ostream& os, const effect& e) : stream_(&os) {
            location_in_stack_ = detail::push_effect(stream_, e.type_, e.code_);
        }

        persist::persist(std::ostream& os, const effect_set& e) : stream_(&os) {
            location_in_stack_ = detail::push_effects(stream_, e);
        }

        persist::persist(persist&& other) {
            stream_ = other.stream_;

            location_in_stack_ = other.location_in_stack_;
            other.location_in_stack_ = 0;
        }

        persist& persist::operator=(persist&& rhs) {
            stream_ = rhs.stream_;

            std::swap(location_in_stack_, rhs.location_in_stack_);

            return *this;
        }

        persist&& persist::operator<<(const effect& e) {
            detail::set(stream_, location_in_stack_, e.type_, e.code_);
            /*if(detail::persist_has_effect_of_type(stream_, location_in_stack_, e.type_)) {
                detail::set_top(stream_, e.type_, e.code_);
            }
            else {
                location_in_stack_ = detail::push_effect(stream_, e.type_, e.code_);
            }*/
            return std::move(*this);
        }

        persist&& persist::operator<<(const effect_set& es) {
            detail::set(stream_, location_in_stack_, es);

            return std::move(*this);
        }

        persist operator<<(std::ostream& stream, const effect& e) {
            return {stream, e};
        }

        persist operator<<(std::ostream& stream, const effect_set& e) {
            return {stream, e};
        }

        std::ostream& persist::ostream() {
            return *stream_;
        }

        const std::ostream& persist::ostream() const {
            return *stream_;
        }

        persist::~persist() {
            if(location_in_stack_ != empty_persist_location) {
                detail::pop_effect(stream_); // TODO: replace pop with a delete function that takes an index too
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

        std::string effect_string::unsafe_string(const std::ostream& stream) const {
            std::string ret;
            for(const auto& string : strings_) {
                ret += string.string;

                for(unsigned i = 0; i < string.active_effects_.size(); ++i) {
                    if(string.active_effects_[i]) {
                        ret += detail::get_top_code(&stream, static_cast<effect_type>(i));
                    }
                }
            }

            return ret;
        }

        persist&& operator<<(persist& p, const effect_string& es) {
/*
            for(const auto& string : es.strings_) {
                p << string.string;

                for(unsigned i = 0; i < string.active_effects_.size(); ++i) {
                    if(string.active_effects_[i]) {
                        detail::reapply_top(p.stream_, static_cast<effect_type>(i));
                    }
                }
            }*/

            return p << es.unsafe_string(*p.stream_);
        }

        persist operator<<(std::ostream& os, const effect_string& es) {
            persist ret(os);

            ret << es;

            return ret;
        }

        namespace detail {
            template<typename T>
            struct filled_array_helper_t {
                const T& val;

                filled_array_helper_t(const T& val) : val(val) {}

                template<std::size_t N>
                operator std::array<T,N>() {
                    std::array<T,N> ret;
                    ret.fill(val);
                    return ret;
                }
            };

            template<typename T>
            filled_array_helper_t<T> filled_array(const T& value) {
                return filled_array_helper_t<T>(value);
            }

            template<std::size_t N, typename T>
            std::array<T,N> filled_array(const T& value) {
                std::array<T,N> ret;
                ret.fill(value);
                return ret;
            }

            template<std::size_t N, typename T, typename U>
            std::array<T,N> fill_array_and_set_one_value(const T& fill_value, unsigned index, const U& set_value) {
                auto arr = filled_array<T, N>(fill_value);
                arr[index] = set_value;
                return arr;
            }

            #ifdef IRO_UNIX
                bool stdout_isatty() {
                    return isatty(STDOUT_FILENO);
                }
                bool stderr_isatty() {
                    return isatty(STDERR_FILENO);
                }
            #elif defined(IRO_WINDOWS)

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
            /*static std::array<std::unordered_map<std::ostream*, std::vector<const char*>, effect_type_to_stream_hash_t, effect_type_to_stream_equals_t>,
                              number_of_effect_types> effect_type_to_stream_to_effect_stack_;*/

            std::unordered_map<const std::ostream*,
                               std::vector<std::array<const char*, number_of_effect_types>>,
                               effect_type_to_stream_hash_t, effect_type_to_stream_equals_t> stream_to_stack_;

            static std::array<const char*, number_of_effect_types> effect_type_to_default_code_ = {"\x1b[39m",
                                                                                                   "\x1b[49m",
                                                                                                   "\x1b[22m",
                                                                                                   "\x1b[24m",
                                                                                                   "\x1b[25m"};

            static std::ostream* streams_[] = {&std::cout, &std::cerr};

            // returns location in map
            unsigned push_effect(std::ostream* stream, effect_type type, const char* code) {
                auto ret = push_empty_effect(stream);
                set_top(stream, type, code);

                return ret;
            }

            unsigned push_empty_effect(std::ostream* stream) {
                if(!stream_to_stack_.count(stream)) { // I'm using c++14, so .contains() isn't available :(
                    stream_to_stack_[stream].push_back(effect_type_to_default_code_);
                }
                auto& stack = stream_to_stack_.at(stream);
                unsigned ret = stack.size();
                stack.push_back(filled_array<const char*>(nullptr));

                return ret;
            }

            unsigned push_effects(std::ostream* stream, const effect_set& effects) {
                auto ret = push_empty_effect(stream);
                for(unsigned i = 0; i < number_of_effect_types; ++i) {
                    if(effects.type_to_code_[i]) {
                        set_top(stream, static_cast<effect_type>(i), effects.type_to_code_[i]);
                    }
                }

                return ret;
            }

            void pop_effect(std::ostream* stream) {
                auto& stack = stream_to_stack_.at(stream);
                stack.pop_back();

                for(unsigned effect_type_index = 0; effect_type_index < number_of_effect_types; ++effect_type_index) {
                    for(unsigned index_in_stack = stack.size(); index_in_stack > 0; --index_in_stack) { // iterate backwards and find the first nonempty effect
                        const auto& effect = stack[index_in_stack-1];
                        const auto& e = effect[effect_type_index];
                        if(e) { // don't apply a null code
                            *stream << e;

                            if(stdout_isatty() && stderr_isatty()) { // TODO: put this in a function to get rid of code duplication
                                for(unsigned i = 0; i < 2; ++i) {
                                    if(stream == streams_[i]) {
                                        *streams_[!i] << e;
                                        break;
                                    }               // ^ !i turns 0 into 1 and 1 into 0,
                                    // so this basically says "if the stream is cout, also print this effect to cerr,
                                    // and if the stream is cerr, also print this effect to cout
                                }
                            }

                            break;
                        }
                    }
                }


                /*else { no, what were you thinking???
                 *         don't need to do this because the next effect will be popped automatically when it gets destroyed
                    pop_effect(stream, type); // pop again because we didn't actually apply anything.
                }                             // this cycle continues as many times as necessary*/

                /*if(stack.size() == 1) {
                    map.erase(&stream);
                }*/
            }

            void delete_persist(std::ostream* stream, unsigned index_in_stack) {
                auto& stack = stream_to_stack_.at(stream);

                if(index_in_stack == stack.size()-1) {
                    unsigned i = stack.size();

                    while((--i >= 0) && (stack[i][0]))
                }
            }

            void set(std::ostream* stream, unsigned index, effect_type type, const char* code) {
                auto& stack = stream_to_stack_.at(stream);
                stack[index][type] = code;

                bool is_top_non_empty = true;
                for(unsigned i = index+1; i < stack.size(); ++i) {
                    is_top_non_empty &= !(stack[i][type]);
                }
                if(is_top_non_empty) {
                    *stream << code;

                    if(stdout_isatty() && stderr_isatty()) {
                        for(unsigned i = 0; i < 2; ++i) {
                            if(stream == streams_[i]) {
                                *streams_[!i] << code;
                                break;
                            }               // ^ !i turns 0 into 1 and 1 into 0,
                                            // so this basically says "if the stream is cout, also print this effect to cerr,
                                            // and if the stream is cerr, also print this effect to cout
                        }
                    }
                }
            }

            void set(std::ostream* stream, unsigned index, const effect_set& effects) {
                for(unsigned i = 0; i < effects.type_to_code_.size(); ++i) {
                    if(effects.type_to_code_[i]) { // TODO: In order to have this work with persist assignment operators, you'll have to change this so that when the code is empty,
                                                   // it cleans up the old persists state. This will involve walking down the stack and finding the last non-empty code for the given effect type
                        set(stream, index, static_cast<effect_type>(i), effects.type_to_code_[i]);
                    }
                }
            }

            bool persist_has_effect_of_type(std::ostream* stream, unsigned index, effect_type type) {
                return stream_to_stack_.at(stream)[index][type];
            }

            void set_top(std::ostream* stream, effect_type type, const char* code) {
                set(stream, stream_to_stack_.at(stream).size()-1, type, code);
            }

            const char* get_top_code(const std::ostream* stream, effect_type type) {
                if(!stream_to_stack_.count(stream)) { // I'm using c++14, so .contains() isn't available :(
                    stream_to_stack_.at(stream).push_back(effect_type_to_default_code_);
                }
                auto& stack = stream_to_stack_.at(stream);

                for(unsigned i = stack.size(); i > 0; --i) {
                    if(stack[i-1][type]) { // find first code that isn't null
                        return stack[i-1][type];
                    }
                }

                assert(false);
            }

            void reapply_top(std::ostream* stream, effect_type type) {
                *stream << get_top_code(stream, type);

                /*if(stream_to_stack_.count(stream)) {
                    for(const auto& e : stream_to_stack_.at(stream).back()) {
                        if(e) {
                            *stream << e;
                        }
                    }
                }*/
            }
        }
    #endif
}
