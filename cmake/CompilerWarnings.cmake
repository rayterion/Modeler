# cmake/CompilerWarnings.cmake
# Sets a curated set of warnings on a given target.
# Usage: set_modeler_warnings(<target>)

function(set_modeler_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /w14242  # 'identifier': conversion from 'type1' to 'type2', possible loss of data
            /w14263  # 'function': member function does not override any base class virtual member function
            /w14265  # 'classname': class has virtual functions, but destructor is not virtual
            /w14287  # 'operator': unsigned/negative constant mismatch
            /we4289  # non-standard extension: 'variable' used as loop variable declared in for-loop
            /w14296  # 'operator': expression is always 'boolean_value'
            /w14545  # expression before comma evaluates to a function which is missing an argument list
            /w14546  # function call before comma missing argument list
            /w14547  # 'operator': operator before comma has no effect; expected operator with side-effect
            /w14549  # 'operator': operator before comma has no effect; did you intend 'operator'?
            /w14619  # pragma warning: there is no warning number 'number'
            /w14640  # 'instance': construction of local static object is not thread-safe
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wconversion
            -Wsign-conversion
            -Wmisleading-indentation
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
        )
    endif()
endfunction()
