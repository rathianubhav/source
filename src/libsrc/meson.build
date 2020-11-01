libsrc_sources = [
    'exec.cc',
    'eval.cc',
    'label.cc',
    'lexer.cc',
    'parser.cc',
    'st.cc',
    'value.cc',
    'vm.cc'
]

libsrc = shared_library(
    'src',
    libsrc_sources,
    include_directories: inc,
    install: true,
    cpp_args: ['-fpermissive']

)