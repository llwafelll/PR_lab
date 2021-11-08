import pandas as pd
import numpy as np
import itertools as it
import re
from collections import defaultdict
import os
import enum

# Building path
BASENAME = os.path.join(os.getcwd(), 'tempPR', 'PR_lab', 'lab_5')
FILENAME = "data"

# Regular expressions
settings_regex = re.compile(r"(?<==)\d+(?:.\d+)*")
get_data_regex = re.compile(r"(?:((?P<mode>[a-z]+\_[a-z]+|(?:[a-z]+)))\s*|\s(?P<values>\d+\.\d+))")

# Building columns structure using itertools
c1 = list(it.product(('integral', ), ('a', 'b', 'dx', 'threads')))
c2 = list(it.product(('seq', 'block', 'cycle', 'decomp'), ('value', 'time')))

# Creation of multiindex based on prepared c1 and c2 lists
_index = pd.MultiIndex.from_tuples([*c1, *c2])

# Initialization of the DataFrame
df = pd.DataFrame(columns=_index)

with open(os.path.join(BASENAME, FILENAME), "r") as f:
    data = defaultdict(defaultdict)

    while (line := f.readline()):
        if line[:3] == '===':
            i = settings_regex.finditer(line)
            om = (o for o in i) # helper generator
            a, b, dx, w = [x.group(0) for x in om]

            # Append new row filled with data
            updater = pd.DataFrame([[a, b, dx, w]],
                                   columns=pd.MultiIndex.from_tuples([*c1]))
            df = df.append(updater)

        elif line != '\n':
            i = get_data_regex.findall(line)
            _dict = defaultdict(list)

            z = [v[2] if c else v[0] for c, v in enumerate(i)]
            _dict[z[0]].append(z[1])
            _dict[z[0]].append(z[2])

            # Fill rest of the colums with data for each row
            for k, v in _dict.items():
                if k == "sekwencyjnie":
                    value, time = v[0], v[1]
                    
                    labels = it.product(('seq', ), ('time', 'value'))
                    _columns = pd.MultiIndex.from_tuples(labels)
                    to_update = pd.DataFrame([[time, value]], columns=_columns)
                    df.iloc[-1:].update(to_update)

                elif k == "zrownolegl_blok":
                    value, time = v[0], v[1]
                    
                    labels = it.product(('block', ), ('time', 'value'))
                    _columns = pd.MultiIndex.from_tuples(labels)
                    to_update = pd.DataFrame([[time, value]], columns=_columns)
                    df.iloc[-1:].update(to_update)

                elif k == "zrownolegl_cykl":
                    value, time = v[0], v[1]

                    labels = it.product(('cycle', ), ('time', 'value'))
                    _columns = pd.MultiIndex.from_tuples(labels)
                    to_update = pd.DataFrame([[time, value]], columns=_columns)
                    df.iloc[-1:].update(to_update)

                elif k == "dekompozycja":
                    value, time = v[0], v[1]

                    labels = it.product(('decomp', ), ('time', 'value'))
                    _columns = pd.MultiIndex.from_tuples(labels)
                    to_update = pd.DataFrame([[time, value]], columns=_columns)
                    df.iloc[-1:].update(to_update)

print(df)
df.to_csv("results2.csv")



