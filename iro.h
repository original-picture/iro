#pragma once

#include <unordered_map>
#include <stack>
#include <array>
#include <ostream>


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

    public:
        effect_set();

        effect_set(const effect& e);

        effect_set   operator| (const effect& rhs) const;
        effect_set&  operator|=(const effect& rhs)&;
        effect_set&& operator|=(const effect& rhs)&&;

        effect_set   operator| (const effect_set& rhs) const;
        effect_set&  operator|=(const effect_set& rhs)&;
        effect_set&& operator|=(const effect_set& rhs)&&;
    };


    namespace detail {
        void push_effect(std::ostream& stream, effect_type type, const char* code);
        void pop_effect(std::ostream& stream, effect_type type);
        void set_top(std::ostream& stream, effect_type type, const char* code);
    }


    class persist {
        std::array<bool, number_of_effect_types> effects_used_;
        std::ostream* stream_ = nullptr;

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


    #ifdef IRO_IMPL
        effect::effect(const char* code, effect_type type) noexcept : code_(code), type_(type) {}

        
        effect_set::effect_set() {
            type_to_code_.fill(nullptr);
        }

        effect_set::effect_set(const effect& e) {
            type_to_code_[e.type_] = e.code_;
        }
    
        effect_set effect_set::operator|(const effect& rhs) const {
            effect_set ret = *this;
            ret.type_to_code_[rhs.type_] = rhs.code_;

            return ret;
        }
        
        effect_set& effect_set::operator|=(const effect& rhs)& {
            type_to_code_[rhs.type_] = rhs.code_;

            return *this;
        }

        effect_set&& effect_set::operator|=(const effect& rhs)&& {
            return std::move(*this|=rhs);
        }

        effect_set effect_set::operator|(const effect_set& rhs) const {
            effect_set ret;
            ret |= rhs;
            return ret;
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

        persist::persist() {
            /*for(unsigned i = 0; i < effects_.type_to_code_.size(); ++i) {
                const auto& e = effects_.type_to_code_[i];
                if(e) {
                    detail::push_effect()
                }
            }*/

            effects_used_.fill(false);
        }

        persist::persist(std::ostream& os, const effect& e) : stream_(&os) {
            effects_used_.fill(false);

            effects_used_[e.type_] = true;
            detail::push_effect(os, e.type_, e.code_);
        }

        persist::persist(std::ostream& os, const effect_set& e) : stream_(&os) {
            effects_used_.fill(false);

            for(unsigned i = 0; i < e.type_to_code_.size(); ++i) {
                auto& el =  e.type_to_code_[i];
                if(el) {
                    effects_used_[i] = true;
                    detail::push_effect(os, static_cast<effect_type>(i), el);
                }
            }
        }

        persist::persist(persist&& other) {
            stream_ = other.stream_;

            effects_used_ = other.effects_used_;
            other.effects_used_ = {};
        }
        persist& persist::operator=(persist&& rhs) {
            stream_ = rhs.stream_;

            std::swap(effects_used_, rhs.effects_used_);

            return *this;
        }

        persist&& persist::operator<<(const effect& e) {
            if(effects_used_[e.type_]) {
                detail::set_top(*stream_, e.type_, e.code_);
            }
            else {
                detail::push_effect(*stream_, e.type_, e.code_);
                effects_used_[e.type_] = true;
            }
            return std::move(*this);
        }

        persist&& persist::operator<<(const effect_set& es) {
            for(unsigned i = 0; i < es.type_to_code_.size(); ++i) {
                auto type = static_cast<effect_type>(i);
                auto code = es.type_to_code_[i];

                if(effects_used_[type]) {
                    detail::set_top(*stream_, type, code);
                }
                else {
                    detail::push_effect(*stream_, type, code);
                    effects_used_[type] = true;
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
            for(unsigned i = 0; i < effects_used_.size(); ++i) {
                if(effects_used_[i]) {
                    detail::pop_effect(*stream_, static_cast<effect_type>(i));
                }
            }
        }


        namespace detail {
            effect create(const char* code, effect_type type) noexcept {
                return {code, type};
            }
                                                                            // the code
            static std::array<std::unordered_map<std::ostream*, std::stack<const char*>>,
                              number_of_effect_types> effect_type_to_stream_to_effect_stack_;
            
            static std::array<const char*, number_of_effect_types> effect_type_to_default_code_ = {"\x1b[39m",
                                                                                                   "\x1b[49m",
                                                                                                   "\x1b[22m",
                                                                                                   "\x1b[24m",
                                                                                                   "\x1b[25m"};

            void push_effect(std::ostream& stream, effect_type type, const char* code) {
                auto& map = effect_type_to_stream_to_effect_stack_[type];
                if(!map.count(&stream)) { // I'm using c++14, so .contains() isn't available :(
                    map[&stream].push(effect_type_to_default_code_[type]);
                }
                map.at(&stream).push(code);
                stream << code;
            }

            void pop_effect(std::ostream& stream, effect_type type) {
                auto& map = effect_type_to_stream_to_effect_stack_[type];
                auto& stack = map.at(&stream);
                stack.pop();
                stream << stack.top();

                /*if(stack.size() == 1) {
                    map.erase(&stream);
                }*/
            }

            void set_top(std::ostream& stream, effect_type type, const char* code) {
                effect_type_to_stream_to_effect_stack_[type].at(&stream).top() = code;
                stream << code;
            }
        }
    #endif
}
