import pandas as pd

input_file = 'out.csv'

df = pd.read_csv(input_file)

filtered_df = df[df.iloc[:, 11] <= 0.89]

output_file = 'bad.csv'

filtered_df.to_csv(output_file, index=False)
