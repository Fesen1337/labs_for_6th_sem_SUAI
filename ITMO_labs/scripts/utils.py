import colorama
import os
import subprocess
import termcolor


def cprint(s, color):
    termcolor.cprint(s, color, attrs=['bold'], end='')


def get_cwd():
    return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def log(msg, color='yellow'):
    termcolor.cprint(msg + '... ', color, attrs=['bold'], end='', flush=True)


def ok(msg='', color='green'):
    termcolor.cprint('OK', color, attrs=['bold', 'blink'], end='')
    termcolor.cprint(' ' + msg, 'magenta', attrs=['bold'], flush=True)


def print_error_and_exit(msg):
    termcolor.cprint('ERROR!!!', 'red', attrs=['bold', 'blink'])
    termcolor.cprint(msg, 'white', attrs=['bold'], flush=True)
    exit(1)


def call(args, on_error='', cwd=get_cwd()):
    result = subprocess.run(args, capture_output=True, cwd=cwd, text=True, universal_newlines=True)
    if result.returncode != 0:
        print_error_and_exit(on_error + ' failed with return code: ' + str(result.returncode) + '\n\t' + ' '.join(args) +
                             '\n\n\n' + str(result.stdout) + '\n\n\n' + str(result.stderr))
    return str(result.stdout)


def call_with_output(args, on_error='', cwd=get_cwd()):
    termcolor.cprint(' '.join(args), 'white', attrs=['bold'], flush=True)
    result = subprocess.run(args, cwd=cwd, text=True)
    if result.returncode != 0:
        print_error_and_exit(on_error + ' failed with return code: ' + str(result.returncode) + '\n')


def get_git_status():
    return call(['git', 'status', '-s']).rstrip()


def is_windows():
    return os.name == 'nt'


def check_non_windows():
    if is_windows():
        print_error_and_exit('Cannot run script on Windows!')


def check_file_exists(path):
    return os.path.exists(path) and os.path.isfile(path)


def init():
    check_non_windows()
    colorama.init()
