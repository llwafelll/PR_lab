import re
regexCPU = r"czas\sCPU\s*=\s(\d+\.\d+)"
regexZEGAR = r"zegarowy\s*=\s(\d+\.\d+)"

data = {
        "CPU_t": [],
        "zegar_t": [],
        }

with open('./log', 'r+') as f:
    for line in f:
        resultCPU = re.search(regexCPU, line)
        resultZEGAR = re.search(regexZEGAR, line)
        if resultCPU:
            data['CPU_t'].append(resultCPU)
        elif resultZEGAR:
            data['zegar_t'].append(resultZEGAR)

print('checkout:')

N = len(data['CPU_t'])
print(N)
for n in range(N):
    for k, v in data.items():
        print(v[n].group(1), end=';')
    print()

print()

