import argparse
import json

from utils import *


init()


DEFAULT_ONE_TEST_TIMEOUT_S = 0.75


parser = argparse.ArgumentParser(description='Run tests')
parser.add_argument('--config', '-c', default='Debug', choices=['Debug', 'Release', 'RelWithDebInfo'], dest='config', help='Specify build type')
parser.add_argument('--filter', '-f', default=None, dest='filter', help='Specify gtest filter for tests')
parser.add_argument('--drop-build', '-d', action='store_true', dest='drop', help='Force drop build results')
parser.add_argument('--timeout', '-s', type=float, default=DEFAULT_ONE_TEST_TIMEOUT_S, dest='timeout_s', help='Timeout for each test in seconds')

args = parser.parse_args()
config = args.config
timeout_s = args.timeout_s

build_folder = get_cwd() + '/cmake-build-' + config
test_file = build_folder + '/tests'

call_with_output(['python3', 'scripts/build.py', '-c', config] + (['-d'] if args.drop else []))

assert check_file_exists(test_file)

print('')
test_args = [test_file] + (['--gtest_filter=' + args.filter] if args.filter is not None else [])
log('Count tests to run')
tests_count = len([s for s in call(test_args + ['--gtest_list_tests'], cwd=build_folder).rstrip().splitlines() if s.startswith('  ')])
ok(str(tests_count))

print('\n\n', end='')

test_log_file = build_folder + '/' + config + '-tests-log.json'
test_args += ['--gtest_output=json:' + test_log_file]
max_execution_time_s = (max(DEFAULT_ONE_TEST_TIMEOUT_S, tests_count * timeout_s) if config == 'Release' else 1000000.0)
try:
    cprint('Run ', 'yellow')
    cprint(str(tests_count), 'magenta')
    cprint(' tests in ', 'yellow')
    cprint(config, 'magenta')
    cprint(' config', 'yellow')
    if config == 'Release':
        cprint(' with total time limit ', 'yellow')
        cprint(str(max_execution_time_s) + 's', 'magenta')
    print('\n', end='')
    if config == 'RelWithDebInfo':
        test_args = ['valgrind', '--tool=memcheck', '--gen-suppressions=all', '--leak-check=full',
                     '--leak-resolution=med', '--track-origins=yes', '--vgdb=no', '--error-exitcode=1'] + test_args
    result = subprocess.run(test_args, cwd=build_folder, text=True, check=True, timeout=max_execution_time_s)
except subprocess.TimeoutExpired:
    print_error_and_exit('Total time limit ' + str(max_execution_time_s) + 's exceeded for specified tests')
except subprocess.CalledProcessError as e:
    print_error_and_exit('Tests returns non-zero exit code: ' + str(e.returncode))

termcolor.cprint('ALL TESTS ARE CORRECT!!!', 'green', attrs=['bold', 'blink'])

if config != 'Release':
    exit(0)

print('\n\n', end='')

termcolor.cprint('Check each test execution time (' + str(timeout_s) + 's per test): ', 'yellow', attrs=['bold'])
max_test_name_len = 0
with open(test_log_file, 'r') as inp:
    outcome = json.load(inp)
    for ts in outcome['testsuites']:
        for test in ts['testsuite']:
            max_test_name_len = max(max_test_name_len, len(ts['name'] + '.' + test['name']))
    was_timeout = False
    for ts in outcome['testsuites']:
        print('')
        for test in ts['testsuite']:
            test_name = (ts['name'] + '.' + test['name']).ljust(max_test_name_len, ' ')
            termcolor.cprint('  ' + test_name + '\t', 'white', attrs=['bold'], end='')
            dur_s = float((test['time'][:-1]))
            if dur_s <= timeout_s:
                termcolor.cprint('OK     ' + test['time'] + ' <= ' + str(timeout_s) + 's', 'green', attrs=['bold'])
            else:
                was_timeout = True
                termcolor.cprint('FAILED ', 'red', attrs=['bold', 'blink'], end='')
                termcolor.cprint(test['time'] + ' > ' + str(timeout_s) + 's', 'red', attrs=['bold'])

    print('\n')
    if was_timeout:
        termcolor.cprint('All tests are correct but some of them exceed ' + str(timeout_s) + 's timeout', 'red', attrs=['bold'])
        exit(1)
    termcolor.cprint('ALL TESTS ARE PASSED!!!', 'green', attrs=['bold', 'blink'])
