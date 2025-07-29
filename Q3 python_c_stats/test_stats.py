# test_stats.py - Example script to demonstrate usage of the stats_module C extension.

import stats_module

data = [1.0, 2.0, 2.0, 3.0, 4.0, 5.0]

print("Data:", data)
print("Sum:", stats_module.sum_values(data))
print("Mean:", stats_module.mean_values(data))
print("Std Dev:", stats_module.std_dev(data))
print("Mode:", stats_module.mode_value(data))
print("Count:", stats_module.count_values(data))
