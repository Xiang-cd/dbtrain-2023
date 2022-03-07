from pathlib import Path

for i in range(1, 6):
    dir = Path(f'lab{i}') / 'result'
    for file in dir.glob('*.result'):
        number = 0
        with open(file, 'r+') as f:
            lines = f.readlines()
            for i, line in enumerate(lines):
                if line.startswith('--'):
                    number += 1
                    lines[i] = line.replace('-- ', f'-- {number}.')
                    f.seek(0)
                    f.write(''.join(lines))
                    f.truncate()
