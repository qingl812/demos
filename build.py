#!/usr/bin/env python3
# coding=utf-8

import platform
import argparse
import os

# 解析命令行参数
parser = argparse.ArgumentParser(description='编译并运行项目')
parser.add_argument('-d', '--direction', required=True,
                    help='specify a source directory.')
parser.add_argument('-t', '--test_name', default='*', help='test case to run')
parser.add_argument('--mingw', action='store_true')
parser.add_argument('--clean', action='store_true')

args = parser.parse_args()
direction = args.direction
test_name = args.test_name
mingw = args.mingw
clean = args.clean

# 判断操作系统
if platform.system() == 'Windows':
    system = 'windows'
else:
    system = 'other'


# 删除文件夹及里面的文件
def rm_dir(dir):
    if system == 'windows':
        os.system(
            'powershell Remove-Item -LiteralPath {}  -Force -Recurs'.format(dir))
    else:
        os.system('rm -rf {}'.format(dir))


# 编译并运行 cmake 项目
def run_cmake(src_dir, build_dir, exe_name, cmake_args='', exe_args=''):
    src_dir = os.path.abspath(src_dir)
    build_dir = os.path.abspath(build_dir)

    if clean and os.path.exists(build_dir):
        rm_dir(build_dir)

    if system == 'windows':
        exe_name += '.exe'

    os.system('cmake -S {} -B {} {}'.format(src_dir, build_dir, cmake_args))
    os.system('cmake --build {}'.format(build_dir))

    # 如果存在 Makefile 文件
    # 可执行文件在 build_dir 目录下
    if os.path.exists(os.path.join(build_dir, 'Makefile')):
        exe_name = os.path.join(build_dir, exe_name)
    # 反之则假定使用的 msvc
    # 可执行文件在 build_dir/Debug 目录下
    else:
        path = os.path.join(build_dir, 'Debug')
        exe_name = os.path.join(path, exe_name)

    os.system('{} {}'.format(exe_name, exe_args))


# 检验参数 direction，是否存在该文件夹
direction = os.path.abspath(direction)
if not os.path.exists(direction):
    print('folder does not exist: '+direction)
    exit(-1)

if mingw:
    cmake_args = "-G \"MinGW Makefiles\""
else:
    cmake_args = ''

run_cmake(src_dir=direction,
          build_dir='build_cpp',
          exe_name='demos_cpp',
          cmake_args=cmake_args,
          exe_args='--gtest_filter='+test_name)
