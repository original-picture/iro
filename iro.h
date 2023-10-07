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
                const effect& grey = bright_black;
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
                const effect& background_grey = bright_black;
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

    namespace detail {

    }


    #ifdef IRO_IMPL
        effect::effect(const char* code, effect_type type) noexcept : code_(code), type_(type) {}

        namespace detail {
            effect create(const char* code, effect_type type) noexcept {
                return {code, type};
            }

            struct effect_type_and_stream_ptr {
                const std::ostream* os;
                const effect_type type;

                bool operator==(const effect_type_and_stream_ptr& rhs) const {
                    return (os   == rhs.os) &&
                           (type == rhs.type);
                }
            };

            template <class T>
            inline void hash_combine(std::size_t& seed, const T& v) // this is just boost::hash_combine
            {
                std::hash<T> hasher;
                seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            }

            struct effect_type_and_stream_ptr_hash_t {
                std::size_t operator()(const effect_type_and_stream_ptr& arg) {
                    std::size_t hash = 0;

                    hash_combine(hash, arg.os);
                    hash_combine(hash, arg.type);

                    return hash;
                }
            };

            static std::array<std::unordered_map<effect_type_and_stream_ptr, std::stack<effect>, effect_type_and_stream_ptr_hash_t>,
                              number_of_effect_types> effect_type_to_stream_to_effect_stack;
        }
    #endif
}
