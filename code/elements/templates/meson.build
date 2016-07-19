project('REPLACE', 'cpp')

add_global_arguments('-std=gnu++11', language : 'cpp')
add_global_arguments('-Ofast', language : 'cpp')

run_command('rm','*.pb.*')
run_command('protoc','-I=.','--cpp_out=.', 'user.proto')
run_command('protoc','-I=.','--python_out=.', 'user.proto')
run_command('protoc','-I=.','--cpp_out=.', 'params.proto')
run_command('protoc','-I=.','--python_out=.', 'params.proto')
run_command('python','../msg_compiler.py','-i','messages.msg', '-n', 'elements', '-o', 'log_messages.h', '-p', 'printers.h')

TC='/opt_eg/toolchain2'

src  = [
        '../easy_time.cpp',
        'main.cpp',
        'params.pb.cc',
        'user.pb.cc',
    ]

incdir = include_directories([  
        '../', 
        TC + '/boost_1_58_0',
        TC + '/protobuf-2.6.1/usr/include',
        TC + '/sbe-20150922/main/cpp',
        TC + '/blpapi_cpp_3.8.18.1/include',
        TC + '/Targaryen_0.1.4.D/inc',
        TC + '/Targaryen_0.1.4.D/inc/sbe',
        '/usr/lib64'
    ])

link = [
    '-L' + TC + '/boost_1_58_0/lib',
    '-L' + TC + '/protobuf-2.6.1/usr/lib',
    '-L' + TC + '/blpapi_cpp_3.8.18.1/Linux',
    '-L' + TC + '/Targaryen_0.1.4.D/lib',
    '-lprotobuf',
    '-lblpapi3_64',
    '-lTargaryen',
    '-lPocoUtil',
    '-lPocoNet',
    '-lPocoXML',
    '-lPocoFoundation',
    '-lPocoJSON',
    '-lboost_system',
    '-lboost_thread',
    '-lboost_filesystem',
    '-lrt'
    ]

executable(
    'REPLACE',
    sources: src,
    include_directories: incdir,
    link_args: link
    )
