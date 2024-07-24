import pandas as pd
import numpy as np
from scipy import stats

def calculate_stats(filename):
    # Read the CSV file
    df = pd.read_csv(filename)
    
    # Extract temperatures
    temperatures = df['Temperature(K)'].values
    
    # Calculate mean
    mean_temp = np.mean(temperatures)
    
    # Calculate mode
    mode_result = stats.mode(temperatures)
    if isinstance(mode_result, np.ndarray):
        mode_temp = mode_result[0]
    elif hasattr(mode_result, 'mode'):
        mode_temp = mode_result.mode
    else:
        # If mode is a scalar, use it directly
        mode_temp = mode_result
    
    # Ensure mode_temp is a scalar
    if isinstance(mode_temp, np.ndarray):
        mode_temp = mode_temp[0]
    
    # Calculate standard deviation
    std_dev_temp = np.std(temperatures)
    
    return (mean_temp, mode_temp, std_dev_temp)

# # For testing
# if __name__ == "__main__":
#     result = calculate_stats("temperature.csv")
#     print(f"Mean: {result[0]}")
#     print(f"Mode: {result[1]}")
#     print(f"Standard Deviation: {result[2]}")
