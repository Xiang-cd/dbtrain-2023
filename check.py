from functools import partial
from itertools import groupby
import operator
from pathlib import Path
import subprocess

class TestCase:
    def __init__(self, name: str, base_dir: Path, filename: Path):
        self.name = name
        self.base_dir = base_dir
        self.filename = filename
        self.result_file = filename.parent.parent / 'result' / f'{name}.result'
        self.tmp_result_file = self.result_file.parent.parent / 'tmp' / f'{name}.tmp'

    def run(self):
        print(self.base_dir)
        subprocess.call(self.base_dir / '..' / '..' / 'dbtrain-lab' / 'build' / 'bin' / 'main', stdin=open(self.filename), stdout=open(self.tmp_result_file, 'w'), stderr=open('/dev/null'))

    def check(self) -> bool:
        with open(self.result_file) as result_file:
            results = result_file.read().splitlines()
            results = [list(g) for _, g in groupby(results, key=partial(operator.ne, ''))]
        with open(self.tmp_result_file) as tmp_result_file:
            tmp_results = tmp_result_file.read().splitlines()
            tmp_results = [list(g) for _, g in groupby(tmp_results, key=partial(operator.ne, ''))]
        if len(results) != len(tmp_results):
            return False
        for i in range(len(results)):
            if set(results[i]) != set(tmp_results[i]):
                return False
        return True

RED = '\033[0;31m'
GREEN = '\033[0;32m'
YELLOW = '\033[0;33m'
NC = '\033[0m'

def main():
    base_dir = Path(__file__).parent.resolve() / 'lab1'
    if not (base_dir / 'tmp').is_dir():
        (base_dir / 'tmp').mkdir()
    test_file_names = sorted((base_dir / 'test').glob('*.test'))
    test_cases = [TestCase(test_file_name.stem, base_dir, test_file_name) for test_file_name in test_file_names]

    success_count = 0
    failure_count = 0
    for i, test_case in enumerate(test_cases):
        test_case.run()
        if test_case.check():
            success_count += 1
            print(f'Test {test_case.name} {GREEN}PASSED{NC}')
        else:
            failure_count += 1
            print(f'Test {test_case.name} {RED}FAILED{NC}')

    print(f'{success_count} / {len(test_cases)} cases PASSED')

if __name__ == '__main__':
    main()
