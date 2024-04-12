import os
import pandas as pd

designs = 'ispd18_test'

df = pd.read_csv('estimate_1layer.log', sep='\s\s+', engine='python')
df.to_csv('1layer_estimate_report.csv', index=None)
