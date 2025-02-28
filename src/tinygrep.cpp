#include <iostream>

#include "tinygrep.hpp"

TinyGrep::TinyGrep(
    std::string pattern,
    std::string file_path
) noexcept(false) : 
    m_path(file_path),
    m_sleuth(pattern)
{
    //to show path only if its a directory
    m_sleuth.set_is_to_show_filename(!m_path.get_is_starting_path_regfile());
}


void TinyGrep::run(void) noexcept
{
    m_path.iterate(
        Path::callback_func_t([this](const char * directory_name) {
                std::string path(directory_name);
                m_sleuth.add_path(path);
            }
        ),
        Path::callback_func_t([this](const char * directory_name) {
                std::string path(directory_name);
                m_sleuth.report_false_hook(path);
            }
        )
    );

    m_sleuth.set_investigation_finished();
}

int TinyGrep::start(
    std::string pattern,
    std::string file_path
) {
    try
    {
        TinyGrep grep(pattern, file_path);
        grep.run();
    }
    catch(std::regex_error &e)
    {
        switch(e.code())
        {
            case std::regex_constants::error_type::_S_error_escape:
            {
                std::cerr << "grep: Trailing backslash";
                break;
            }
            case std::regex_constants::error_type::_S_error_brack:
            {
                std::cerr << "grep: Unmatched [, [^, [:, [., or [=";
                break;
            }
            default:
            {
                std::cerr << "tinygrep: " << e.what();
                break;
            }
        }

        std::cerr << std::endl;

        return EXIT_FAILURE + 1;
    }
    catch(std::exception &e)
    {
        std::cerr << "tinygrep: " << e.what() << std::endl;
        return EXIT_FAILURE + 1;
    }
    catch(...)
    {
        std::cerr << "tinygrep: Unknown error" << std::endl;
        return EXIT_FAILURE + 1;
    }

    return EXIT_SUCCESS;
}
