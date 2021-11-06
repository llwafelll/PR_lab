import pandas as pd
import numpy as np
import itertools as it
import re
from collections import defaultdict


settings_regex = re.compile(r"(?<==)\d+(?:.\d+)*")
concurrent_regex = re.compile(r"(\w+| ).*?(\d+\.\d+)")
get_data_regex = re.compile(r"(?:((?P<mode>[a-z]+\_[a-z]+|(?:[a-z]+)))\s*|\s(?P<values>\d+\.\d+))")
# sekw_regex = re.compile(r"sekw.*?((?<=:\s)\d+\.\d+)(?:\s)(\d+\.\d+)")
# blok_regex = re.compile(r"blok.*?((?<=:\s)\d+\.\d+)(?:\s)(\d+\.\d+)")
# cykl_regex = re.compile(r"cykl.*?((?<=:\s)\d+\.\d+)(?:\s)(\d+\.\d+)")

c1 = it.product(('integral', ), ('a', 'b', 'dx', 'threads'))
c2 = it.product(('seq', 'block', 'cycle'), ('value', 'time'))
_index = pd.MultiIndex.from_tuples([*c1, *c2])
df = pd.DataFrame(columns=_index)

with open("./data", "r") as f:
    data = defaultdict(defaultdict)
    while (l := f.readline()):
        if l[:3] == '===':
            i = settings_regex.finditer(l)
            om = (o for o in i)
            a, b, dx, w = [x.group(0) for x in om]

            c1 = it.product(('integral', ), ('a', 'b', 'dx', 'threads'))
            updater = pd.DataFrame([[a, b, dx, w]], columns=pd.MultiIndex.from_tuples([*c1]))
            df = df.append(updater)

            
        elif l != '\n':
            i = get_data_regex.findall(l)

            cseq = it.product(('seq', 'block', 'cycle'), ('value', 'time'))
            cseq = it.product(('seq', 'block', 'cycle'), ('value', 'time'))
            cseq = it.product(('seq', 'block', 'cycle'), ('value', 'time'))

            defd = defaultdict(list)
            z = [v[2] if c else v[0] for c, v in enumerate(i)]
            defd[z[0]].append(z[1])
            defd[z[0]].append(z[2])

            for k, v in defd.items():
                if k == 'sekwencyjnie':
                    value, time = v[0], v[1]
                    u = pd.DataFrame([[time, value]], columns=pd.MultiIndex.from_tuples(it.product(('seq', ), ('time', 'value'))))
                    # df.iloc[-1:]['time'] = time
                    df.iloc[-1:].update(u)
                    # df.iloc[-1:].update(u)
                    # print(df)
                
print(df)



