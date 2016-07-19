project('fix_test', 'cpp')

add_global_arguments('-std=gnu++11', language : 'cpp')
#add_global_arguments('-g', language : 'cpp')
add_global_arguments('-O3', language : 'cpp')


src  = [
    'main.cpp',
    ] 

incdir = include_directories([  
        '../', 
        '/opt/toolchain/boost_1_58_0',
        '/opt/toolchain/protobuf-2.6.1/usr/include'
    ])

link = [
    '-L/opt/toolchain/boost_1_58_0/',
    '-L/opt/toolchain/protobuf-2.6.1/usr/lib',
    '-lprotobuf',
    '-lrt'
]

executable(
    'fix_test',
    sources: src,
    include_directories: incdir,
    link_args: link
    )
