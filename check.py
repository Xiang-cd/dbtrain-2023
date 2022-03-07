import argparse
from functools import partial
from itertools import groupby
import operator
from pathlib import Path
import subprocess

RED = '\033[0;31m'
GREEN = '\033[0;32m'
YELLOW = '\033[0;33m'
NC = '\033[0m'

class TestCase:
    def __init__(self, name: str, base_dir: Path, filename: Path):
        self.name = name
        self.base_dir = base_dir
        self.filename = filename
        self.result_file = filename.parent.parent / 'result' / f'{name}.result'
        self.tmp_result_file = self.result_file.parent.parent / 'tmp' / f'{name}.tmp'

    def run(self):
        subprocess.call([self.base_dir / '..' / '..' / 'dbtrain-lab' / 'build' / 'bin' / 'main', '-s'], stdin=open(self.filename), stdout=open(self.tmp_result_file, 'w'), stderr=open('/dev/null'))

    def print_mismatch(self, expected: str, got: str):
        print('Expected: ')
        print(f'{expected}')
        print('Got: ')
        print(f'{got}')


    def check(self) -> bool:
        with open(self.result_file) as result_file:
            results = result_file.read().splitlines()
            results = [[i for i in g if not i.startswith('--')] for _, g in groupby(results, key=partial(operator.ne, ''))]
            results = list(filter([''].__ne__, results))
        with open(self.tmp_result_file) as tmp_result_file:
            tmp_results = tmp_result_file.read().splitlines()
            tmp_results = [list(g) for _, g in groupby(tmp_results, key=partial(operator.ne, ''))]
            tmp_results = list(filter([''].__ne__, tmp_results))
        if len(results) != len(tmp_results):
            print(f'{YELLOW}Incorrect number of results{NC}')
            self.print_mismatch(len(results), len(tmp_results))
            return False
        for i in range(len(results)):
            results[i].sort()
            tmp_results[i].sort()
            if results[i] != tmp_results[i]:
                print(f'SQL {i+1}')
                if len(results[i]) != len(tmp_results[i]):
                    print(f'{YELLOW}Incorrect length{NC}')
                    self.print_mismatch(len(results[i]), len(tmp_results[i]))
                else:
                    for j in range(len(results[i])):
                        if results[i][j] != tmp_results[i][j]:
                            print(f'{YELLOW}Incorrect result{NC}')
                            self.print_mismatch(results[i][j], tmp_results[i][j])
                            break
                return False
        return True


def parse():
    parser = argparse.ArgumentParser()
    parser.add_argument('-u', '--until', type=int, default=1000)
    parser.add_argument('-l', '--lab', type=int, default=1)
    args = parser.parse_args()
    return args


def get_test_cases(until, test_file_names, base_dir):
    test_cases = []
    for test_file_name in test_file_names:
        test_name = test_file_name.stem
        test_index = int(test_name.split('_')[0])
        if test_index <= until:
            test_cases.append(TestCase(test_name, base_dir, test_file_name))
    return test_cases


def main():
    args = parse()

    base_dir = Path(__file__).parent.resolve() / f'lab{args.lab}'
    if not (base_dir / 'tmp').is_dir():
        (base_dir / 'tmp').mkdir()
    test_file_names = sorted((base_dir / 'test').glob('*.sql'))
    test_cases = get_test_cases(args.until, test_file_names, base_dir)

    success_count = 0
    failure_count = 0
    for test_case in test_cases:
        test_case.run()
        if test_case.check():
            success_count += 1
            print(f'Test {test_case.name} {GREEN}PASSED{NC}')
            print()
        else:
            failure_count += 1
            print(f'Test {test_case.name} {RED}FAILED{NC}')
            print()

    print(f'{success_count} / {len(test_cases)} cases PASSED')

if __name__ == '__main__':
    main()
