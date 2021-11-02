import re
regexCPU = r"czas\sCPU\s*=\s(\d+\.\d+)"
regexZEGAR = r"zegarowy\s*=\s(\d+\.\d+)"

data = {
        "weCPU_t": [],
        "wezegar_t": [],
        "arCPU_t": [],
        "arzegar_t": [],
        }

wewy = True
with open('./logger', 'r+') as f:
    for line in f:
        resultCPU = re.search(regexCPU, line)
        resultZEGAR = re.search(regexZEGAR, line)
        if resultCPU:
            data['weCPU_t'].append(resultCPU) if wewy else data['arCPU_t'].append(resultCPU)
        elif resultZEGAR:
            data['wezegar_t'].append(resultZEGAR) if wewy else data['arzegar_t'].append(resultZEGAR)
            wewy = not wewy

print('checkout:')

# take random key and take it's lenght of list it contains (all keys contain
# items with fixed length)

N = len(data['weCPU_t'])
print(N)
for n in range(N):
    for k, v in data.items():
        print(v[n].group(1), end=';')
    print()

# for k, v in data.items():
#     for i in v:
#         print(i.group(1), end=';')
#     print()

print()
