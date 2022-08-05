from utils import *


init()


log('Check \'~/.config/hub\' file')
hub_file = os.getenv('HOME') + '/.config/hub'
if not os.path.exists(hub_file) or not os.path.isfile(hub_file):
    print('')
    print_error_and_exit('Cannot find file ~/.config/hub\n'
                         'Use this doc to create it: https://docs.google.com/document/d/16f_W4NbzalWAeFCASB9dSLhAKsvxEeY9SzKYdVE1DJs/edit?usp=sharing')
login = None
with open(hub_file, 'r') as inp:
    for line in inp.readlines():
        line = line.strip()
        needle = '- user: '
        pos = line.find(needle)
        if pos == -1:
            continue
        login = line[pos + len(needle):]
        break
if login is None:
    print('')
    print_error_and_exit('Wrong content of file ~/.config/hub\n'
                         'Use this doc to create it: https://docs.google.com/document/d/16f_W4NbzalWAeFCASB9dSLhAKsvxEeY9SzKYdVE1DJs/edit?usp=sharing')
ok(login)


log('Check uncommitted changes')
output = get_git_status()
if len(output) != 0:
    print_error_and_exit('You have uncommitted changes on repository:\n' + output +
                         '\n\nTo continue use commands:\n\n\tgit add .\n\tgit commit -m \"<commit message>\"' +
                         '\n\nAfter that repeat script')
ok()


log('Find out current branch')
current_branch = call(['git', 'branch', '--show-current']).rstrip()
if len(current_branch) == 0:
    print_error_and_exit('HEAD detached from any branch!\nCheckout on any branch')
ok(current_branch)


log('Check PR existent')
output = call(['hub', 'pr', 'list']).rstrip()
if len(output) != 0:
    ok('PR presented: ' + output)
    exit(0)
ok('No PR')


log('Checkout on main branch')
call(['git', 'checkout', 'main'])
ok()


log('Count commits on main branch')
output = call(['git', 'rev-list', '--count', 'main']).rstrip()
commits_count = int(output)
ok(str(commits_count))


if commits_count > 1:
    log('Move HEAD to Init commit')
    call(['git', 'checkout', 'HEAD~' + str(commits_count - 1)])
    ok()


feedback_branch = 'feedback'
log('Create feedback branch')
call(['git', 'checkout', '-B', feedback_branch])
ok()


log('Publish feedback branch')
call(['git', 'push', '-uf', 'origin', feedback_branch])
ok()


if commits_count == 1:
    log('Return on main branch')
    call(['git', 'checkout', 'main'])
    ok()

    log('Create empty diff commit')
    call(['git', 'commit', '--allow-empty', '-m', 'Empty diff commit'])
    ok()


log('Return on initial branch ' + current_branch)
call(['git', 'checkout', current_branch])
ok()


log('Create pull request')
pr_message = 'Feedback\n' \
             '\n' \
             ':wave:! GitHub Classroom created this pull request as a place for your teacher to leave feedback on your work. It will update automatically. ' \
             '**Don’t close or merge this pull request**, unless you’re instructed to do so by your teacher.\n' \
             '\n' \
             'In this pull request, your teacher can leave comments and feedback on your code. ' \
             'Click the **Subscribe** button to be notified if that happens.\n' \
             '\n' \
             'Click the **Files changed** or **Commits** tab to see all of the changes pushed to `main` since the assignment started. ' \
             'Your teacher can see this too.\n' \
             '\n' \
             '<details>\n' \
             '<summary>\n' \
             '<strong>Notes for teachers</strong>\n' \
             '</summary>\n' \
             '\n' \
             'Use this PR to leave feedback. Here are some tips:\n' \
             '\n' \
             '- Click the **Files changed** tab to see all of the changes pushed to `main` since the assignment started. ' \
             'To leave comments on specific lines of code, put your cursor over a line of code and click the blue **+** (plus sign). ' \
             'To learn more about comments, read ' \
             '“[Commenting on a pull request](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/commenting-on-a-pull-request)”.\n' \
             '- Click the **Commits** tab to see the commits pushed to `main`. Click a commit to see specific changes.\n' \
             '- If you turned on autograding, then click the **Checks** tab to see the results.\n' \
             '- This page is an overview. It shows commits, line comments, and general comments. You can leave a general comment below.\n' \
             '\n' \
             'For more information about this pull request, read ' \
             '“[Leaving assignment feedback in GitHub]' \
             '(https://docs.github.com/education/manage-coursework-with-github-classroom/leave-feedback-with-pull-requests)”.\n' \
             '</details>\n' \
             '\n' \
             '\n' \
             'Subscribed: @' + login
subprocess.run(['hub', 'pull-request', '-p' if commits_count == 1 else '-f', '-b', feedback_branch, '-h', 'main', '-m', pr_message,
                '--reviewer=burakov28'], cwd=get_cwd(), check=True, text=True)
ok()
