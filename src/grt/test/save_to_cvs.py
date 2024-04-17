import os
import pandas as pd

designs = 'ispd18_test'

df = pd.read_csv('estimate_2layer.log', sep='\s\s+', engine='python')
df.to_csv('2layer_estimate_report.csv', index=None)
