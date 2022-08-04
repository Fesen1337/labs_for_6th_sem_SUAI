import argparse

from utils import *


init()


parser = argparse.ArgumentParser(description='Build')
parser.add_argument('--config', '-c', default='Debug', choices=['Debug', 'Release', 'RelWithDebInfo'], dest='config', help='Build type')
parser.add_argument('--drop-build', '-d', action='store_true', dest='drop', help='Drop build results')

args = parser.parse_args()
config = args.config

cwd = get_cwd()
folder = cwd + '/cmake-build-' + config

if args.drop:
    call_with_output(['rm', '-rf', folder], 'Remove build folder')
call_with_output(['mkdir', '-p', folder], 'Create build folder')
call_with_output(['cmake', '-DCMAKE_BUILD_TYPE=' + config, '-S', cwd, '-B', folder], 'Rebuild CMake')
call_with_output(['cmake', '--build', folder], 'Build')
